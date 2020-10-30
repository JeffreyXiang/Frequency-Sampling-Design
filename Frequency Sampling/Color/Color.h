#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

using namespace std;

class Color
{
    public:
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        double alpha;

        //构造函数，初始值为(0, 0, 0, 0)
        Color();

        //构造函数 16进制(0xRRGGBB)
        Color(uint32_t data);
        Color(uint8_t red, uint8_t green, uint8_t blue);
        Color(uint8_t red, uint8_t green, uint8_t blue, double alpha);
        //RBG + A
        Color(Color& C, double alpha);

        ~Color() {}

        //用RGB设置颜色，alpha默认为1
        Color& rgb(uint8_t red, uint8_t green, uint8_t blue);

        //用RGBA设置颜色
        Color& rgba(uint8_t red, uint8_t green, uint8_t blue, double alpha);

        //用颜色+alpha设置
        Color& ca(Color C, double alpha);

        Color operator*(double k);

        //混合
        static Color mix(Color C1, Color C2, double k);

        //叠加
        static Color overlie(Color C1, Color C2);
};

#endif