#include <iostream>
#include <fstream>

int main()
{
    double kek = 15.25;
    std::ofstream file("test.dat", std::ios::out | std::ios::binary);
    file.write((char*)&kek, sizeof(double));
    return 0;
}
