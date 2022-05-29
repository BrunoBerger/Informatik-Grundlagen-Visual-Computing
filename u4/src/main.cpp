
#include <iostream>

// #include "vector.h"

template <typename T>
class Point3D {
    public:
        T e1;
        T e2;
        T e3;

    public:
        // Point3D() {}
        Point3D(T _e1, T _e2, T _e3) {
            e1 = _e1;
            e2 = _e2;
            e3 = _e3;
        }
        template<typename A>
        void print() {
            std::cout << e1 << e2 << e3 << std::endl;
        }
        friend std::ostream& operator << (std::ostream &os , const Point3D& v ) {
            return os << "(" << v.e1 << "|" << v.e2 << "|" << v.e3 << ")";
        }
        // Point3D add(Point3D<A> other) {
        //     Point3D<A> result(
        //         e1 + other.e1,
        //         e2 + other.e2,
        //         e3 + other.e3
        //     );
        //     return result;
        // }
        // template<typename T0, typename T1, typename T2>
        // friend Point3D<T0>& operator + (const Point3D<T1> &v1, const Point3D<T2> &v2) {
        //     Point3D<T0> sum(
        //         v1.e1 + v2.e1,
        //         v1.e2 + v2.e2,
        //         v1.e3 + v2.e3
        //     );
        //     return sum;
        // };

};

// template<typename T>
// using Vector3D = Point3D<T>;
template <typename T>
class Vector3D : public Point3D<T> {
     using Point3D<T>::Point3D;
};


enum Direction {REJECTED, RIGHT, LEFT};

template<typename T1, typename T2, typename T3, typename T4>
Direction testWayPoint(Point3D<T1> dronePosition, Vector3D<T2> droneForward, Vector3D<T3> droneUp, Point3D<T4> waypoint){
    return REJECTED;
}


int main(int argc, char* argv[]) {
    Point3D<float> dPos(2, 4, 6);
    Vector3D<int> dForward(1, 1, 1);
    Vector3D<int> dUp(0, 1, 0);
    Point3D<int> wp(5,5,5);

    // auto test = wp.add(dUp);
    // std::cout << test << std::endl;

    
    int i = testWayPoint(dPos, dForward, dUp, wp);
    std::cout << i << std::endl;
}
