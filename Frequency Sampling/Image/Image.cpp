#include "Image.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>

double Image::clamp(double x, double low, double high)
{
    return x < low ? low : x > high ? high : x;
}

//重采样函数，参数(x坐标，y坐标，重采样比率（小于1为缩小，大于1为放大），重采样类型（枚举中选择）)
Color Image::resample(double x, double y, double kx, double ky, resampling type)
{
    double r = 0, g = 0, b = 0, a = 0, h;
    int u, v;
    Color pc;

    //整数化 k
    if (kx > 1) kx = 1;
    kx = 1 / round(1 / kx);
    if (ky > 1) ky = 1;
    ky = 1 / round(1 / ky);

    //找到所用的卷积核
    const Kernel* kernel = NULL;
    switch (type)
    {
    case NEAREST: kernel = &nearest; break;
    case BILINEAR: kernel = &biLinear; break;
    case BICUBIC: kernel = &biCubic; break;
    case LANCZOS: kernel = &lanczos; break;
    }

    //遍历卷积核的非0区域
    for (int i = ceil(x - kernel->boundary / kx); i <= floor(x + kernel->boundary / kx); i++)
        for (int j = ceil(y - kernel->boundary / ky); j <= floor(y + kernel->boundary / ky); j++)
        {
            //最近取值法扩展取色
            if (i < 0) u = 0;
            else if (i >= width) u = width - 1;
            else u = i;
            if (j < 0) v = 0;
            else if (j >= height) v = height - 1;
            else v = j;
            pc = getPixel(u, v);

            //按公式进行重采样计算（颜色按透明度加权）
            h = kx * ky * kernel->h(kx * (i - x)) * kernel->h(ky * (j - y));
            r += pc.alpha * pc.red * h;
            g += pc.alpha * pc.green * h;
            b += pc.alpha * pc.blue * h;
            a += pc.alpha * h;
        }
    r /= a;
    g /= a;
    b /= a;

    //返回钳位过的颜色
    return {
        (uint8_t)clamp(r, 0, 255),
        (uint8_t)clamp(g, 0, 255),
        (uint8_t)clamp(b, 0, 255),
        clamp(a, 0, 1)
    };
}
   
Image::Image(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;
    data = new Color[width * height];
    for (int i = 0; i < width * height; i++)
        data[i].rgba(0, 0, 0, 0);
}

//生成文字原图（一一对应）
Image::Image(string str, Font& font, Color color)
{
    //计算文字宽高
    width = font.stringWidth(str);
    height = font.getHeight();

    //创建画布
    data = new Color[width * height];
    for (int i = 0; i < width * height; i++)
        data[i].rgba(0, 0, 0, 0);

    //依次绘制文字
    int temp = 0;
    for (int i = 0; i < str.length(); i++)
    {
        for (int j = 0; j < height; j++)
            for (int k = 0; k < font[str[i]].width; k++)
                if (font[str[i]].data[j * font[str[i]].width + k])
                    overliePixel(k + temp, height - j - 1, color);
        temp += font[str[i]].width;
    }
}

Image::Image(Image& I)
{
    width = I.width;
    height = I.height;
    data = new Color[width * height];
    for (int i = 0; i < width * height; i++)
        data[i] = I.data[i];
}

Image::Image(Image&& I)
{
    width = I.width;
    height = I.height;
    data = I.data;
    I.data = NULL;
}

Image::~Image()
{
    delete[] data;
}

Image& Image::operator=(Image& I)
{
    delete[] data;
    width = I.width;
    height = I.height;
    data = new Color[width * height];
    for (int i = 0; i < width * height; i++)
        data[i] = I.data[i];
    return *this;
}

Image& Image::operator=(Image&& I)
{
    delete[] data;
    width = I.width;
    height = I.height;
    data = I.data;
    I.data = NULL;
    return *this;
}

void Image::setBackgroundColor(Color color)
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            setPixel(x, y, color);
}

void Image::setPixel(uint32_t x, uint32_t y, Color color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    data[y * width + x] = color;
}

Color Image::getPixel(uint32_t x, uint32_t y)
{
    return data[y * width + x];
}

