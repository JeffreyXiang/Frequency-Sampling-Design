#include "Figure.h"

double Figure::segmentSDF(Vector& E1, Vector& E2, Vector& P)
{
    Vector V1 = E2 - E1;
    Vector V2 = P - E1;
    if (V1 * V2 <= 0)
        return V2.module();
    else
    {
        Vector V3 = P - E2;
        if (V1 * V3 >= 0)
            return V3.module();
        else
            return fabs(V1.normalVector() * V2);
    }
}

double Figure::tSDF(Vector pos)
{
    double res = SDF(pos);
    res -= attribute.roundedRadius;
    if (attribute.annularRadius >= 0)
        res = fabs(res) - attribute.annularRadius;
    return res;
}

Figure::AABBdata Figure::tAABB()
{
    AABBdata res = AABB();
    res.xMin -= attribute.roundedRadius + (attribute.annularRadius >= 0 ? attribute.annularRadius : 0);
    res.yMin -= attribute.roundedRadius + (attribute.annularRadius >= 0 ? attribute.annularRadius : 0);
    res.xMax += attribute.roundedRadius + (attribute.annularRadius >= 0 ? attribute.annularRadius : 0);
    res.yMax += attribute.roundedRadius + (attribute.annularRadius >= 0 ? attribute.annularRadius : 0);
    return res;
}

double Circle::SDF(Vector pos)
{
    return (pos - center).module() - radius;
}

Figure::AABBdata Circle::AABB()
{
    AABBdata res;
    res.xMin = center.x - radius;
    res.yMin = center.y - radius;
    res.xMax = center.x + radius;
    res.yMax = center.y + radius;
    return res;
}

double Capsule::SDF(Vector pos)
{
    Vector V1 = endpoint2 - endpoint1;
    Vector V2 = pos - endpoint1;
    if (V1 * V2 <= 0)
        return V2.module() - radius;
    else
    {
        Vector V3 = pos - endpoint2;
        if (V1 * V3 >= 0)
            return V3.module() - radius;
        else
            return fabs(V1.normalVector() * V2) - radius;
    }
}

Figure::AABBdata Capsule::AABB()
{
    AABBdata res;
    res.xMin = min(endpoint1.x, endpoint2.x) - radius;
    res.yMin = min(endpoint1.y, endpoint2.y) - radius;
    res.xMax = max(endpoint1.x, endpoint2.x) + radius;
    res.yMax = max(endpoint1.y, endpoint2.y) + radius;
    return res;
}

double UnevenCapsule::SDF(Vector pos)
{
    double dx = endpoint2.x - endpoint1.x;
    double dy = endpoint2.y - endpoint1.y;
    double h = sqrt(dx * dx + dy * dy);
    double cos_ = dx / h;
    double sin_ = -dy / h;
    Vector E1 = { 0, 0 };
    Vector E2 = { h, 0 };
    double a = (radius1 - radius2) / h;
    double b = sqrt(1 - a * a);
    Vector L = { b, -a };
    Vector N = { a, b };
    pos = pos - endpoint1;
    Vector pos_ = { cos_ * pos.x - sin_ * pos.y, fabs(sin_ * pos.x + cos_ * pos.y) };
    double jdg = (pos_ - E1) * L;
    if (jdg <= 0)
        return (pos_ - E1).module() - radius1;
    else if (jdg >= b * h)
        return (pos_ - E2).module() - radius2;
    else
        return (pos_ - E1) * N - radius1;
}

Figure::AABBdata UnevenCapsule::AABB()
{
    AABBdata res;
    res.xMin = min(endpoint1.x - radius1, endpoint2.x - radius2);
    res.yMin = min(endpoint1.y - radius1, endpoint2.y - radius2);
    res.xMax = max(endpoint1.x + radius1, endpoint2.x + radius2);
    res.yMax = max(endpoint1.y + radius1, endpoint2.y + radius2);
    return res;
}

double Rectangle::SDF(Vector pos)
{
    double cos_ = cos(theta);
    double sin_ = sin(theta);
    pos = pos - center;
    Vector pos_ = { fabs(cos_ * pos.x + sin_ * pos.y), fabs(-sin_ * pos.x + cos_ * pos.y) };
    if (pos_.x > size.x)
    {
        if (pos_.y > size.y)
            return (pos_ - size).module();
        else
            return pos_.x - size.x;
    }
    else
    {
        if (pos_.y > size.y)
            return pos_.y - size.y;
        else
            return max(pos_.y - size.y, pos_.x - size.x);
    }
}

