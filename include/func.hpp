#pragma once

#include <cmath>
#include <vector>
#include <list>

#include "vector.hpp"

struct LatticeV
{
    Vector s;
    double d;
    double A;
};

struct Ray
{
    Vector p;
    Vector v;
    double l;
    double I;
};

struct Spectrum
{
    std::list<double> l;
    std::list<double> I;
};

extern Ray Source; 
extern Ray Holder;
extern Ray Detector;

void read_setup(const char* filename);
void read_lattice(const char* filename);
void read_spectrum(const char* filename);

void init(std::vector<Ray> &Rays);
void out(const char* filename, std::vector<Ray> &Rays);

void to_origin(Ray origin_ray, std::vector<Ray> &Rays);
void intersection(std::vector<Ray> &Rays);

void diffraction(std::vector<Ray> &Rays);

