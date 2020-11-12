#include "Plot.h"
#include <sstream>
#include <iomanip>

void Plot::plotRaw(Image& image, vector<Discrete> data, int points, double xMin, double xMax, bool isDiscrete)
{
    if (defFont == NULL)
    {
        cerr << "ERROR | Plot::plotRaw(Image&, vector<Discrete>, int, double, double, bool) : Default font not set.\n";
        throw FONT_NOT_SET;
    }

    //��ʼ������
    if (!title.font) title.font = defFont;
    if (!verName.font) verName.font = defFont;
    if (!horName.font) horName.font = defFont;
    if (!rulerFont) rulerFont = defFont;

    //��ȡ��ֵ����ֵ���С2e-6��
    if (autoRangeEnabled)
    {
        range.yMax = data[0].data[0], range.yMin = range.yMax;
        for (size_t i = 0; i < data.size(); i++)
        {
            for (int j = 1; j < points; j++)
            {
                range.yMax = max(range.yMax, data[i].data[j]);
                range.yMin = min(range.yMin, data[i].data[j]);
            }
        }
        if (range.yMax - range.yMin < 2e-6)
        {
            range.yMax += 1e-6;
            range.yMin -= 1e-6;
        }
    }

    if (isDiscrete)
    {
        if (range.yMin > 0) range.yMin = 0;
        if (range.yMax < 0) range.yMax = 0;
    }

    int om ;            //������
    double step;        //�̶ȼ��
    double pos;         //�̶�����λ��
    stringstream str;   //�̶���ֵ������

    //��������̶���ֵ������
    double verNumW = 0;
    if (verMarkEnabled && verNumberEnabled)
    {
        om = round(log10(range.yMax - range.yMin) - 1);     //������
        step = pow(10, om);                     //��ע���
        //�������Ŀ̶���ֵ�����Ϊ������
        for (int y = round(range.yMin / step); y <= round(range.yMax / step); y++)
        {
            str << fixed << setprecision(-om > 0 ? -om : 0) << y * step;
            verNumW = max(rulerFont->stringWidth(str.str(), rulerHeight), verNumW);
            str.str("");
        }
    }

    //�������̶���ֵ������
    double horNumW = (horMarkEnabled && horNumberEnabled) ? rulerHeight : 0;

    //����̶��߿��
    double verSclW = verMarkEnabled ? 15 : 0;
    double horSclW = horMarkEnabled ? 15 : 0;

    //�������ֱ�ǩ���
    double titleW = title.text ? 1.5 * title.height : 0;
    double verNameW = verName.text ? 1.5 * verName.height : 0;
    double horNameW = horName.text ? 1.5 * horName.height : 0;

    //��������������
    double outXMin = max(25.0, 0.05 * image.getWidth());
    double outXMax = image.getWidth() - outXMin;
    double outYMin = max(25.0, 0.05 * image.getHeight());
    double outYMax = image.getHeight() - outYMin;

    //���ñ���������
    double rulerXMin = outXMin + verNameW + verNumW + verSclW;
    double rulerXMax = outXMax;
    double rulerYMin = outYMin + horNameW + horNumW + horSclW;
    double rulerYMax = outYMax - titleW;

    //���û����������
    double plotXMin = rulerXMin + 0.05 * (rulerXMax - rulerXMin);
    double plotXMax = rulerXMax - 0.05 * (rulerXMax - rulerXMin);
    double plotYMin = rulerYMin + 0.05 * (rulerYMax - rulerYMin);
    double plotYMax = rulerYMax - 0.05 * (rulerYMax - rulerYMin);

    //��������ֵ������λ�õ�ӳ��(x' = s * x + d)
    double ys, yd, xs, xd;
    ys = (plotYMax - plotYMin) / (range.yMax - range.yMin);
    yd = -ys * range.yMin + plotYMin;
    xs = (plotXMax - plotXMin) / (xMax - xMin);
    xd = -xs * xMin + plotXMin;

    //׼������
    Capsule* cap;       //ֱ�߶���
    Circle* cir;        //Բ����
    Figure::Attribute rulerAttr = { rulerColor, 0, -1 };
    image.setBackgroundColor({ 0, 0, 0, 0 });
    image.setDrawZone(outXMin, outYMin, outXMax, outYMax);

    //��������
    cap = new Capsule({ rulerXMin - 10, rulerYMin }, { rulerXMax + 10, rulerYMin }, 1, rulerAttr);
    image.draw(*cap);
    delete cap;
    cap = new Capsule({ rulerXMin, rulerYMin - 10 }, { rulerXMin, rulerYMax + 10 }, 1, rulerAttr);
    image.draw(*cap);
    delete cap;

    //��ֵ��ע
    if (verMarkEnabled)
    {
        om = round(log10(range.yMax - range.yMin) - 1);     //������
        step = pow(10, om);                     //��ע���
        for (int y = floor(range.yMin / step); y <= ceil(range.yMax / step); y++)
        {
            pos = ys * y * step + yd;           //����̶ȵ�ͼ������
            if (pos > rulerYMin && pos < rulerYMax)
            {
                //���ƿ̶���
                cap = new Capsule({ rulerXMin, pos }, { rulerXMin - 10, pos }, 1, rulerAttr);
                image.draw(*cap);
                delete cap;
                if (verNumberEnabled)
                {
                    //���ƿ̶���ֵ
                    str << fixed << setprecision(-om > 0 ? -om : 0) << y * step;
                    image.addText(str.str(), { rulerXMin - 15, pos }, { 1, 0.5 }, rulerHeight, 0, *rulerFont, rulerColor);
                    str.str("");
                }
            }
        }
    }
    if (horMarkEnabled)
    {
        om = round(log10(xMax - xMin) - 1);
        step = pow(10, om);
        for (int x = floor(xMin / step); x <= ceil(xMax / step); x++)
        {
            pos = xs * x * step + xd;
            if (pos > rulerXMin && pos < rulerXMax)
            {
                cap = new Capsule({ pos, rulerYMin }, { pos, rulerYMin - 10 }, 1, rulerAttr);
                image.draw(*cap);
                delete cap;
                if (horNumberEnabled)
                {
                    str << fixed << setprecision(-om > 0 ? -om : 0) << x * step;
                    image.addText(str.str(), { pos, rulerYMin - 15 }, { 0.5, 1 }, rulerHeight, 0, *rulerFont, rulerColor);
                    str.str("");
                }
            }
        }
    }

    //����ǩ
    if (title.text)
        image.addText(title.text, { width / 2.0, outYMax }, { 0.5, 1 }/*ˮƽ���У���������*/, title.height, 0, *title.font, title.color);
    if (verName.text)
        image.addText(verName.text, { outXMin, height / 2.0 }, { 0.5, 0 }/*ˮƽ���У��ײ�����*/, verName.height, -90, *verName.font, verName.color);
    if (horName.text)
        image.addText(horName.text, { width / 2.0, outYMin }, { 0.5, 0 }/*ˮƽ���У��ײ�����*/, horName.height, 0, *horName.font, horName.color);

    //������ͼ
    image.setDrawZone(rulerXMin + 5, rulerYMin + 5, rulerXMax - 5, rulerYMax - 5);
    for (size_t i = 0; i < data.size(); i++)
    {
        Figure::Attribute funcAttr = { data[i].color, 0, -1 };
        double lwidth = data[i].lineWidth / 2;
        if (!isDiscrete)
        {
            double x1, y1, x2, y2;
            for (int j = 0; j < points - 1; j++)
            {
                //���㺯���ϵ������Ӧ��ͼ�е�����
                x1 = plotXMin + j * (plotXMax - plotXMin) / (points - 1);
                y1 = ys * data[i].data[j] + yd;
                x2 = plotXMin + (j + 1) * (plotXMax - plotXMin) / (points - 1);
                y2 = ys * data[i].data[j + 1] + yd;
                //����һ��ֱ��
                cap = new Capsule({ x1, y1 }, { x2, y2 }, lwidth, funcAttr);
                image.draw(*cap);
                delete cap;
            }
        }
        else
        {
            double x, y;
            for (int j = 0; j < points; j++)
            {
                //���㺯���ϵ������Ӧ��ͼ�е�����
                x = plotXMin + j * (plotXMax - plotXMin) / (points - 1);
                y = ys * data[i].data[j] + yd;
                //����һ����ɢ��
                cap = new Capsule({ x, y }, { x, yd }, lwidth, funcAttr);
                cir = new Circle({ x, y }, 2 * lwidth, funcAttr);
                image.draw(*cap);
                image.draw(*cir);
                delete cap;
                delete cir;
            }
        }
    }

    /*int sparse = 1000;
    int size = 30;
    Color colorList[] = {
        0xef2b35,
        0x00f0d3,
        0xffef00,
        0xff2bd8,
        0x832dff,
        0x1c66ff,
        0x43ff36,
        0xff9804,
    };
    Rectangle* rec;
    for (size_t i = 0; i < data.size(); i++)
    {
        for (int j = 1; j < points; j++)
        {
            x1 = plotXMin + j * (plotXMax - plotXMin) / (points - 1);
            y1 = ys * data[i].data[j] + yd;
            double x = rand() % 10 - 5;
            double y = rand() % sparse - sparse / 2;
            double s = abs(y) / 15 + rand() % 20;
            rec = new Rectangle(
                { x1 + x, y1 + y },
                { s, s }, 0, { Color(colorList[rand() % 8], 0.5), 0, -1 });
            image.draw(*rec);
            delete rec;
        }
    }*/
}

//������
Image Plot::plot(double xMin, double xMax, int points, vector<Continuous> funcs)
{
    //����ɢ������
    vector<Discrete> disc(funcs.size());
    for (size_t i = 0; i < funcs.size(); i++)
    {
        double* data = new double[points];
        for (int j = 0; j < points; j++)
        {
            data[j] = funcs[i].func(xMin + (double)(xMax - xMin) * j / (points - 1));
        }
        disc[i] = { data, funcs[i].color, funcs[i].lineWidth };
    }

    //��������ͼ
    Image image(width, height);
    plotRaw(image, disc, points, xMin, xMax, false);
    for (size_t i = 0; i < disc.size(); i++)
    {
        delete[] disc[i].data;
    }
    return image;
}

//����ɢ����
Image Plot::plot(int points, vector<Discrete> data)
{
    Image image(width, height);
    plotRaw(image, data, points, 0, points, true);
    return image;
}
