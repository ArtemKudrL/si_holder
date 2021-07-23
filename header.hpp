#pragma once

#include <cmath>
#include <vector>
#include <list>

#include "vector.hpp"

struct LatticeV
{
    Vector s;
    double d;
    double SA;
};

struct Spectrum
{
    std::list<double> lambda;
    std::list<double> intensity;
};

double deg2rad(double deg);
double rad2deg(double rad);

void init(std::vector<Vector> &Points, std::vector<Vector> &Vectors);

void holder_to_origin(std::vector<Vector> &Points, std::vector<Vector> &Vectors);
void detector_to_origin(std::vector<Vector> &Points, std::vector<Vector> &Vectors);

std::vector<Vector> intersection(std::vector<Vector> &Points, std::vector<Vector> &Vectors);
std::vector<double> get_intensity(std::vector<double> &lambdas);
std::vector<double> get_lambda(std::vector<Vector> &Vectors);
std::vector<Vector> diffraction(std::vector<Vector> &Vectors);

void out(std::vector<double> &intensity, std::vector<double> &lambda, std::vector<Vector> &holder_points,std::vector<Vector> &detector_points);
