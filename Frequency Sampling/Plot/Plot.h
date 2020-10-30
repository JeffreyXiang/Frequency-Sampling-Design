#ifndef PLOT_H
#define PLOT_H

#include "../Image/Image.h"
#include "../Figure/Figure.h"
#include "../Font/Font.h"

#include <functional>
#include <vector>

class Plot
{
	public:
		enum ERROR { FONT_NOT_SET };

		//ͼ���ϵ����ֱ�ǩ
		typedef struct
		{
			char* text;
			Color color;
			Font* font;
			double height;
		} Lable;

		//��������
		typedef struct
		{
			function<double(double)> func;
			Color color;
			double lineWidth;
		} Continuous;

		//��ɢ����
		typedef struct
		{
			double* data;
			Color color;
			double lineWidth;
		} Discrete;

	private:
		static const int defTitleH = 72;
		static const int defNameH = 48;
		static const int defNumberH = 32;
		static const int defWidth = 1920;
		static const int defHeight = 1080;

		int width;
		int height;

		Lable title;				//ͼ�����
		Lable verName;				//��������
		Lable horName;				//��������

		Color rulerColor;			//�����ɫ
		Font* rulerFont;			//�������
		double rulerHeight;			//�������߶�
		bool verScaleEnabled;		//��ʾ�̶�
		bool horScaleEnabled;
		bool verNumberEnabled;		//��ʾ����
		bool horNumberEnabled;

		Font* defFont;				//Ĭ������

		void plotRaw(Image& image, vector<Discrete> data, int points, double xMin, double xMax, int flag);

	public:
		Plot() :
			width(defWidth), height(defHeight), defFont(NULL),
			title({ NULL, { 0, 0, 0 }, NULL, defTitleH }),
			verName({ NULL, { 0, 0, 0 }, NULL, defNameH }),
			horName({ NULL, { 0, 0, 0 }, NULL, defNameH }),
			rulerColor(0, 0, 0), rulerHeight(defNumberH), rulerFont(NULL),
			verScaleEnabled(true), horScaleEnabled(true),
			verNumberEnabled(true), horNumberEnabled(true) {}

		~Plot() {}

		//��������ߴ�
		void setSize(int w, int h) { width = w; height = h; }

		//����Ĭ������
		void setDefaultFont(Font& font) { defFont = &font; }

		//���ñ���
		void setTitle(const char* text, Color color = { 0, 0, 0 }, double height = defTitleH, Font* font = NULL)
		{ title.text = (char*)text; title.color = color; title.height = height; title.font = font; }

		//������������
		void setVerName(const char* text, Color color = { 0, 0, 0 }, double height = defNameH, Font* font = NULL)
		{ verName.text = (char*)text; verName.color = color; verName.height = height; verName.font = font; }

		//���ú�������
		void setHorName(const char* text, Color color = { 0, 0, 0 }, double height = defNameH, Font* font = NULL)
		{ horName.text = (char*)text; horName.color = color; horName.height = height; horName.font = font; }

		//���ñ��
		void setRuler(Color color = { 0, 0, 0 }, double height = defNumberH, Font* font = NULL)
		{ rulerColor = color; rulerHeight = height; rulerFont = font; }

		//��������̶���ʾ
		void setVerScaleEnabled(bool enabled) { verScaleEnabled = enabled; }

		//���ú���̶���ʾ
		void setHorScaleEnabled(bool enabled) { horScaleEnabled = enabled; }

		//��������������ʾ
		void setVerNumberEnabled(bool enabled) { verNumberEnabled = enabled; }

		//���ú���������ʾ
		void setHorNumberEnabled(bool enabled) { horNumberEnabled = enabled; }

		//������
		Image plot(double xMin, double xMax, int points, vector<Continuous> funcs);

		//����ɢ����
		Image plot(int points, vector<Discrete> data);
};

#endif // !PLOT_H
