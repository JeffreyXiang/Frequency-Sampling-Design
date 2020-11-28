#pragma once

#include "Signals/Signals.h"

class Designer
{
private:
    CTDomain ctd;                           //连续时域
    DTDomain dtd;                           //离散时域
    CFDomain cfd;                           //连续频域
    DFDomain dfd;                           //离散频域
    DFDomain opdfd;                         //带过渡的离散频域

    double cutOffFrequency;                 //截止频率
    int transZoneBegin;                     //过渡带开始
    int transZoneEnd;                       //过渡带结束
    std::vector<double> transZoneValue;     //过渡带数据
    std::vector<double> grad;               //梯度
    CFDomain::dataPoint overshotHigh;       //通带肩峰
    CFDomain::dataPoint overshotLow;        //阻带过冲

private:
    double gradLength();

public:
    Designer() : cutOffFrequency(0), transZoneBegin(0), transZoneEnd(0) {}
    ~Designer() {}

    DTDomain& timeDomain() { return dtd; }
    CFDomain& frequencyDomain() { return cfd; }

    //设置截止频率
    void targetLowPass(double cutoff);

    //设置点数
    void setLength(int num);

    //设置过渡带
    void setTransZone(std::vector<double> value);

    //频率采样
    void frequencySampling();

    void calcParameters();
    CFDomain::dataPoint getOvershotHigh() { return overshotHigh; }
    CFDomain::dataPoint getOvershotLow() { return overshotLow; }

    double lossFunction(CFDomain& cfd);
    std::vector<double>& calcGradient();
    std::vector<double>& getGradient() { return grad; }

    void gradDescOptimize();
};