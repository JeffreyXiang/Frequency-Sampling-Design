#include "Image.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>

double Image::clamp(double x, double low, double high)
{
    return x < low ? low : x > high ? high : x;
}

//�ز�������������(x���꣬y���꣬�ز������ʣ�С��1Ϊ��С������1Ϊ�Ŵ󣩣��ز������ͣ�ö����ѡ��)
Color Image::resample(double x, double y, double kx, double ky, resampling type)
{
    double r = 0, g = 0, b = 0, a = 0, h;
    int u, v;
    Color pc;

    //������ k
    if (kx > 1) kx = 1;
    kx = 1 / round(1 / kx);
    if (ky > 1) ky = 1;
    ky = 1 / round(1 / ky);

    //�ҵ����õľ����
    const Kernel* kernel = NULL;
    switch (type)
    {
    case NEAREST: kernel = &nearest; break;
    case BILINEAR: kernel = &biLinear; break;
    case BICUBIC: kernel = &biCubic; break;
    case LANCZOS: kernel = &lanczos; break;
    }

    //��������˵ķ�0����
    for (int i = ceil(x - kernel->boundary / kx); i <= floor(x + kernel->boundary / kx); i++)
        for (int j = ceil(y - kernel->boundary / ky); j <= floor(y + kernel->boundary / ky); j++)
        {
            //���ȡֵ����չȡɫ
            if (i < 0) u = 0;
            else if (i >= width) u = width - 1;
            else u = i;
            if (j < 0) v = 0;
            else if (j >= height) v = height - 1;
            else v = j;
            pc = getPixel(u, v);

            //����ʽ�����ز������㣨��ɫ��͸���ȼ�Ȩ��
            h = kx * ky * kernel->h(kx * (i - x)) * kernel->h(ky * (j - y));
            r += pc.alpha * pc.red * h;
            g += pc.alpha * pc.green * h;
            b += pc.alpha * pc.blue * h;
            a += pc.alpha * h;
        }
    r /= a;
    g /= a;
    b /= a;

    //����ǯλ������ɫ
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

//��������ԭͼ��һһ��Ӧ��
Image::Image(string str, Font& font, Color color)
{
    //�������ֿ��
    width = font.stringWidth(str);
    height = font.getHeight();

    //��������
    data = new Color[width * height];
    for (int i = 0; i < width * height; i++)
        data[i].rgba(0, 0, 0, 0);

    //���λ�������
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

//���������ͼƬ���ɱ��Σ�
Image Image::resize(int width, int height, resampling type)
{
    //�������ű���
    double kx = (double)this->width / width;
    double ky = (double)this->height / height;

    //������ͼ��
    Image res(width, height);

    //������������
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            res.setPixel(i, j, resample(kx * i + (kx - 1) / 2, ky * j + (ky - 1) / 2, 1 / kx, 1 / ky, type));
    return res;
}

//ֻ���߶�����ͼƬ�������Σ�
Image Image::resize(int height, resampling type)
{
    int width = this->width * height / this->height;
    return resize(width, height, type);
}

//����ͼƬ��Դ��ê��λ�ã�Դ��ê��λ�ã���ȣ��߶ȣ���ת�ǣ�����������
Image& Image::insert(Image& src, Vector pos, Vector center, double width, double height, double theta, resampling type)
{
    theta = (theta - 360 * floor(theta / 360)) * PI / 180;
    double cos_ = cos(theta);
    double sin_ = sin(theta);
    double kx = width / src.width;      //���ű���
    double ky = height / src.height;

    //����AABB��Χ��
    double xMin, xMax, yMin, yMax;      //AABB
    double xL = -center.x * width;      //�Ľ�����(L,R,T,B: ��������)
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

    double u, v;                        //����ͼ�ϵ�λ��
    
    //��AABB����
    for (int i = floor(xMin); i <= ceil(xMax); i++)
        for (int j = floor(yMin); j <= ceil(yMax); j++)
        {
            //  -ê�����-   ---------ê�㵽���ص�ʸ��������ת----------  -������-   -������λ��-       
            u = (center.x + ((i - pos.x) * cos_ + (j - pos.y) * sin_) / width) * src.width;
            v = (center.y + (-(i - pos.x) * sin_ + (j - pos.y) * cos_) / height) * src.height;
            if (u >= 0 && u < src.width && v >= 0 && v < src.height)    //�ڲ���ͼ��
                overliePixel(i, j, src.resample(u, v, kx, ky, type));
        }
    return *this;
}

//����ͼƬ��Դ��ê��λ�ã�Դ��ê��λ�ã��߶ȣ���ת�ǣ�����������
Image& Image::insert(Image& src, Vector pos, Vector center, double height, double theta, resampling type)
{
    double width = height * src.width / src.height;
    return insert(src, pos, center, width, height, theta, type);
}

//�������֣����֣�Ŀ��λ�ã�Դ�϶�Ӧpos��λ�ã��߶ȣ���ת�ǣ����壬��ɫ��
Image& Image::addText(string str, Vector pos, Vector center, double size, double theta, Font& font, Color color)
{
    //��������ԭͼ������
    Image* text = new Image(str, font, color);
    insert(*text, pos, center, size, theta, NEAREST);
    delete text;
    return *this;
}

Image& Image::addTitle(string str, double size, Font& font, Color color)
{
    //Ĭ�������Ŀ��ͼ���ϲ��м䣬����ˮƽ���У�������Ŀ��ͼ�������룻
    addText(str, { width / 2.0, (double)height }, { 0.5, 1 }, size, 0, font, color);
    return *this;
}

//��ȡBMP�ļ�
Image& Image::readBMP(const char* filename)
{
    delete[] data;
    fstream file(filename, ios::in | ios::binary);
    BMPHead head;
    file.read((char*)&head, 54);

    //��ʱ֧��24λ��Ȳ�ѹ��
    if (head.biBitCount != 24 || head.biCompression != 0)
    {
        cout << "Error: unsupported file.\n";
        exit(0);
    }

    //���ļ�ͷ������Ϣ�����ռ�
    this->width = head.biWidth;
    this->height = head.biHeight;
    data = new Color[width * height];

    //��ȡ����
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

//����BMP�ļ�
void Image::saveBMP(const char* filename)
{
    //�����ļ�ͷ
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
        0x00000000,         //biCompression(0, ��ѹ��)
        0x00000000,         //biSizeImage(ȱʡ)
        0x00000000,         //biXPelsPerMeter(ȱʡ)
        0x00000000,         //biYPelsPerMeter(ȱʡ)
        0x00000000,         //biClrUsed(0,ȫ����ɫ)
        0x00000000          //biClrImportant(0,ȫ����ɫ)
    };

    //���ļ�
    cout << "Exporting...\n";
    fstream file(filename, ios::out | ios::binary);
    if (!file)
    {
        cout << "Error: File open failed.\n";
        return;
    }

    //д���ļ�ͷ
    file.write((char*)&head, 54);

    //д������
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
