
#include <iostream>
#include <math.h>

template <typename T>
class Point3D {
    public:
        T e1;
        T e2;
        T e3;
    public:
        Point3D(T _e1, T _e2, T _e3) {
            e1 = _e1;
            e2 = _e2;
            e3 = _e3;
        }
        auto distance(Point3D other) {
            auto x = other.e1 - e1;
            auto y = other.e2 - e2;
            auto z = other.e3 - e3;
            return sqrt(x*x + y*y + z*z);
        }
        auto dotProduct(Point3D other) {
            return e1*other.e1 + e2*other.e2 + e3*other.e3;
        }


        friend std::ostream& operator << (std::ostream &os , const Point3D& v ) {
            return os << "(" << v.e1 << "|" << v.e2 << "|" << v.e3 << ")";
        }
};


template <typename T>
class Vector3D : public Point3D<T> {
     using Point3D<T>::Point3D;
};


enum Direction {REJECTED, RIGHT, LEFT};

// either add converstion or give pos and wp the same type
template<typename T1, typename T2, typename T3, typename T4>
Direction testWayPoint(Point3D<T1> dronePosition, Vector3D<T2> droneForward, Vector3D<T3> droneUp, Point3D<T4> waypoint){
    if (dronePosition.distance(waypoint) >= 10)
        return REJECTED;
    
    return RIGHT;
}

int main() {
    Point3D<float> dPos(0, 0, 0);
    Vector3D<int> dForward(1, 1, 1);
    Vector3D<int> dUp(0, 1, 0);
    Point3D<float> wp(4, 0, 0);
    std::cout << dPos << dForward << dUp << wp << std::endl;

   
    int x = testWayPoint(dPos, dForward, dUp, wp);
    std::cout << x << std::endl;
}