void Image::overliePixel(uint32_t x, uint32_t y, Color color)
{
    setPixel(x, y, Color::overlie(color, getPixel(x, y)));
}

Image& Image::draw(Figure& s)
{
    Figure::AABBdata b = s.tAABB();
    int xMin = max((int)floor(b.xMin), 0);
    int yMin = max((int)floor(b.yMin), 0);
    int xMax = min((int)ceil(b.xMax), (int)width - 1);
    int yMax = min((int)ceil(b.yMax), (int)height - 1);
    /*for (int u = xMin; u <= xMax; u++)
    {
        setPixel(u, yMin, {0, 255, 0});
        setPixel(u, yMax, {0, 255, 0});
    }
    for (int v = yMin; v <= yMax; v++)
    {
        setPixel(xMin, v, {0, 255, 0});
        setPixel(xMax, v, {0, 255, 0});
    }*/
    Color final;
    double alpha;
    for (int u = xMin; u <= xMax; u++)
        for (int v = yMin; v <= yMax; v++)
        {
            alpha = -s.tSDF({ (double)u, (double)v }) + 0.5;
            alpha = clamp(alpha, 0, 1);
            if (alpha > 0)
            {
                final.ca(s.getAttribute().color, alpha);
                overliePixel(u, v, final);
            }
        }
    return *this;
}

//按宽高缩放图片（可变形）
Image Image::resize(int width, int height, resampling type)
{
    //计算缩放比例
    double kx = (double)this->width / width;
    double ky = (double)this->height / height;

    //创建新图像
    Image res(width, height);

    //按采样结果填充
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            res.setPixel(i, j, resample(kx * i + (kx - 1) / 2, ky * j + (ky - 1) / 2, 1 / kx, 1 / ky, type));
    return res;
}

//只按高度缩放图片（不变形）
Image Image::resize(int height, resampling type)
{
    int width = this->width * height / this->height;
    return resize(width, height, type);
}

//插入图片（源，锚点位置，源上锚点位置，宽度，高度，旋转角，采样方法）
Image& Image::insert(Image& src, Vector pos, Vector center, double width, double height, double theta, resampling type)
{
    theta = (theta - 360 * floor(theta / 360)) * PI / 180;
    double cos_ = cos(theta);
    double sin_ = sin(theta);
    double kx = width / src.width;      //缩放比例
    double ky = height / src.height;

    //计算AABB包围盒
    double xMin, xMax, yMin, yMax;      //AABB
    double xL = -center.x * width;      //四角坐标(L,R,T,B: 左右上下)
    double xR = (1 - center.x) * width;
    double yT = (1 - center.y) * height;
    double yB = -center.y * height;
    if (theta < PI / 2)
    {
        xMin = pos.x + xL * cos_ - yT * sin_;
        xMax = pos.x + xR * cos_ - yB * sin_;
        yMin = pos.y + xL * sin_ + yB * cos_;
        yMax = pos.y + xR * sin_ + yT * cos_;
    }
    else if (theta < PI)
    {
        xMin = pos.x + xR * cos_ - yT * sin_;
        xMax = pos.x + xL * cos_ - yB * sin_;
        yMin = pos.y + xL * sin_ + yT * cos_;
        yMax = pos.y + xR * sin_ + yB * cos_;
    }
    else if (theta < 3 * PI / 2)
    {
        xMin = pos.x + xR * cos_ - yB * sin_;
        xMax = pos.x + xL * cos_ - yT * sin_;
        yMin = pos.y + xR * sin_ + yT * cos_;
        yMax = pos.y + xL * sin_ + yB * cos_;
    }
    else
    {
        xMin = pos.x + xL * cos_ - yB * sin_;
        xMax = pos.x + xR * cos_ - yT * sin_;
        yMin = pos.y + xR * sin_ + yB * cos_;
        yMax = pos.y + xL * sin_ + yT * cos_;
    }
    xMin = max(xMin, 0.0); xMax = min(xMax, this->width - 1.0);
    yMin = max(yMin, 0.0); yMax = min(yMax, this->height - 1.0);

    double u, v;                        //插入图上的位置
    
    //按AABB遍历
    for (int i = floor(xMin); i <= ceil(xMax); i++)
        for (int j = floor(yMin); j <= ceil(yMax); j++)
        {
            //  -锚点比例-   ---------锚点到像素的矢量反向旋转----------  -化比例-   -化像素位置-       
            u = (center.x + ((i - pos.x) * cos_ + (j - pos.y) * sin_) / width) * src.width;
            v = (center.y + (-(i - pos.x) * sin_ + (j - pos.y) * cos_) / height) * src.height;
            if (u >= 0 && u < src.width && v >= 0 && v < src.height)    //在插入图内
                overliePixel(i, j, src.resample(u, v, kx, ky, type));
        }
    return *this;
}

