#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <fstream>

using namespace std;

class Font
{
	public:
		//单个字符的点阵
		typedef struct
		{
			uint16_t width;
			bool* data;
		}FontMatrix;

	private:
		int height;

		//所有字符的点阵数组
		FontMatrix* fontData;

		//解析特定字符的点阵
		FontMatrix getFontMatrix(char* data, char c);

	public:
		Font(const char* filename, int height);
		~Font() { delete[] fontData; }

		//获取特定字符的点阵
		FontMatrix operator[](int idx) { return fontData[idx - 32]; }

		//获取字体高度
		int getHeight() { return height; }

		//获取字符串宽度
		int stringWidth(string str);
		double stringWidth(string str, double height) { return stringWidth(str) * height / this->height; }
};

#endif