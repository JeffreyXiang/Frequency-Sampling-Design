#pragma once

#include <complex>
#include "CTFunction.h"
#include "Plot/Plot.h"

class DFDomain;

class CFDomain : public CTFunction<std::complex<double>>
{
public:
    DFDomain sample(int num);

    Image plotAmplitude(Plot& plot);
    Image plotPhase(Plot& plot);
};

#include "DFDomain.h"
