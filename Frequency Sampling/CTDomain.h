#pragma once

#include <complex>
#include "CTFunction.h"
#include "Plot/Plot.h"

class DTDomain;

class CTDomain : public CTFunction<std::complex<double>>
{
public:
    //������ʱ���źŲ�������ɢʱ���ź�
    DTDomain sample(double Fs);

    //����ʱ���źŻ��Ʒ�ֵ
    Image plotAmplitude(Plot& plot);

    //����ʱ���źŻ��Ʒ���
    Image plotPhase(Plot& plot);
};

#include "DTDomain.h"
