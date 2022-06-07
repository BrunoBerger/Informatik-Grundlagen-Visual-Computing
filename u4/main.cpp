
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
        Point3D cross(Point3D other) {
            return Point3D(
                e2*other.e3 - e3*other.e2,
                e3*other.e1 - e1*other.e3,
                e1*other.e2 - e2*other.e1
            );
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

template<typename T>
Direction testWayPoint(Point3D<T> dronePosition, Vector3D<T> droneForward, Vector3D<T> droneUp, Point3D<T> waypoint){
    float distance = dronePosition.distance(waypoint);
    float angle = droneForward.angle(waypoint);
    Point3D<float> crossProd = droneForward.cross(waypoint);

    // std::cout << "distance: " << distance << std::endl;
    // std::cout << "angle: " << angle << std::endl;
    // std::cout << "CrossProd: " << crossProd << std::endl;

    if (distance >= 10)
        return REJECTED;
    if (abs(angle) > 60)
        return REJECTED;
    if (crossProd.e1 > 0)
        return RIGHT;
    else 
        return LEFT;    
}


int main() {
    // Testing testWayPoint function
    Point3D<float> dPos(0, 0, 0);
    Vector3D<float> dForward(0, 0, 3);
    Vector3D<float> dUp(0, 1, 0);
    std::cout << "Waypoint Test:\nDrone: "<< dPos << dForward << dUp << std::endl;
   
    Point3D<float> wp1(3, 4, 4);
    int res1 = testWayPoint(dPos, dForward, dUp, wp1);
    std::cout << "Waypoint: "<< wp1 << " Acceptance: " << res1 << std::endl;

    Point3D<float> wp2(0,-2, 2);
    int res2 = testWayPoint(dPos, dForward, dUp, wp2);
    std::cout << "Waypoint: "<< wp2 << " Acceptance: " << res2 << std::endl;

    Point3D<float> wp3(0, 1, 14);
    int res3 = testWayPoint(dPos, dForward, dUp, wp3);
    std::cout << "Waypoint: "<< wp3 << " Acceptance: " << res3 << std::endl;

    Point3D<float> wp4(0, 4, 1);
    int res4 = testWayPoint(dPos, dForward, dUp, wp4);
    std::cout << "Waypoint: "<< wp4 << " Acceptance: " << res4 << std::endl;

    std::cout << "\nMatrix Multiplication Test:" << std::endl;
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

    // Vector3D<float> test_crossProd = dPos.cross(dUp); // How to return child-class ?
}
