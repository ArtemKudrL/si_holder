#include "vector.hpp"
#include <iostream>
#include <ostream>

int main()
{
    Vector a;
    a = Vector(1, 2, 3);
    std::cout << "assign 123: "<< a[0] << " " << a[1] << " " << a[2] << std::endl; 
    Vector b(a), e;
    Vector c(2, 4, 6), d;
    
    if(a == b) 
        std::cout << "123 == 123" << std::endl;
    else
    {
        std::cout << "123 != 123???" << std::endl;
        return -1;
    }


    if((a + b) == c)
        std::cout << "123 + 123 == 246" << std::endl;
    else
    {
        std::cout << "123 + 123 != 246???" << std::endl;
        return -1;
    }

    if((a-=b) == d)
        std::cout << "123 - 123 == 000" << std::endl;
    else
    {
        std::cout << "123 - 123 != 000???" << std::endl;    
        return -1;
    }

    if((b.dot(b)) == (1*1+2*2+3*3))
        std::cout << "(123)^2 == 1^2+2^2+3^2" << std::endl;
    else
    {
        std::cout << "(123)^2 != 1^2+2^2+3^2???" << std::endl;
        return -1;
    }

    std::cout << "test completed!" << std::endl;

    return 0;
}
