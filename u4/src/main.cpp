
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
        template<typename A>
        Vector3D add(Vector3D<A> other) {

        }
        void print() {
            std::cout << e1 << e2 << e3 << std::endl;
        }
        friend std::ostream& operator << (std::ostream &os , const Vector3D& v ) {
            return os << "(" << v.e1 << "|" << v.e2 << "|" << v.e3 << ")";
        }
        // template<typename T0, typename T1, typename T2>
        // friend Vector3D<T0>& operator + (const Vector3D<T1> &v1, const Vector3D<T2> &v2) {
        //     Vector3D<T0> sum(
        //         v1.e1 + v2.e1,
        //         v1.e2 + v2.e2,
        //         v1.e3 + v2.e3
        //     );
        //     return sum;
        // };

};

template<typename T>
using Point3D = Vector3D<T>;


enum Direction {REJECTED, RIGHT, LEFT};

template<typename T1, typename T2, typename T3, typename T4>
Direction testWayPoint(Point3D<T1> dronePosition, Vector3D<T2> droneForward, Vector3D<T3> droneUp, Point3D<T4> waypoint){
    
    return REJECTED;
}


int main(int argc, char* argv[]) {
    Point3D<float> dPos(2, 4, 6);
    Vector3D<float> dForward(1.1,3.3,5.5);
    Vector3D<int> dUp(0, 1, 0);
    Point3D<int> wp(5,5,5);

    int i = testWayPoint(dPos, dForward, dUp, wp);
    // Vector3D<int> ii = dPos + dUp;

    // std::cout << v1 << std::endl << v2 << std::endl;
    std::cout << i << std::endl;
}
