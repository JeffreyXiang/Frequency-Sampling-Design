#include "Vector.h"

Vector& Vector::operator=(const Vector& V)
{
    x = V.x;
    y = V.y;
    return *this;
}

Vector Vector::operator-()
{
    Vector R(-x, -y);
    return R;
}

Vector Vector::operator+(const Vector& V)
{
    Vector R;
    R.x = x + V.x;
    R.y = y + V.y;
    return R;
}

Vector Vector::operator-(const Vector& V)
{
    Vector R;
    R.x = x - V.x;
    R.y = y - V.y;
    return R;
}

double Vector::operator*(const Vector& V)
{
    return x * V.x + y * V.y;
}

Vector Vector::operator*(double k)
{
    Vector R(x * k, y * k);
    return R;
}

double Vector::module()
{
    return sqrt(x * x + y * y);
}

Vector Vector::unitVector()
{
    return *this * (1.0 / module());
}

Vector Vector::normalVector()
{
    Vector R(-y, x);
    return R.unitVector();
}
