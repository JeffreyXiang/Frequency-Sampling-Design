#include "Font.h"

Font::FontMatrix Font::getFontMatrix(char* data, char c)
{
	FontMatrix res;

	//字符在字库中的偏移量
	int offset = (2 + height * height / 8) * (c - 32);

	//读取宽度和字符
	res.width = ((uint16_t)*(data + offset) << 8) | ((uint8_t)*(data + offset + 1));
	res.data = new bool[res.width * height];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < res.width; j++)
		{
			res.data[i * res.width + j] = *(data + offset + 2 + height / 8 * i + j / 8) & (0x80 >> j % 8);
		}
	return res;
}

Font::Font(const char* filename, int height)
{
	this->height = height;

	//读取整个文件
	ifstream f(filename, ios::binary); 
	if (!f.is_open())
	{
		cout<<"Error: no such file.\n";
		exit(0);
	}
	int length;  
	f.seekg(0, ios::end);
	length = f.tellg(); 
	f.seekg(0, ios::beg);
	char* data = new char[length];
	f.read(data, length);
	f.close();

	//解析字符点阵
	fontData = new FontMatrix[96];
	for (int i = 32; i < 128; i++)
	{
		fontData[i - 32] = getFontMatrix(data, i);
	}
	delete[] data;
}

//获取字符串宽度
int Font::stringWidth(string str)
{
	int width = 0;
	for (int i = 0; i < str.length(); i++)
		width += (*this)[str[i]].width;
	return width;
}
