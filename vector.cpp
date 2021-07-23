#include "vector.hpp"
#include <math.h>

Vector::Vector()
{
    data = new double[3];
    end = data + 3;

    for(double *ptr = data; ptr != end; ++ptr)
        *ptr = 0.0;
}


Vector::Vector(double x, double y, double z)
{
    data = new double[3];
    end = data + 3;
    
    double* ptr = data;
    *(ptr++) = x;
    *(ptr++) = y;
    *(ptr  ) = z;
}

Vector::Vector(double *ptr)
{
    data = new double[3];
    end = data + 3;
    
    for(double *t_ptr = data, *o_ptr = ptr; t_ptr != end; ++t_ptr, ++o_ptr)
        *t_ptr = *o_ptr;
}

Vector::Vector(const Vector &other)
{
    data = new double[3];
    end = data + 3;
    
    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        *t_ptr = *o_ptr;
}

Vector::~Vector()
{
    delete [] data;
}

Vector& Vector::operator=(const Vector &other)
{
    data = new double[3];
    end = data + 3;

    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        *t_ptr = *o_ptr;

    return *this;
}
Vector Vector::operator+(const Vector &other) const
{
    Vector result(*this);

    for(double *t_ptr = result.data, *o_ptr = other.data; t_ptr != result.end; ++t_ptr, ++o_ptr)
        *t_ptr += *o_ptr;

    return result;
}

Vector Vector::operator-(const Vector &other) const
{
    Vector result(*this);
    for(double *t_ptr = result.data, *o_ptr = other.data; t_ptr != result.end; ++t_ptr, ++o_ptr)
        *t_ptr -= *o_ptr;

    return result;
}

Vector operator-(const Vector &other)
{
    
    Vector result(other);
    for(double *t_ptr = result.data; t_ptr != result.end; ++t_ptr)
        *t_ptr = -*t_ptr;

    return result;
}

Vector Vector::operator*(double lambda) const
{
    Vector result(*this);
    for(double *ptr = result.data; ptr != result.end; ++ptr)
        *ptr *= lambda;

    return result;
}

Vector operator*(double lambda, const Vector &other)
{
    Vector result(other);
    for(double *ptr = result.data; ptr != result.end; ++ptr)
        *ptr *= lambda;

    return result;
}

Vector Vector::operator/(double lambda) const
{
    Vector result(*this);
    for(double *ptr = result.data; ptr != result.end; ++ptr)
        *ptr /= lambda;

    return result;
}

Vector &Vector::operator+=(const Vector &other)
{
    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        *t_ptr += *o_ptr;

    return *this;
}

Vector &Vector::operator-=(const Vector &other)
{
    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        *t_ptr -= *o_ptr;

    return *this;
}

Vector &Vector::operator*=(double lambda)
{
    for(double *ptr = data; ptr != end; ++ptr)
        *ptr *= lambda;

    return *this;
}

Vector &Vector::operator/=(double lambda)
{
    for(double *ptr = data; ptr != end; ++ptr)
        *ptr /= lambda;

    return *this;
}

double &Vector::operator[](int i)
{
    return *(data + i);
}

double Vector::operator[](int i) const
{
    return *(data + i);
}

double &Vector::x()
{
    return data[0];
}
double &Vector::y()
{
    return data[1];
}
double &Vector::z()
{
    return data[2];
}
char* Vector::get_data_pointer() const
{
    return (char*)data;
}

bool Vector::operator==(const Vector &other) const
{
    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        if(*t_ptr != *o_ptr)
            return false;
    return true;
}

bool Vector::operator!=(const Vector &other) const
{
    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        if(*t_ptr != *o_ptr)
            return true;
    return false;
}

double Vector::dot(const Vector &other) const
{
    double result = 0.0;

    for(double *t_ptr = data, *o_ptr = other.data; t_ptr != end; ++t_ptr, ++o_ptr)
        result += (*t_ptr) * (*o_ptr);

    return result;
}

double dot(const Vector &first, const Vector &second)
{
    double result = 0.0;

    for(double *f_ptr = first.data, *s_ptr = second.data; f_ptr != first.end; ++f_ptr, ++s_ptr)
        result += (*f_ptr) * (*s_ptr);

    return result;
}

Vector Vector::cross(const Vector &other) const
{
    Vector result;
    double *ptr = result.data;
    *(ptr++) = data[1] * other.data[2] - data[2] * other.data[1];
    *(ptr++) = data[2] * other.data[0] - data[0] * other.data[2];
    *(ptr  ) = data[0] * other.data[1] - data[1] * other.data[0];

    return result;
}

Vector cross(const Vector &first, const Vector &second)
{
    Vector result;
    double *ptr = result.data;
    *(ptr++) = first.data[1] * second.data[2] - first.data[2] * second.data[1];
    *(ptr++) = first.data[2] * second.data[0] - first.data[0] * second.data[2];
    *(ptr  ) = first.data[0] * second.data[1] - first.data[1] * second.data[0];

    return result;
}

double Vector::len() const
{
    return sqrt(dot(*this));
}
