#include "header.hpp"
#include "quaternion.hpp"
#include "vector.hpp"

#include <fstream>
// #include <iostream>

constexpr double PI = 3.14159265358979;

Vector S_point, S_vector;
Vector H_point, H_vector;
Vector D_point, D_vector;

int COLS = 100; // Columns of rays (restricted by SS)
int ROWS = 100; // Rows of rays (restricted by DS)
double LENGTH = 200.0; // [mm] Source - holder and holder - detector distances

double S_W = 0.0; // [mm] Source point shift
double S_TH = deg2rad(15.0); // [deg] Source theta angle 
double D_TH = deg2rad(15.0); // [deg] Detector theta angle
double H_Z = 0.0; // [mm] Holder vertical shift
double H_A = deg2rad(9.0); // [deg] Lattice first spherical angle rotation
double H_B = deg2rad(0.0); // [deg] Lattice second spherical angle rotation
double S_DS = deg2rad(0.5); // [deg] Source divergency slit angle
double S_SS = deg2rad(2.0); // [deg] Source Soller slit angle

double D_XMIN = -30.0;
double D_XMAX = 30.0;
int D_XNUM = 150;
double D_YMIN = -4.0;
double D_YMAX = 4.0;
int D_YNUM = 20;

LatticeV Si111
{
    {0.5773, 0.5773, 0.5773},
    3.12,
    1.0
};

Spectrum Cu30kV {};

double rad2deg(double rad)
{
    return rad * 180.0 / PI;
}

double deg2rad(double deg)
{
    return deg * PI / 180.0;
}

double mean(const std::vector<double> &vec)
{
    if(vec.size() == 0)
        return 0.0;

    double res = 0.0;
    for(auto &a : vec)
        res += a;

    return res / (double)vec.size();
}

void read_setup()
{
    std::ifstream file("setup.dat", std::ios::in | std::ios::binary);
    file.read((char*)&COLS, sizeof(COLS));
    file.read((char*)&ROWS, sizeof(ROWS));
    file.read((char*)&LENGTH, sizeof(LENGTH));
    file.read((char*)&S_W, sizeof(S_W));
    file.read((char*)&S_TH, sizeof(S_TH));
    file.read((char*)&D_TH, sizeof(D_TH));
    file.read((char*)&H_Z, sizeof(H_Z));
    file.read((char*)&H_A, sizeof(H_A));
    file.read((char*)&H_B, sizeof(H_B));
    file.read((char*)&S_DS, sizeof(S_DS));
    file.read((char*)&S_SS, sizeof(S_SS));
    file.read((char*)&D_XMIN, sizeof(D_XMIN));
    file.read((char*)&D_XMAX, sizeof(D_XMAX));
    file.read((char*)&D_XNUM, sizeof(D_XNUM));
    file.read((char*)&D_YMIN, sizeof(D_YMIN));
    file.read((char*)&D_YMAX, sizeof(D_YMAX));
    file.read((char*)&D_YNUM, sizeof(D_YNUM));

    file.close();
}

void read_lattice()
{
    std::ifstream file("lattice.dat", std::ios::in | std::ios::binary);

    double temp[3];
    file.read((char*)&temp, sizeof(temp));
    Si111.s = Vector(temp);
    
    Vector v(sin(H_A) * cos(H_B), sin(H_A) * sin(H_B), cos(H_A));
    Quaternion q = rotation(Si111.s, v);
    Si111.s = q.rotate(Si111.s);
    // std::cout << Si111.s[0] << " " << Si111.s[1] << " " << Si111.s[2] << std::endl;
    
    file.read((char*)&Si111.d, sizeof(Si111.d));
    file.read((char*)&Si111.SA, sizeof(Si111.SA));
    file.close();
}

void read_spectrum()
{
    std::ifstream file("spectrum.dat", std::ios::in | std::ios::binary);
    int size;
    file.read((char*)&size, sizeof(size));
    // std::cout << size << std::endl;
    Cu30kV.lambda = std::list<double>(size);
    for(auto &temp : Cu30kV.lambda)
        file.read((char*)&temp, sizeof(double));
    Cu30kV.intensity = std::list<double>(size);
    for(auto &temp : Cu30kV.intensity)
        file.read((char*)&temp, sizeof(double));
    std::vector<double> temp(1);
    // temp[0] = 1.54;
    // std::cout << "lam_min " << Cu30kV.lambda[0] << " lam_max " << Cu30kV.lambda[3630 - 1] << std::endl;
    // std::cout << "int_min " << Cu30kV.intensity[0] << " int_max " << Cu30kV.intensity[3630 - 1] << std::endl;
    // std::cout << "1.54 intensity: " << get_intensity(temp)[0] << std::endl;
    file.close();
}