Figure::AABBdata Rectangle::AABB()
{
    AABBdata res;
    double cos_ = cos(theta);
    double sin_ = sin(theta);
    double dx = max(abs(size.x * cos_ - size.y * sin_), abs(size.x * cos_ + size.y * sin_));
    double dy = max(abs(size.x * sin_ + size.y * cos_), abs(size.x * sin_ - size.y * cos_));
    res.xMin = center.x - dx;
    res.yMin = center.y - dy;
    res.xMax = center.x + dx;
    res.yMax = center.y + dy;
    return res;
}

double Triangle::SDF(Vector pos)
{
    double d1 = segmentSDF(vertex1, vertex2, pos);
    double d2 = segmentSDF(vertex2, vertex3, pos);
    double d3 = segmentSDF(vertex3, vertex1, pos);
    double d = min(min(d1, d2), d3);
    if ((((vertex2.x - vertex1.x) * (pos.y - vertex1.y) - (vertex2.y - vertex1.y) * (pos.x - vertex1.x) > 0) &&
        ((vertex3.x - vertex2.x) * (pos.y - vertex2.y) - (vertex3.y - vertex2.y) * (pos.x - vertex2.x) > 0) &&
        ((vertex1.x - vertex3.x) * (pos.y - vertex3.y) - (vertex1.y - vertex3.y) * (pos.x - vertex3.x) > 0)) ||
        (((vertex2.x - vertex1.x) * (pos.y - vertex1.y) - (vertex2.y - vertex1.y) * (pos.x - vertex1.x) < 0) &&
            ((vertex3.x - vertex2.x) * (pos.y - vertex2.y) - (vertex3.y - vertex2.y) * (pos.x - vertex2.x) < 0) &&
            ((vertex1.x - vertex3.x) * (pos.y - vertex3.y) - (vertex1.y - vertex3.y) * (pos.x - vertex3.x) < 0)))
        return -d;
    else
        return d;
}

Figure::AABBdata Triangle::AABB()
{
    AABBdata res;
    res.xMin = min(min(vertex1.x, vertex2.x), vertex3.x);
    res.yMin = min(min(vertex1.y, vertex2.y), vertex3.y);
    res.xMax = max(max(vertex1.x, vertex2.x), vertex3.x);
    res.yMax = max(max(vertex1.y, vertex2.y), vertex3.y);
    return res;
}

double Pie::SDF(Vector pos)
{
    double cos_ = cos((theta1 + theta2) / 2);
    double sin_ = sin((theta1 + theta2) / 2);
    Vector O = { 0, 0 };
    Vector D = { radius * cos((theta2 - theta1) / 2), radius * sin((theta2 - theta1) / 2) };
    pos = pos - center;
    Vector pos_ = { cos_ * pos.x + sin_ * pos.y, fabs(-sin_ * pos.x + cos_ * pos.y) };
    double l = pos_.module();
    if (D.x * pos_.y - D.y * pos_.x > 0)
        return segmentSDF(O, D, pos_);
    else if (l >= radius)
        return l - radius;
    else
        return max(l - radius, -segmentSDF(O, D, pos_));
}

Figure::AABBdata Pie::AABB()
{
    AABBdata res;
    double dtheta = theta2 - theta1;
    double x1 = radius * cos(theta1), y1 = radius * sin(theta1);
    double x2 = radius * cos(theta2), y2 = radius * sin(theta2);
    if ((0 - theta1) - 2 * PI * floor((0 - theta1) / (2 * PI)) < dtheta)
        res.xMax = center.x + radius;
    else
        res.xMax = center.x + max(max(x1, x2), 0.);
    if ((PI / 2 - theta1) - 2 * PI * floor((PI / 2 - theta1) / (2 * PI)) < dtheta)
        res.yMax = center.y + radius;
    else
        res.yMax = center.y + max(max(y1, y2), 0.);
    if ((PI - theta1) - 2 * PI * floor((PI - theta1) / (2 * PI)) < dtheta)
        res.xMin = center.x - radius;
    else
        res.xMin = center.x + min(min(x1, x2), 0.);
    if ((PI * 3 / 2 - theta1) - 2 * PI * floor((PI * 3 / 2 - theta1) / (2 * PI)) < dtheta)
        res.yMin = center.y - radius;
    else
        res.yMin = center.y + min(min(y1, y2), 0.);
    return res;
}

