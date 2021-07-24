#include "./../include/func.hpp"
#include "./../include/quaternion.hpp"
#include "./../include/vector.hpp"

#include <fstream>
#include <iostream>

struct Sizes
{
    int COLS;
    int ROWS;
    int LVSIZE;
};

struct ForIn
{
    double LENGTH;
    double S_W;
    double S_TH;
    double D_TH;
    double H_Z;
    double H_A;
    double H_B;
    double S_DS;
    double S_SS;
};

Sizes SIZES;

ForIn INPUT;

Ray Source;
Ray Holder;
Ray Detector;

std::list<LatticeV> Lattice;

Spectrum Cu30kV;


void read_setup(const char* filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    file.read((char*)&SIZES, sizeof(SIZES));
    file.read((char*)&INPUT, sizeof(INPUT));

    file.close();
    
}

void read_lattice(const char* filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    int size;
    file.read((char*)&size, sizeof(int));
    
    if(size < SIZES.LVSIZE)
        SIZES.LVSIZE = size;

    Lattice = std::list<LatticeV>(SIZES.LVSIZE);

    double buffer[3];
    Vector first_vector_direction(sin(INPUT.H_A) * cos(INPUT.H_B), sin(INPUT.H_A) * sin(INPUT.H_B), cos(INPUT.H_A));

    file.read((char*)buffer, 3 * sizeof(double));
    Lattice.front().s = Vector(buffer);
    
    Quaternion q = rotation(Lattice.front().s, first_vector_direction);
    q.rotate(Lattice.front().s);

    file.read((char*)&(Lattice.front().d), sizeof(double));
    file.read((char*)&(Lattice.front().A), sizeof(double));

    for(auto pLV = (++Lattice.begin()); pLV != Lattice.end(); ++pLV)
    {
        file.read((char*)&buffer, sizeof(buffer));
        pLV->s = Vector(buffer);
        q.rotate(pLV->s);
        
        file.read((char*)&(pLV->d), sizeof(double));
        file.read((char*)&(pLV->A), sizeof(double));
    }

    file.close();
}

void read_spectrum(const char* filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    int size;
    file.read((char*)&size, sizeof(size));

    Cu30kV.l = std::list<double>(size);
    for(auto &temp : Cu30kV.l)
        file.read((char*)&temp, sizeof(double));

    Cu30kV.I = std::list<double>(size);
    for(auto &temp : Cu30kV.I)
        file.read((char*)&temp, sizeof(double));
    
    file.close();
}

void init(std::vector<Ray> &Rays)
{
    // Key points setup and initialising
    Source.p = Vector(-INPUT.LENGTH*cos(INPUT.S_TH), 0.0, INPUT.LENGTH*sin(INPUT.S_TH));
    Holder.p = Vector(0.0, 0.0, INPUT.H_Z);
    Detector.p = Vector(INPUT.LENGTH*cos(INPUT.D_TH), 0.0, INPUT.LENGTH*sin(INPUT.D_TH));

    Source.v = Holder.p - Source.p;
    Source.v /= Source.v.len();

    Holder.v = Vector(0.0, 0.0, 1.0);
    
    Detector.v = Holder.p - Detector.p;
    Detector.v /= Detector.v.len();
    
    // Idea of generating ray vectors is to make rays distributed evenly all over the lighted area of the holder
    double Xmin = INPUT.LENGTH * (sin(INPUT.S_TH) / tan(INPUT.S_TH + INPUT.S_DS * 0.5) - cos(INPUT.S_TH));
    double Xmax = INPUT.LENGTH * (sin(INPUT.S_TH) / tan(INPUT.S_TH - INPUT.S_DS * 0.5) - cos(INPUT.S_TH));

    double Ymin = -INPUT.LENGTH * tan(INPUT.S_SS * 0.5);
    double Ymax = INPUT.LENGTH * tan(INPUT.S_SS * 0.5);
   
    double xstep = (Xmax - Xmin) / (double)(SIZES.ROWS + 1);
    double ystep = (Ymax - Ymin) / (double)(SIZES.COLS + 1);

    // Finally Rays init
    Rays = std::vector<Ray>(SIZES.COLS * SIZES.ROWS);
    auto it = Rays.begin();
    Vector temp;
    Vector tempSource(Source.p);
    tempSource.y() += INPUT.S_W;
    for(double x = Xmin + xstep; x < Xmax; x += xstep)
        for(double y = Ymin + ystep; y < Ymax; y += ystep)
        {
            (*it).p = tempSource;

            temp = Vector(x, y, 0.0);
            (*it).v = temp - Source.p;
            (*it).v /= (*it).v.len();

            (*it).l = 0.0;
            (*it).I = 1.0;

            ++it;
        }
}

void to_origin(Ray origin_ray, std::vector<Ray> &Rays)
{
    Vector zero(0.0, 0.0, 0.0);
    Quaternion identity(1.0, 0.0, 0.0, 0.0);
    Quaternion q = rotation(origin_ray.v, Vector(0.0, 0.0, 1.0));

    if(origin_ray.p != zero)
    {
        Source.p -= origin_ray.p;
        Holder.p -= origin_ray.p;
        Detector.p -= origin_ray.p;
        for(auto &ray : Rays)
            ray.p -= origin_ray.p;
    }
    
    if(q != identity)
    {
        q.rotate(Source.p);
        q.rotate(Source.v);
        q.rotate(Holder.p);
        q.rotate(Holder.v);
        q.rotate(Detector.p);
        q.rotate(Detector.v);
        for(auto &ray : Rays)
        {
            q.rotate(ray.p);
            q.rotate(ray.v);
        }    
    }
}

void intersection(std::vector<Ray> &Rays)
{
    for(auto &temp : Rays)
        temp.p -= temp.v * temp.p.z() / temp.v.z();
}
void diffraction(std::vector<Ray> &Rays)
{
    std::vector<Ray> result(SIZES.COLS * SIZES.ROWS * SIZES.LVSIZE);
    auto itR  = result.begin();
    for(auto &ray : Rays)
        for(auto &LV : Lattice)
        {
            itR->l = - 2.0 * LV.d * dot(ray.v, LV.s);
            
            if(itR->l < Cu30kV.l.front() || itR->l > Cu30kV.l.back())
                itR->I = 0.0;
            else
                for(auto pL = Cu30kV.l.begin(), pI = Cu30kV.I.begin();; ++pL, ++pI)
                    if((*pL) >= itR->l)
                    {
                        itR->I = (*pI) * LV.A;
                        break;
                    }

            itR->v = ray.v - 2.0 * LV.s * dot(ray.v, LV.s);
            itR->p = ray.p;
            
            ++itR;
        }

    Rays.resize(result.size());
    Rays.assign(result.begin(), result.end());
}
void out(const char* filename, std::vector<Ray> &Rays)
{
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    for(auto &ray : Rays)
    {
        file.write((char*)&(ray.I), sizeof(double));
        file.write((char*)&(ray.l), sizeof(double));
        file.write(ray.p.get_data_pointer(), 2 * sizeof(double));
    }

    file.close();
}