void init(std::vector<Vector> &Points, std::vector<Vector> &Vectors)
{
    // Input file reading
    read_setup();
    read_lattice();
    read_spectrum();

    // Key points setup and initialising
    S_point = Vector(-LENGTH*cos(S_TH), 0.0, LENGTH*sin(S_TH));
    H_point = Vector(0.0, 0.0, H_Z);
    D_point = Vector(LENGTH*cos(D_TH), 0.0, LENGTH*sin(D_TH));

    S_vector = H_point - S_point;
    S_vector /= S_vector.len();
    H_vector = Vector(0.0, 0.0, 1.0);
    D_vector = H_point - D_point;
    D_vector /= D_vector.len();
    
    Points = std::vector<Vector>(COLS * ROWS);
    Vectors = std::vector<Vector>(COLS * ROWS);

    // Initial ray point
    S_point.y() += S_W;
    for(auto &P : Points)
        P = Vector(S_point);

    // std::cout << "COLS * ROWS: " << COLS * ROWS << std::endl;
    // std::cout << "D_XMAX: " << D_XMAX << std::endl;

    // Idea of generating ray vectors is to make rays distributed evenly all over the lighted area of the holder
    double Xmin = LENGTH * (sin(S_TH) / tan(S_TH + S_DS * 0.5) - cos(S_TH));
    double Xmax = LENGTH * (sin(S_TH) / tan(S_TH - S_DS * 0.5) - cos(S_TH));
    double Ymin = LENGTH * tan(S_SS * 0.5);
    double Ymax = -Ymin;
   
    double xstep = (Xmax - Xmin) / (double)(ROWS + 1);
    double ystep = (Ymax - Ymin) / (double)(COLS + 1);

    auto it = Vectors.begin();
    double x = Xmin + xstep;
    // std::cout << "Xmin, xstep, Xmax: " << Xmin << ", " << xstep << ", " << Xmax << std::endl;
    double y = Ymin + ystep;
    Vector temp;
    for(int i = 0; i < ROWS; ++i, x += xstep)
        for(int j = 0; j < COLS; ++j, y += ystep)
        {
            temp = Vector(x, y, 0.0);
            (*it) = temp - S_point;
            (*it) /= (*it).len();
            ++it;
        }
}

void holder_to_origin(std::vector<Vector> &Points, std::vector<Vector> &Vectors)
{

    for(auto &P : Points)
        P -= H_point;

    S_point -= H_point;
    D_point -= H_point;

    H_point = Vector();

    if(H_vector != Vector(0.0, 0.0, 1.0))
    {
        Quaternion q = rotation(H_vector, Vector(0.0, 0.0, 1.0));
        for(auto &P : Points)
            P = q.rotate(P);
        for(auto &V : Vectors)
            V = q.rotate(V);
        S_point = q.rotate(S_point);
        S_vector = q.rotate(S_vector);
        D_point = q.rotate(D_point);
        D_vector = q.rotate(D_vector);

        H_vector = Vector(0.0, 0.0, 1.0);
    }
}

void detector_to_origin(std::vector<Vector> &Points, std::vector<Vector> &Vectors)
{
    for(auto &P : Points)
        P -= D_point;

    S_point -= D_point;
    H_point -= D_point;

    D_point = Vector();

    Quaternion q = rotation(D_vector, Vector(0.0, 0.0, 1.0));
    for(auto &P : Points)
        P = q.rotate(P);
    for(auto &V : Vectors)
        V = q.rotate(V);
    S_point = q.rotate(S_point);
    S_vector = q.rotate(S_vector);
    H_point = q.rotate(H_point);
    H_vector = q.rotate(H_vector);

    D_vector = Vector(0.0, 0.0, 1.0);
}

std::vector<Vector> intersection(std::vector<Vector> &Points, std::vector<Vector> &Vectors)
{
    std::vector<Vector> result(Points);
    auto itP = Points.begin();
    auto itV = Vectors.begin();
    for(auto &R : result)
    {
        R -= (*itV) * (*(itP)).z() / (*(itV)).z();
        ++itV;
        ++itP;
    }

    return result;
}

