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

	//����ɢʱ�丵��Ҷ�任	x[n]=\frac{1}{2\pi} \int_{2\pi}X(e^{j\omega})e^{j\omega n}d\omega
	DTDomain IDTFT();
};

#include "DFDomain.h"
