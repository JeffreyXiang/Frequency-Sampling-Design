#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

using namespace std;

//二维向量，比较易懂，就不注释了
class Vector
{
    public:
        double x;
        double y;

        Vector() : x(0), y(0) {}
        Vector(double x, double y) : x(x), y(y) {}
        Vector(const Vector& V) : x(V.x), y(V.y) {}
        ~Vector(){}

        Vector& operator=(const Vector& V);
        Vector operator-();
        Vector operator+(const Vector& V);
        Vector operator-(const Vector& V);
        double operator*(const Vector& V);
        Vector operator*(double k);
        double module();
        Vector unitVector();
        Vector normalVector();
};

#endif