#pragma once

#include "Signals/Signals.h"

class Designer
{
private:
    CTDomain ctd;                           //����ʱ��
    DTDomain dtd;                           //��ɢʱ��
    CFDomain cfd;                           //����Ƶ��
    DFDomain dfd;                           //��ɢƵ��
    DFDomain opdfd;                         //�����ɵ���ɢƵ��

    double cutOffFrequency;                 //��ֹƵ��
    int transZoneBegin;                     //���ɴ���ʼ
    int transZoneEnd;                       //���ɴ�����
    std::vector<double> transZoneValue;     //���ɴ�����
    std::vector<double> grad;               //�ݶ�
    CFDomain::dataPoint overshotHigh;       //ͨ�����
    CFDomain::dataPoint overshotLow;        //�������

private:
    double gradLength();

public:
    Designer() : cutOffFrequency(0), transZoneBegin(0), transZoneEnd(0) {}
    ~Designer() {}

    DTDomain& timeDomain() { return dtd; }
    CFDomain& frequencyDomain() { return cfd; }

    //���ý�ֹƵ��
    void targetLowPass(double cutoff);

    //���õ���
    void setLength(int num);

    //���ù��ɴ�
    void setTransZone(std::vector<double> value);

    //Ƶ�ʲ���
    void frequencySampling();

    void calcParameters();
    CFDomain::dataPoint getOvershotHigh() { return overshotHigh; }
    CFDomain::dataPoint getOvershotLow() { return overshotLow; }

    double lossFunction(CFDomain& cfd);
    std::vector<double>& calcGradient();
    std::vector<double>& getGradient() { return grad; }

    void gradDescOptimize();
};