//插入图片（源，锚点位置，源上锚点位置，高度，旋转角，采样方法）
Image& Image::insert(Image& src, Vector pos, Vector center, double height, double theta, resampling type)
{
    double width = height * src.width / src.height;
    return insert(src, pos, center, width, height, theta, type);
}

//插入文字（文字，目标位置，源上对应pos的位置，高度，旋转角，字体，颜色）
Image& Image::addText(string str, Vector pos, Vector center, double size, double theta, Font& font, Color color)
{
    //生成文字原图并插入
    Image* text = new Image(str, font, color);
    insert(*text, pos, center, size, theta, NEAREST);
    delete text;
    return *this;
}

Image& Image::addTitle(string str, double size, Font& font, Color color)
{
    //默认添加在目标图的上部中间，文字水平居中，顶部与目标图顶部对齐；
    addText(str, { width / 2.0, (double)height }, { 0.5, 1 }, size, 0, font, color);
    return *this;
}

//读取BMP文件
Image& Image::readBMP(const char* filename)
{
    delete[] data;
    fstream file(filename, ios::in | ios::binary);
    BMPHead head;
    file.read((char*)&head, 54);

    //暂时支持24位深度不压缩
    if (head.biBitCount != 24 || head.biCompression != 0)
    {
        cout << "Error: unsupported file.\n";
        exit(0);
    }

    //按文件头部的信息创建空间
    this->width = head.biWidth;
    this->height = head.biHeight;
    data = new Color[width * height];

    //读取数据
    int fillNum = (4 - (3 * width) % 4) % 4;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            file.read((char*)&data[i * width + j].blue, 1);
            file.read((char*)&data[i * width + j].green, 1);
            file.read((char*)&data[i * width + j].red, 1);
            data[i * width + j].alpha = 1;
        }
        file.seekg(fillNum, ios::cur);
    }
    file.close();
    return *this;
}

//保存BMP文件
void Image::saveBMP(const char* filename)
{
    //构造文件头
    uint32_t size = width * height * 3 + 54;
    BMPHead head = {
        0x4D42,             //bfType("BM")
        size,               //bfSize
        0x0000,0x0000,      //bfReserved1,2(0)
        0x00000036,         //bfOffBits(54)
        0x00000028,         //biSize(40)
        width,              //biWidth
        height,             //biHeight
        0x0001,             //biPlanes(1)
        0x0018,             //biBitCount(24)
        0x00000000,         //biCompression(0, 无压缩)
        0x00000000,         //biSizeImage(缺省)
        0x00000000,         //biXPelsPerMeter(缺省)
        0x00000000,         //biYPelsPerMeter(缺省)
        0x00000000,         //biClrUsed(0,全部颜色)
        0x00000000          //biClrImportant(0,全部颜色)
    };

    //打开文件
    cout << "Exporting...\n";
    fstream file(filename, ios::out | ios::binary);
    if (!file)
    {
        cout << "Error: File open failed.\n";
        return;
    }

    //写入文件头
    file.write((char*)&head, 54);

    //写入数据
    uint8_t fillBytes[3] = { 0 };
    int fillNum = (4 - (3 * width) % 4) % 4;
    Color final;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            final = Color::overlie(data[i * width + j], { 255, 255, 255 });
            file.write((char*)&final.blue, 1);
            file.write((char*)&final.green, 1);
            file.write((char*)&final.red, 1);
        }
        file.write((char*)fillBytes, fillNum);
    }
    file.close();
}