double Arc::SDF(Vector pos)
{
    double cos_ = cos((theta1 + theta2) / 2);
    double sin_ = sin((theta1 + theta2) / 2);
    Vector O = { 0, 0 };
    Vector D = { radius1 * cos((theta2 - theta1) / 2), radius1 * sin((theta2 - theta1) / 2) };
    pos = pos - center;
    Vector pos_ = { cos_ * pos.x + sin_ * pos.y, fabs(-sin_ * pos.x + cos_ * pos.y) };
    double l = pos_.module();
    if (D.x * pos_.y - D.y * pos_.x > 0)
        return (pos_ - D).module() - radius2;
    else
        return fabs(l - radius1) - radius2;
}

Figure::AABBdata Arc::AABB()
{
    AABBdata res;
    double dtheta = theta2 - theta1;
    double x1 = radius1 * cos(theta1), y1 = radius1 * sin(theta1);
    double x2 = radius1 * cos(theta2), y2 = radius1 * sin(theta2);
    if ((0 - theta1) - 2 * PI * floor((0 - theta1) / (2 * PI)) < dtheta)
        res.xMax = center.x + radius1 + radius2;
    else
        res.xMax = center.x + max(x1, x2) + radius2;
    if ((PI / 2 - theta1) - 2 * PI * floor((PI / 2 - theta1) / (2 * PI)) < dtheta)
        res.yMax = center.y + radius1 + radius2;
    else
        res.yMax = center.y + max(y1, y2) + radius2;
    if ((PI - theta1) - 2 * PI * floor((PI - theta1) / (2 * PI)) < dtheta)
        res.xMin = center.x - radius1 - radius2;
    else
        res.xMin = center.x + min(x1, x2) - radius2;
    if ((PI * 3 / 2 - theta1) - 2 * PI * floor((PI * 3 / 2 - theta1) / (2 * PI)) < dtheta)
        res.yMin = center.y - radius1 - radius2;
    else
        res.yMin = center.y + min(y1, y2) - radius2;
    return res;
}

double Polygon::SDF(Vector pos)
{
    Vector E, W;
    double d = 1e10;
    int s = 1;
    for (int i = 0, j; i < vertexNumber; i++)
    {
        j = (i + 1) % vertexNumber;
        d = min(d, segmentSDF(vertex[i], vertex[j], pos));
        if ((vertex[j].y >= pos.y && vertex[i].y < pos.y &&
            (vertex[j].x - vertex[i].x) * (pos.y - vertex[i].y) -
            (vertex[j].y - vertex[i].y) * (pos.x - vertex[i].x) > 0) ||
            (vertex[i].y >= pos.y && vertex[j].y < pos.y &&
                (vertex[j].x - vertex[i].x) * (pos.y - vertex[i].y) -
                (vertex[j].y - vertex[i].y) * (pos.x - vertex[i].x) < 0))
            s *= -1;
    }
    return s * d;
}

Figure::AABBdata Polygon::AABB()
{
    AABBdata res;
    res.xMin = vertex[0].x;
    res.xMax = vertex[0].x;
    res.yMin = vertex[0].y;
    res.yMax = vertex[0].y;
    for (int i = 0; i < vertexNumber; i++)
    {
        if (vertex[i].x > res.xMax)
            res.xMax = vertex[i].x;
        if (vertex[i].x < res.xMin)
            res.xMin = vertex[i].x;
        if (vertex[i].y > res.yMax)
            res.yMax = vertex[i].y;
        if (vertex[i].y < res.yMin)
            res.yMin = vertex[i].y;
    }
    return res;
}

