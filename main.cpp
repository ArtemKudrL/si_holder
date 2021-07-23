#include "header.hpp"
#include <iostream>

int main()
{ 
    std::vector<Vector> source_points;
    std::vector<Vector> directions;
    std::cout << "begin ok" << std::endl;

    init(source_points, directions);
    std::cout << "init ok" << std::endl;
    // std::cout << "source_points: " <<  source_points[0][0] << " " <<  source_points[0][1] << " " <<  source_points[0][2] << " " << std::endl;
    // std::cout << "directions: " <<  directions[0][0] << " " <<  directions[0][1] << " " << directions[0][2] << " " << std::endl;

    std::vector<Vector> holder_points(source_points);
    holder_to_origin(holder_points, directions);
    std::cout << "HTO ok" << std::endl;
    holder_points = intersection(holder_points, directions);
    // std::cout << "holder_points: " <<  holder_points[0][0] << " " <<  holder_points[0][1] << " " <<  holder_points[0][2] << " " << std::endl;
    std::cout << "IH ok" << std::endl;

    std::vector<double> lambda(directions.size());
    std::vector<double> intensity(directions.size());
    lambda = get_lambda(directions);
    // std::cout << "lambda: " << lambda[0] << std::endl;
    std::cout << "L ok" << std::endl;
    intensity = get_intensity(lambda);
    // std::cout << "intensity: " << intensity[0] << std::endl;
    std::cout << "I ok" << std::endl;
    directions = diffraction(directions);
    // std::cout << "directions: " <<  directions[0][0] << " " <<  directions[0][1] << " " << directions[0][2] << " " << std::endl;
    std::cout << "D ok" << std::endl;

    std::vector<Vector> detector_points(holder_points);
    detector_to_origin(detector_points, directions);
    std::cout << "DTO ok" << std::endl;
    detector_points = intersection(detector_points, directions);
    // std::cout << "detector_points: " <<  detector_points[0][0] << " " <<  detector_points[0][1] << " " <<  detector_points[0][2] << " " << std::endl;
    std::cout << "ID ok" << std::endl;

    out(intensity, lambda, holder_points, detector_points);
    std::cout << "out ok" << std::endl;

    return 0;
}
