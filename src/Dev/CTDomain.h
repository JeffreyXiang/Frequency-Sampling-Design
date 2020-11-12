#pragma once

#include <complex>
#include "CTFunction.h"
#include "Plot/Plot.h"

class DTDomain;

class CTDomain : public CTFunction<std::complex<double>>
{
public:
    //将连续时域信号采样成离散时域信号
    DTDomain sample(double Fs);

    //连续时域信号绘制幅值
    Image plotAmplitude(Plot& plot);

    //连续时域信号绘制辐角
    Image plotPhase(Plot& plot);
};

#include "DTDomain.h"