double Ellipse::SDF(Vector pos)
{
    double cos_ = cos(theta);
    double sin_ = sin(theta);
    pos = pos - center;
    Vector p = { fabs(cos_ * pos.x + sin_ * pos.y), fabs(-sin_ * pos.x + cos_ * pos.y) };
    double a = this->a, b = this->b;
    if (p.x > p.y)
    {
        swap(p.x, p.y);
        swap(a, b);
    }
    double l = b * b - a * a;
    double m = a * p.x / l;
    double m2 = m * m;
    double n = b * p.y / l;
    double n2 = n * n;
    double c = (m2 + n2 - 1.0) / 3.0;
    double c3 = c * c * c;
    double q = c3 + m2 * n2 * 2.0;
    double d = c3 + m2 * n2;
    double g = m + m * n2;
    double co;
    if (d < 0.0)
    {
        double h = acos(q / c3) / 3.0;
        double s = cos(h);
        double t = sin(h) * sqrt(3.0);
        double rx = sqrt(-c * (s + t + 2.0) + m2);
        double ry = sqrt(-c * (s - t + 2.0) + m2);
        co = (ry + (l > 0 ? 1 : -1) * rx + fabs(g) / (rx * ry) - m) / 2.0;
    }
    else
    {
        double h = 2.0 * m * n * sqrt(d);
        double s = (q + h > 0 ? 1 : -1) * pow(fabs(q + h), 1.0 / 3.0);
        double u = (q - h > 0 ? 1 : -1) * pow(fabs(q - h), 1.0 / 3.0);
        double rx = -s - u - c * 4.0 + 2.0 * m2;
        double ry = (s - u) * sqrt(3.0);
        double rm = sqrt(rx * rx + ry * ry);
        co = (ry / sqrt(rm - rx) + 2.0 * g / rm - m) / 2.0;
    }
    Vector r = { a * co, b * sqrt(1 - co * co) };
    return (r - p).module() * (p.y - r.y > 0 ? 1 : -1);
}

Figure::AABBdata Ellipse::AABB()
{
    AABBdata res;
    double cos_ = cos(theta);
    double sin_ = sin(theta);
    double dx = max(abs(a * cos_ - b * sin_), abs(a * cos_ + b * sin_));
    double dy = max(abs(a * sin_ + b * cos_), abs(a * sin_ - b * cos_));
    res.xMin = center.x - dx;
    res.yMin = center.y - dy;
    res.xMax = center.x + dx;
    res.yMax = center.y + dy;
    return res;
}

double QuadraticBezier::SDF(Vector pos)
{
    Vector a = B - A;
    Vector b = A - B * 2.0 + C;
    Vector c = a * 2.0;
    Vector d = A - pos;
    double kk = 1.0 / (b * b);
    double kx = kk * (a * b);
    double ky = kk * (2.0 * (a * a) + (d * b)) / 3.0;
    double kz = kk * (d * a);
    double p = ky - kx * kx;
    double p3 = p * p * p;
    double q = kx * (2.0 * kx * kx - 3.0 * ky) + kz;
    double h = q * q + 4.0 * p3;
    double res = 0.0;
    if (h >= 0)
    {
        h = sqrt(h);
        double x = (h - q) / 2.0;
        double y = (-h - q) / 2.0;
        double u = (x > 0 ? 1 : -1) * pow(fabs(x), 1.0 / 3.0);
        double v = (y > 0 ? 1 : -1) * pow(fabs(y), 1.0 / 3.0);
        double t = u + v - kx;
        t = (t < 0 ? 0 : (t > 1 ? 1 : t));
        Vector e = d + (c + b * t) * t;
        res = e * e;
    }
    else
    {
        double z = sqrt(-p);
        double v = acos(q / (p * z * 2.0)) / 3.0;
        double m = cos(v);
        double n = sin(v) * sqrt(3);
        double x = (m + m) * z - kx;
        x = (x < 0 ? 0 : (x > 1 ? 1 : x));
        double y = (-n - m) * z - kx;
        y = (y < 0 ? 0 : (y > 1 ? 1 : y));
        Vector e = d + (c + b * x) * x;
        Vector f = d + (c + b * y) * y;
        res = min(e * e, f * f);
    }
    return sqrt(res);
}

Figure::AABBdata QuadraticBezier::AABB()
{
    AABBdata res;
    res.xMin = min(min(A.x, B.x), C.x);
    res.yMin = min(min(A.y, B.y), C.y);
    res.xMax = max(max(A.x, B.x), C.x);
    res.yMax = max(max(A.y, B.y), C.y);
    return res;
}
