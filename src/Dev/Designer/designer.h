#pragma once

#include "Signals/Signals.h"

class Designer
{
private:
    CTDomain ctd;
    DTDomain dtd;
    CFDomain cfd;
    DFDomain dfd;
    DFDomain opdfd;

    double cutOffFrequency;
    int transZoneBegin;
    int transZoneEnd;

public:
    Designer() : cutOffFrequency(0), transZoneBegin(0), transZoneEnd(0) {}
    ~Designer() {}

    DTDomain& timeDomain() { return dtd; }
    CFDomain& frequencyDomain() { return cfd; }

    void targetLowPass(double cutoff);
    void setLength(int num);
    void setTransZone(std::vector<double> value);
    void frequencySampling();
};