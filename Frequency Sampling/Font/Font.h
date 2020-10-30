#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <fstream>

using namespace std;

class Font
{
	public:
		//�����ַ��ĵ���
		typedef struct
		{
			uint16_t width;
			bool* data;
		}FontMatrix;

	private:
		int height;

		//�����ַ��ĵ�������
		FontMatrix* fontData;

		//�����ض��ַ��ĵ���
		FontMatrix getFontMatrix(char* data, char c);

	public:
		Font(const char* filename, int height);
		~Font() { delete[] fontData; }

		//��ȡ�ض��ַ��ĵ���
		FontMatrix operator[](int idx) { return fontData[idx - 32]; }

		//��ȡ����߶�
		int getHeight() { return height; }

		//��ȡ�ַ������
		int stringWidth(string str);
		double stringWidth(string str, double height) { return stringWidth(str) * height / this->height; }
};

#endif