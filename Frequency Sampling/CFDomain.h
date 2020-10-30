#pragma once

#include <complex>
#include "CTFunction.h"
#include "Plot/Plot.h"

class DFDomain;

class CFDomain : public CTFunction<std::complex<double>>
{
public:
    //������Ƶ�����ݲ�������ɢƵ������
    DFDomain sample(int num);

    //��������Ƶ�����ݵķ�ֵ
    Image plotAmplitude(Plot& plot);

    //��������Ƶ�����ݵķ���
    Image plotPhase(Plot& plot);
};

#include "DFDomain.h"
