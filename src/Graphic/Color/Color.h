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

        //���캯������ʼֵΪ(0, 0, 0, 0)
        Color();

        //���캯�� 16����(0xRRGGBB)
        Color(uint32_t data);
        Color(uint8_t red, uint8_t green, uint8_t blue);
        Color(uint8_t red, uint8_t green, uint8_t blue, double alpha);
        //RBG + A
        Color(Color& C, double alpha);

        ~Color() {}

        //��RGB������ɫ��alphaĬ��Ϊ1
        Color& rgb(uint8_t red, uint8_t green, uint8_t blue);

        //��RGBA������ɫ
        Color& rgba(uint8_t red, uint8_t green, uint8_t blue, double alpha);

        //����ɫ+alpha����
        Color& ca(Color C, double alpha);

        Color operator*(double k);

        //���
        static Color mix(Color C1, Color C2, double k);

        //����
        static Color overlie(Color C1, Color C2);
};

#endif