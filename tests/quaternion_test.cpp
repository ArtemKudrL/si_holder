#include "quaternion.hpp"
#include <iostream>
#include <ostream>


int main()
{
    Quaternion q1(0, 1, 2, 3);
    std::cout << "initialized 0123: " << q1[0] << " " << q1[1] << " " << q1[2] << " " << q1[3] << std::endl;

    Quaternion p1(0, -1, -2, -3);
    if(conj(q1) == p1)
        std::cout << "conjugated 0123 ok" << std::endl;
    else
    {
        std::cout << "0123 conj != 0-1-2-3???" << std::endl;
        return -1;
    }

    Vector v1(1, 0, 0), v2(1, 1, 0), v3(0, 0, 1);
    Quaternion q2 = rotation(v1, v2);
    
    std::cout << "rotating quaternion from 100 to 110 is: " << std::endl <<
        q2[0] << " " << q2[1] << " " << q2[2] << " " << q2[3] << std::endl;

    q2.rotate(v1);
    q2.rotate(v2);
    q2.rotate(v3);

    std::cout << "rotated vectors" << std::endl;
    std::cout << v1[0] << " " << v1[1] << " " << v1[2] << std::endl;  
    std::cout << v2[0] << " " << v2[1] << " " << v2[2] << std::endl;  
    std::cout << v3[0] << " " << v3[1] << " " << v3[2] << std::endl;  
    std::cout << "these vectors should be:" << std::endl <<
        "0.707 0.707 0.0" << std::endl <<
        "0.0   1.414 0.0" << std::endl <<
        "0.0   0.0   1.0" << std::endl;

    std::cout << "if there's no errors, you can use quaternions :)" << std::endl;

    return 0;
}
