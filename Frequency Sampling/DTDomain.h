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
    CTDomain interpolate(int num = 4097);
    DFDomain DFT();
    CFDomain DTFT(int num = 4097);

    Image plotAmplitude(Plot& plot);
    Image plotPhase(Plot& plot);
};

#include "CTDomain.h"
#include "DFDomain.h"
#include "CFDomain.h"

