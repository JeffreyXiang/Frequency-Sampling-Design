#pragma once

#include <complex>
#include "CTFunction.h"
#include "Plot/Plot.h"

class DTDomain;

class CTDomain : public CTFunction<std::complex<double>>
{
public:
    DTDomain sample(double Fs);

    Image plotAmplitude(Plot& plot);
    Image plotPhase(Plot& plot);
};

#include "DTDomain.h"
