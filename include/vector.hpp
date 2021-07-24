#pragma once

class Vector
{
public:
    Vector();
    Vector(double x, double y, double z);
    Vector(double *ptr);
    Vector(const Vector &other);
    ~Vector();

    Vector& operator=(const Vector &other);

    Vector operator+(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    friend Vector operator-(const Vector &other);

    Vector operator*(double lambda) const;
    friend Vector operator*(double lambda, const Vector &other);
    Vector operator/(double lambda) const;

    Vector &operator+=(const Vector &other);
    Vector &operator-=(const Vector &other);

    Vector &operator*=(double lambda);
    Vector &operator/=(double lambda);

    double &operator[](int i);
    double operator[](int i) const;

    double &x();
    double &y();
    double &z();

    char* get_data_pointer() const;

    bool operator==(const Vector &other) const;
    bool operator!=(const Vector &other) const;

    double dot(const Vector &other) const;
    friend double dot(const Vector &first, const Vector &second);
    Vector cross(const Vector &other) const;
    friend Vector cross(const Vector &first, const Vector &second);
    double len() const;

    double* data;
    double* end;
};
