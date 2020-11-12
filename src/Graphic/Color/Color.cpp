#include "Color.h"

Color::Color()
{
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
}

Color::Color(uint32_t data)
{
    this->red = data >> 16;
    this->green = data >> 8;
    this->blue = data;
    this->alpha = 1;
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = 1;
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, double alpha)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}

Color::Color(Color& C, double alpha)
{
    red = C.red;
    green = C.green;
    blue = C.blue;
    this->alpha = alpha * C.alpha;
}

Color& Color::rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = 1;
    return *this;
}

Color& Color::rgba(uint8_t red, uint8_t green, uint8_t blue, double alpha)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
    return *this;
}

Color& Color::ca(Color C, double alpha)
{
    red = C.red;
    green = C.green;
    blue = C.blue;
    this->alpha = alpha * C.alpha;
    return *this;
}

Color Color::operator*(double k)
{
    Color result;
    result.rgba(red * k, green * k, blue * k, alpha);
    return result;
}

Color Color::mix(Color C1, Color C2, double k)
{
    Color res;
    k = k < 0 ? 0 : k > 1 ? 1 : k;
    res.rgba(k * C1.red + (1 - k) * C2.red, k * C1.green + (1 - k) * C2.green, k * C1.blue + (1 - k) * C2.blue, k * C1.alpha + (1 - k) * C2.alpha);
    return res;
}

Color Color::overlie(Color C1, Color C2)
{
    Color res;
    double alpha = 1 - (1 - C1.alpha) * (1 - C2.alpha);
    res.rgba((C1.alpha * C1.red + (1 - C1.alpha) * C2.alpha * C2.red) / alpha,
        (C1.alpha * C1.green + (1 - C1.alpha) * C2.alpha * C2.green) / alpha,
        (C1.alpha * C1.blue + (1 - C1.alpha) * C2.alpha * C2.blue) / alpha,
        alpha);
    return res;
}
