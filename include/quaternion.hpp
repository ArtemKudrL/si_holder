#pragma once

#include "vector.hpp"
#include <cmath>

class Quaternion
{
public:
    Quaternion();
    Quaternion(double s, double i, double j, double k);
    Quaternion(double s, const Vector &p);
    Quaternion(const Quaternion &other);

    ~Quaternion();

    Quaternion operator+(const Quaternion &other) const;
    Quaternion operator-(const Quaternion &other) const;
    friend Quaternion operator-(const Quaternion &other);

    Quaternion operator*(double lambda) const;
    friend Quaternion operator*(double lambda, const Quaternion &other);
    Quaternion operator/(double lambda) const;

    Quaternion &operator+=(const Quaternion &other);
    Quaternion &operator-=(const Quaternion &other);

    Quaternion &operator*=(double lambda);
    Quaternion &operator/=(double lambda);

    double &operator[](int i);
    double operator[](int i) const;
    
    double &Re();
    Vector &Im();

    bool operator==(const Quaternion &other) const;
    bool operator!=(const Quaternion &other) const;

    Quaternion operator*(const Quaternion &other) const;
    Quaternion operator/(const Quaternion &other) const;

    Quaternion conj() const;
    friend Quaternion conj(const Quaternion &other);
    double len() const;
    friend double len(const Quaternion &other);

    void rotation(double angle, const Vector &axis);
    friend Quaternion rotation(double angle, const Vector &axis);
    void rotation(const Vector &from, const Vector &to);
    friend Quaternion rotation(const Vector &from, const Vector &to);
    void rotate(Vector &other);
private:
    double s;
    Vector p;
};

Quaternion rotation(const Vector &from, const Vector &to);
Quaternion rotation(double angle, const Vector &axis);
