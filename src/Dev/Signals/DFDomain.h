#pragma once

#include <complex>
#include "DTFunction.h"
#include "Plot/Plot.h"

class CFDomain;
class DTDomain;

class DFDomain : public DTFunction<std::complex<double>>
{
public:
    //离散频域数据线性相位规范化
    void linearPhase();

    //插值产生连续频域数据
    CFDomain interpolate(int num = 4096);

    //离散傅里叶逆变换
    DTDomain IDFT();
    
    //离散频域数据绘制幅值
    Image plotAmplitude(Plot& plot);

    //离散频域数据绘制辐角
    Image plotPhase(Plot& plot);
};

#include "CFDomain.h"
#include "DTDomain.h"