std::vector<Vector> diffraction(std::vector<Vector> &Vectors)
{
    std::vector<Vector> result(Vectors);
    auto itV  = Vectors.begin();
    for(auto &V : result)
        V -= 2.0 * Si111.s * dot(*(itV++), Si111.s);

    return result;
}

std::vector<double> get_lambda(std::vector<Vector> &Vectors)
{
    std::vector<double> result(COLS * ROWS);
    auto itV  = Vectors.begin();
    for(auto &I : result)
    {
        I = - 2.0 * Si111.d * dot(*(itV++), Si111.s);
    }

    return result;
}
std::vector<double> get_intensity(std::vector<double> &lambdas)
{
    std::vector<double> result(COLS * ROWS);
    auto plambda  = lambdas.begin();
    double lambda;
    auto ptr_lam = Cu30kV.lambda.begin();
    auto ptr_int = Cu30kV.intensity.begin();
    for(auto &I : result)
    {
        lambda = *(plambda++);
       
        if(lambda < *(Cu30kV.lambda.begin()) || lambda > *(Cu30kV.lambda.rbegin()))
        {
            I = 0.0;
            continue;
        }

        ptr_lam = Cu30kV.lambda.begin();
        ptr_int = Cu30kV.intensity.begin();
        for(; ptr_lam != Cu30kV.lambda.end() && (*ptr_lam) < lambda; ++ptr_lam, ++ptr_int)
            ;
        
        I = *ptr_int;
    }

    return result;
}

void out(std::vector<double> &intensity, std::vector<double> &lambda, std::vector<Vector> &holder_points, std::vector<Vector> &detector_points)
{
    std::vector<int> n_to_i(COLS * ROWS), n_to_j(COLS * ROWS);
    
    double D_XSTEP = (D_XMAX - D_XMIN) / (double)D_XNUM;
    double D_YSTEP = (D_YMAX - D_YMIN) / (double)D_YNUM;

    auto pD = detector_points.begin();
    auto pI = n_to_i.begin();
    auto pJ = n_to_j.begin();
    for(; pD != detector_points.end(); ++pD)
    {
        if(pD->x() < D_XMIN)
            (*pI) = -1;
        else if(pD->x() >= D_XMAX)
            (*pI) = -1;
        else
            (*pI) = (int)((pD->x() - D_XMIN) / D_XSTEP);
        ++pI;

        if(pD->y() < D_YMIN)
            (*pJ) = -1;
        else if(pD->y() >= D_YMAX)
            (*pJ) = -1;
        else
            (*pJ) = (int)((pD->y() - D_YMIN) / D_YSTEP);
        ++pJ;
    }

    std::vector<std::vector<double>> intensity_arr(D_XNUM * D_YNUM);
    std::vector<std::vector<double>> lambda_arr(D_XNUM * D_YNUM); 
    std::vector<std::vector<double>> holderx_arr(D_XNUM * D_YNUM); 
    std::vector<std::vector<double>> holdery_arr(D_XNUM * D_YNUM); 

    pI = n_to_i.begin();
    pJ = n_to_j.begin();
    auto pInt = intensity.begin();
    auto pLam = lambda.begin();
    auto pHol = holder_points.begin();
    for(int idx; pI != n_to_i.end(); ++pI, ++pJ)
    {
        if((*pI) != -1 && (*pJ) != -1)
        {
            idx = (*pI) * D_YNUM + (*pJ);
            intensity_arr[idx].push_back(*pInt);
            lambda_arr[idx].push_back(*pLam);
            holderx_arr[idx].push_back((*pHol).x());
            holdery_arr[idx].push_back((*pHol).y());
        }
    }

    std::ofstream file("out.dat", std::ios::out | std::ios::binary);
    double res;
    for(auto &temp : intensity_arr)
    {
        res = mean(temp);
        file.write((char*)&res, sizeof(double));
    }
    for(auto &temp : lambda_arr)
    {
        res = mean(temp);
        file.write((char*)&res, sizeof(double));
    }
    for(auto &temp : holderx_arr)
    {
        res = mean(temp);
        file.write((char*)&res, sizeof(double));
    }
    for(auto &temp : holdery_arr)
    {
        res = mean(temp);
        file.write((char*)&res, sizeof(double));
    }
    file.close();
}
