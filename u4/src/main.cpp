
#include <array>
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
        float distance(Point3D<T> other) {
            auto x = other.e1 - e1;
            auto y = other.e2 - e2;
            auto z = other.e3 - e3;
            return sqrt(x*x + y*y + z*z);
        }
        float magnitude() {
            Point3D<T> nullVec(0,0,0);
            return this->distance(nullVec);
        }
        float dotProduct(Point3D other) {
            return e1*other.e1 + e2*other.e2 + e3*other.e3;
        }
        float angle(Point3D other) {
            auto temp = this->dotProduct(other) / (this->magnitude() * other.magnitude());
            return acos(temp) * 180 / M_PI;
        }

        friend std::ostream& operator << (std::ostream &os , const Point3D& v ) {
            return os << "(" << v.e1 << "|" << v.e2 << "|" << v.e3 << ")";
        }
};

template <typename T>
class Vector3D : public Point3D<T> {
    using Point3D<T>::Point3D;

    public:
        std::array<T, 4> multiplyWithMatr(float mat[4][4]) {
            std::array<T, 4> res;
            for (int i=0; i<4; i++) {
                res[i] = 
                    mat[i][0] * this->e1 +
                    mat[i][1] * this->e2 + 
                    mat[i][2] * this->e3 +
                    mat[i][3] * 1;
            }
            return res;
        };
};



enum Direction {REJECTED, RIGHT, LEFT};

template<typename T1, typename T2>
Direction testWayPoint(Point3D<T1> dronePosition, Vector3D<T1> droneForward, Vector3D<T2> droneUp, Point3D<T1> waypoint){
    float distance = dronePosition.distance(waypoint);
    float angle = droneForward.angle(waypoint);

    std::cout << "distance: " << distance << std::endl;
    std::cout << "angle: " << angle << std::endl;

    if (distance >= 10)
        return REJECTED;
    if (abs(angle) > 60)
        return REJECTED;

    return RIGHT;
}


int main() {
    // Testing testWayPoint function
    Point3D<float> dPos(0, 0, 0);
    Vector3D<float> dForward(0, 1, 0);
    Vector3D<int> dUp(0, 1, 0);
    Point3D<float> wp(0, 3, 4);
    std::cout << "Input vecs: "<< dPos << dForward << dUp << wp << std::endl;
   
    int x = testWayPoint(dPos, dForward, dUp, wp);
    std::cout << "Waypoint acceptance: " << x << std::endl;


    // Testing matrix multiplication
    float mat[4][4];
    mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3; mat[0][3] = 4;
    mat[1][0] = 1; mat[1][1] = 2; mat[1][2] = 1; mat[1][3] = 1;
    mat[2][0] = 1; mat[2][1] = 1; mat[2][2] = 2; mat[2][3] = 1;
    mat[3][0] = 1; mat[3][1] = 1; mat[3][2] = 1; mat[3][3] = 2;

    Vector3D<int> matTestVec(2, 3, 4);
    auto res = matTestVec.multiplyWithMatr(mat);
    for (auto i : res) {
        std::cout << i << std::endl;
    }
    // For comparison: 
    // https://matrixcalc.org/de/#%7B%7B1,2,3,4%7D,%7B1,2,1,1%7D,%7B1,1,2,1%7D,%7B1,1,1,2%7D%7D*%7B%7B2%7D,%7B3%7D,%7B4%7D,%7B1%7D%7D
}
