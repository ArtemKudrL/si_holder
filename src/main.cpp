#include "./../include/func.hpp"

int main()
{ 
    read_setup("./../data/in/setup.dat");
    read_lattice("./../data/in/lattice.dat");
    read_spectrum("./../data/in/spectrum.dat");

    std::vector<Ray> Rays;

    init(Rays);

    to_origin(Holder, Rays);

    intersection(Rays);

    diffraction(Rays);

    out("./../data/out/h_out.dat", Rays);

    to_origin(Detector, Rays);

    intersection(Rays);

    out("./../data/out/d_out.dat", Rays);

    return 0;
}
