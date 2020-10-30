#pragma once

#include <complex>
#include "DTFunction.h"
#include "Plot/Plot.h"

class CFDomain;
class DTDomain;

class DFDomain : public DTFunction<std::complex<double>>
{
public:
    void linearPhase();
    CFDomain interpolate(int num = 4096);
    DTDomain IDFT();
    
    Image plotAmplitude(Plot& plot);
    Image plotPhase(Plot& plot);
};

#include "CFDomain.h"
#include "DTDomain.h"
