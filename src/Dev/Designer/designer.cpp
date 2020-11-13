#include "Designer.h"

//设置目标截止频率
void Designer::targetLowPass(double cutoff)
{
    //构造连续频域
    cutOffFrequency = cutoff;
    auto func = [cutoff](double x) {return x < cutoff ? 1 : 0;};
    cfd.fromFunction(func, 0, 2 * PI);
}

//设置点数
void Designer::setLength(int num)
{
    //构造离散频率
    dfd = cfd.sample(num);
    opdfd = dfd;
}

//设置过渡带
void Designer::setTransZone(std::vector<double> value)
{
    //构造带过渡带的离散频域
    transZoneValue = value;
    opdfd = dfd;
    transZoneBegin = ceil(cutOffFrequency / (2 * PI) * opdfd.getData().size() - value.size() / 2.0);
    transZoneEnd = transZoneBegin + value.size() - 1;
    for (int i = transZoneBegin; i <= transZoneEnd; i++)
        opdfd.getData()[i] = value[i - transZoneBegin];
    opdfd.linearPhase();
}

//频率采样设计
void Designer::frequencySampling()
{
    cfd = opdfd.interpolate();
    dtd = opdfd.IDFT();
}

void Designer::calcParameters()
{
    //计算通带肩峰
    overshotHigh.value = 1;
    int lim = (transZoneBegin - 1) * cfd.getData().size() / opdfd.getData().size();
    for (size_t i = 0; i < lim; i++)
    {
        if (abs(cfd.getData()[i].value) > abs(overshotHigh.value))
            overshotHigh = cfd.getData()[i];
    }

    //计算阻带过冲
    overshotLow.value = 0;
    lim = (transZoneEnd + 1) * cfd.getData().size() / opdfd.getData().size();
    for (size_t i = lim; i <= cfd.getData().size() / 2; i++)
    {
        if (abs(cfd.getData()[i].value) > abs(overshotLow.value))
            overshotLow = cfd.getData()[i];
    }
}

//计算梯度
std::vector<double>& Designer::calcGradient()
{
    grad.resize(transZoneValue.size());
    double cen, low;
    DFDomain ldfd;
    CFDomain lcfd;

    //计算中心点阻带过冲
    ldfd = dfd;
    for (int i = transZoneBegin; i <= transZoneEnd; i++)
        ldfd.getData()[i] = transZoneValue[i - transZoneBegin];
    ldfd.linearPhase();
    lcfd = ldfd.interpolate();
    int lim = (transZoneEnd + 1) * lcfd.getData().size() / ldfd.getData().size();
    cen = 0;
    for (size_t i = lim; i <= lcfd.getData().size() / 2; i++)
    {
        if (abs(lcfd.getData()[i].value) > cen)
            cen = abs(lcfd.getData()[i].value);
    }

    //计算各轴阻带过冲变化
    for (size_t i = 0; i < grad.size(); i++)
    {
        for (int j = transZoneBegin; j <= transZoneEnd; j++)
            ldfd.getData()[j] = transZoneValue[j - transZoneBegin];
        ldfd.getData()[i + transZoneBegin] += 1e-6;
        ldfd.linearPhase();

        lcfd = ldfd.interpolate();
        low = 0;
        for (size_t i = lim; i <= lcfd.getData().size() / 2; i++)
        {
            if (abs(lcfd.getData()[i].value) > low)
                low = abs(lcfd.getData()[i].value);
        }
        grad[i] = (low - cen) * 1e6;
    }

    return grad;
}

double Designer::gradLength()
{
    double res = 0;
    for (size_t i = 0; i < grad.size(); i++)
        res += grad[i] * grad[i];
    return sqrt(res);
}

void Designer::gradDescOptimize()
{
    int count = 0;
    calcGradient();
    while (count <= 100)
    {
        for (size_t i = 0; i < grad.size(); i++)
        {
            transZoneValue[i] -= 1e-2 * grad[i];
            cout << transZoneValue[i] << "  ";
        }
        cout << endl;
        calcGradient();
        count++;
    }
    for (int i = transZoneBegin; i <= transZoneEnd; i++)
        opdfd.getData()[i] = transZoneValue[i - transZoneBegin];
    opdfd.linearPhase();
    frequencySampling();
    calcParameters();
}