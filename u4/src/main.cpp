
#include <iostream>

// #include "vector.h"

template < typename T >
class Vector3D {
    private:
        T e1;
        T e2;
        T e3;

    public:
        Vector3D(T _e1, T _e2, T _e3) {
            e1 = _e1;
            e2 = _e2;
            e3 = _e3;
        }
        void print() {
            std::cout << e1 << e2 << e3 << std::endl;
        }
        friend std::ostream& operator << (std::ostream &os , const Vector3D& v ) {
            return os << "(" << v.e1 << "|" << v.e2 << "|" << v.e3 << ")";
        }

};

template<typename T>
using Point3D = Vector3D<T>;


enum Direction {REJECTED, RIGHT, LEFT};


Direction testWayPoint() {
    
    return REJECTED;
}


int main(int argc, char* argv[]) {
    Vector3D<int> v1(2, 4, 6);
    Point3D<float> v2(1.1,3.3,5.5);
    std::cout << v1 << std::endl << v2 << std::endl;

    int i = testWayPoint();
    std::cout << i << std::endl;
    
}
