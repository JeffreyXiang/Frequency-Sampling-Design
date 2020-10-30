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

		//窗口区域
		typedef struct
		{
			double yMin;
			double yMax;
		} Range;

		//图表上的文字标签
		typedef struct
		{
			char* text;
			Color color;
			Font* font;
			double height;
		} Lable;

		//连续数据
		typedef struct
		{
			function<double(double)> func;
			Color color;
			double lineWidth;
		} Continuous;

		//离散数据
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

		Lable title;				//图表标题
		Lable verName;				//纵轴名字
		Lable horName;				//横轴名字

		Color rulerColor;			//标尺颜色
		Font* rulerFont;			//标尺字体
		double rulerHeight;			//标尺字体高度

		Range range;

		bool autoRangeEnabled;	    //自动缩放
		bool verMarkEnabled;		//显示刻度
		bool horMarkEnabled;
		bool verNumberEnabled;		//显示数字
		bool horNumberEnabled;

		Font* defFont;				//默认字体

		void plotRaw(Image& image, vector<Discrete> data, int points, double xMin, double xMax, bool isDiscrete);

	public:
		Plot() :
			width(defWidth), height(defHeight), 
			range({ 0, 0 }), defFont(NULL),
			title({ NULL, { 0, 0, 0 }, NULL, defTitleH }),
			verName({ NULL, { 0, 0, 0 }, NULL, defNameH }),
			horName({ NULL, { 0, 0, 0 }, NULL, defNameH }),
			rulerColor(0, 0, 0), rulerHeight(defNumberH), rulerFont(NULL),
			verMarkEnabled(true), horMarkEnabled(true),
			verNumberEnabled(true), horNumberEnabled(true),
			autoRangeEnabled(true) {}

		~Plot() {}

		//设置输出尺寸
		void setSize(int w, int h) { width = w; height = h; }

		//设置默认字体
		void setDefaultFont(Font& font) { defFont = &font; }

		//设置标题
		void setTitle(const char* text, Color color = { 0, 0, 0 }, double height = defTitleH, Font* font = NULL)
		{ title.text = (char*)text; title.color = color; title.height = height; title.font = font; }

		//设置纵轴名字
		void setVerName(const char* text, Color color = { 0, 0, 0 }, double height = defNameH, Font* font = NULL)
		{ verName.text = (char*)text; verName.color = color; verName.height = height; verName.font = font; }

		//设置横轴名字
		void setHorName(const char* text, Color color = { 0, 0, 0 }, double height = defNameH, Font* font = NULL)
		{ horName.text = (char*)text; horName.color = color; horName.height = height; horName.font = font; }

		//设置标尺
		void setRuler(Color color = { 0, 0, 0 }, double height = defNumberH, Font* font = NULL)
		{ rulerColor = color; rulerHeight = height; rulerFont = font; }

		//设置自动缩放
		void setAutoRangeEnabled(bool enabled) { autoRangeEnabled = enabled; }

		//设置纵轴范围
		void setRange(Range range) { this->range = range; }
		void setRange(double min, double max) { range = { min, max }; }

		//设置纵轴刻度显示
		void setVerMarkEnabled(bool enabled) { verMarkEnabled = enabled; }

		//设置横轴刻度显示
		void setHorMarkEnabled(bool enabled) { horMarkEnabled = enabled; }

		//设置纵轴数字显示
		void setVerNumberEnabled(bool enabled) { verNumberEnabled = enabled; }

		//设置横轴数字显示
		void setHorNumberEnabled(bool enabled) { horNumberEnabled = enabled; }

		//画函数
		Image plot(double xMin, double xMax, int points, vector<Continuous> funcs);

		//画离散数据
		Image plot(int points, vector<Discrete> data);
};

#endif // !PLOT_H
