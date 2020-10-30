#ifndef IMAGE_H
#define IMAGE_H

#include "../Color/Color.h"
#include "../Figure/Figure.h"
#include "../Font/Font.h"
#include <functional>

class Image
{
    public:
        //采样方法：（临近，双线性，双三次，LANCZOS）
        enum resampling {NEAREST, BILINEAR, BICUBIC, LANCZOS};

    private:
        Color* data;
        uint32_t width;
        uint32_t height;

        //卷积核
        typedef struct
        {
             function<double(double)> h;
             double boundary;
        }Kernel;

        const Kernel nearest = {
            [](double x)->double{return 1;},
            0.5
        };

        const Kernel biLinear = {
            [](double x)->double{return 1 - abs(x);},
            1
        };

        const Kernel biCubic = {
            [](double x)->double{
                x = abs(x);
                return (x < 1) ? (1.5 * x * x * x - 2.5 * x * x + 1) :
                                (-0.5 * x * x * x + 2.5 * x * x - 4 * x + 2);
            },
            2
        };

        const Kernel lanczos = {
            [](double x)->double{return x == 0 ? 1 : (3 * sin(PI * x) * sin(PI * x / 3)) / (PI * PI * x * x);},
            3
        };

        double clamp(double x, double low, double high);

        //重采样函数，参数(x坐标，y坐标，重采样比率（小于1为缩小，大于1为放大），重采样类型（枚举中选择）)
        Color resample(double x, double y, double kx, double ky, resampling type);

    public:
        Image() : width(0), height(0), data(NULL) {}

        //构造函数，以图像的宽度与高度（以像素计）创建对象
        Image(uint32_t width, uint32_t height);

        //创建文字原图（一一对应填充）
        Image(string str, Font& font, Color color);

        Image(Image& I);    //拷贝构造
        Image(Image&& I);   //移动构造
        ~Image();           //析构函数

        Image& operator=(Image& I);     //拷贝赋值
        Image& operator=(Image&& I);    //转移赋值

        int getHeight() { return height; }
        int getWidth() { return width; }

        //设置背景
        void setBackgroundColor(Color color);

        //将一像素点（以坐标(x, y)表示）设为特定颜色
        void setPixel(uint32_t x, uint32_t y, Color color);

        //获取像素颜色
        Color getPixel(uint32_t x, uint32_t y);

        //按透明度叠加像素
        void overliePixel(uint32_t x, uint32_t y, Color color);
        
        //绘制 Figure
        Image& draw(Figure& s);

        //按宽高缩放图片（可变形）
        Image resize(int width, int height, resampling type);

        //只按高度缩放图片（不变形）
        Image resize(int height, resampling type);

        //插入图片（源，锚点位置，源上锚点位置，宽度，高度，旋转角，采样方法）
        Image& insert(Image& src, Vector pos, Vector center, double width, double height, double theta, resampling type);

        //插入图片（源，锚点位置，源上锚点位置，高度，旋转角，采样方法）
        Image& insert(Image& src, Vector pos, Vector center, double height, double theta, resampling type);

        //插入文字（文字，目标位置，源上对应pos的位置，高度，旋转角，字体，颜色）
        Image& addText(string str, Vector pos, Vector center, double size, double theta, Font& font, Color color);

        //插入标题（插入文字的预定义版本）
        Image& addTitle(string str, double size, Font& font, Color color);

        //BMP文件头结构体
        #pragma pack(2)     //按2字节对齐，避免结构体中空位
        typedef struct
        {
            uint16_t bfType;
            uint32_t bfSize;
            uint16_t bfReserved1;
            uint16_t bfReserved2;
            uint32_t bfOffBits;
            uint32_t biSize;
            uint32_t biWidth;
            uint32_t biHeight;
            uint16_t biPlanes;
            uint16_t biBitCount;
            uint32_t biCompression;
            uint32_t biSizeImage;
            uint32_t biXPelsPerMeter;
            uint32_t biYPelsPerMeter;
            uint32_t biClrUsed;
            uint32_t biClrImportant;
        }BMPHead;
        #pragma pack()      //取消自定义对齐设置

        //读取BMP文件
        Image& readBMP(const char* filename);

        //保存BMP文件
        void saveBMP(const char* filename);
};

#endif