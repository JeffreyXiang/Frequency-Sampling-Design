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
    //��ֵ������ʱ���ź�
    CTDomain interpolate(int num = 4097);

    //��ɢ����Ҷ�任
    DFDomain DFT();

    //��ɢʱ�丵��Ҷ�任
    CFDomain DTFT(int num = 4097);

    //������ɢʱ���źŷ�ֵ
    Image plotAmplitude(Plot& plot);

    //������ɢʱ���źŷ���
    Image plotPhase(Plot& plot);
};

#include "CTDomain.h"
#include "DFDomain.h"
#include "CFDomain.h"

