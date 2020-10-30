#pragma once

#include <complex>
#include "CTFunction.h"
#include "Plot/Plot.h"

class DFDomain;

class CFDomain : public CTFunction<std::complex<double>>
{
public:
    //将连续频域数据采样成离散频域数据
    DFDomain sample(int num);

    //绘制连续频域数据的幅值
    Image plotAmplitude(Plot& plot);

    //绘制连续频域数据的辐角
    Image plotPhase(Plot& plot);
};

#include "DFDomain.h"
