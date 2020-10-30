#pragma once

#include <complex>
#include "DTFunction.h"
#include "Plot/Plot.h"

class CTDomain;
class DFDomain;
class CFDomain;

class DTDomain : public DTFunction<std::complex<double>>
{
public:
    //插值出连续时域信号
    CTDomain interpolate(int num = 4097);

    //离散傅里叶变换
    DFDomain DFT();

    //离散时间傅里叶变换
    CFDomain DTFT(int num = 4097);

    //绘制离散时间信号幅值
    Image plotAmplitude(Plot& plot);

    //绘制离散时间信号辐角
    Image plotPhase(Plot& plot);
};

#include "CTDomain.h"
#include "DFDomain.h"
#include "CFDomain.h"

