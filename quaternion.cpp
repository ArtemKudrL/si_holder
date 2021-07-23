#include "quaternion.hpp"

Quaternion::Quaternion() {}
Quaternion::Quaternion(double s, double i, double j, double k) : s(s), p(i, j, k) {}
Quaternion::Quaternion(double s, const Vector &p) : s(s), p(p) {}
Quaternion::Quaternion(const Quaternion &other) : s(other.s), p(other.p) {}

Quaternion::~Quaternion() {}

Quaternion Quaternion::operator+(const Quaternion &other) const
{
    return Quaternion(s + other.s, p + other.p);
}
Quaternion Quaternion::operator-(const Quaternion &other) const
{
    return Quaternion(s - other.s, p - other.p);
}
Quaternion operator-(const Quaternion &other)
{
    return Quaternion(-other.s, -other.p);
}

Quaternion Quaternion::operator*(double lambda) const
{
    return Quaternion(s * lambda, p * lambda);
}
Quaternion operator*(double lambda, const Quaternion &other)
{
    return Quaternion(other.s * lambda, other.p * lambda);
}
Quaternion Quaternion::operator/(double lambda) const
{
    return Quaternion(s / lambda, p / lambda);
}

Quaternion &Quaternion::operator+=(const Quaternion &other)
{
    s += other.s;
    p += other.p;
    return *this;
}
Quaternion &Quaternion::operator-=(const Quaternion &other)
{
    s -= other.s;
    p -= other.p;
    return *this;
}

Quaternion &Quaternion::operator*=(double lambda)
{
    s *= lambda;
    p *= lambda;
    return *this;
}
Quaternion &Quaternion::operator/=(double lambda)
{
    s /= lambda;
    p /= lambda;
    return *this;
}

double &Quaternion::operator[](int i)
{
    return (i == 0) ? s : p[i - 1];
}
double Quaternion::operator[](int i) const
{
    return (i == 0) ? s : p[i - 1];
}

double &Quaternion::Re()
{
    return s;
}
Vector &Quaternion::Im()
{
    return p;
}

bool Quaternion::operator==(const Quaternion &other) const
{
    return s == other.s && p == other.p;
}
bool Quaternion::operator!=(const Quaternion &other) const
{
    return s != other.s || p != other.p;
}

Quaternion Quaternion::operator*(const Quaternion &other) const
{
    return Quaternion(s * other.s - dot(p, other.p), s * other.p + p * other.s + cross(p, other.p));
}
Quaternion Quaternion::operator/(const Quaternion &other) const
{
    return (*this) * other.conj() / (other.s * other.s + dot(other.p, other.p));
}

Quaternion Quaternion::conj() const
{
    return Quaternion(s, -p);
}
Quaternion conj(const Quaternion &other)
{
    return other.conj();
}
double Quaternion::len() const
{
    return sqrt(s * s + p.dot(p));
}
double len(const Quaternion &other)
{
    return other.len(); 
}

void Quaternion::rotation(double angle, const Vector &axis)
{
    s = cos(angle * 0.5);
    p = axis * sin(angle * 0.5) / axis.len();
}
Quaternion rotation(double angle, const Vector &axis)
{
    Quaternion result;
    result.s = cos(angle * 0.5);
    result.p = axis * sin(angle * 0.5) / axis.len();
    return result;
}
void Quaternion::rotation(const Vector &from, const Vector &to)
{
    Vector a = from / from.len();
    Vector b = to / to.len();

    double cosAB = dot(a, b);
    double s = sqrt( 2.0 * (1.0 + cosAB) );

    *this = Quaternion(s * 0.5, cross(a, b) / s);
}
Quaternion rotation(const Vector &from, const Vector &to)
{
    Vector a = from / from.len();
    Vector b = to / to.len();

    double cosAB = dot(a, b);
    double s = sqrt( 2.0 * (1.0 + cosAB) );

    return Quaternion(s * 0.5, cross(a, b) / s);
}
Vector Quaternion::rotate(const Vector &other)
{
    return ((*this) * Quaternion(0, other) * (*this).conj()).p;
}
