#pragma once

#include <complex>
#include "DTFunction.h"
#include "Plot/Plot.h"

class CFDomain;
class DTDomain;

class DFDomain : public DTFunction<std::complex<double>>
{
public:
    //��ɢƵ������������λ�淶��
    void linearPhase();

    //��ֵ��������Ƶ������
    CFDomain interpolate(int num = 4096);

    //��ɢ����Ҷ��任
    DTDomain IDFT();
    
    //��ɢƵ�����ݻ��Ʒ�ֵ
    Image plotAmplitude(Plot& plot);

    //��ɢƵ�����ݻ��Ʒ���
    Image plotPhase(Plot& plot);
};

#include "CFDomain.h"
#include "DTDomain.h"
