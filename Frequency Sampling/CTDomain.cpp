#include "CTDomain.h"

DTDomain CTDomain::sample(double Fs)
{
    DTDomain dt;
    dt.setSamplingRate(Fs);
    double idx;
    for (int i = ceil(data->front().index * Fs); i <= floor(data->back().index * Fs); i++)
    {
        idx = i / Fs;
        dt.append(getValue(idx));
    }
    return dt;
}

Image CTDomain::plotAmplitude(Plot& plot)
{
    plot.setHorName("Time(s)");
    plot.setVerName("Amplitude(V)");
    plot.setTitle("C-T Amplitude");
    
    return plot.plot(data->front().index, data->back().index, 4097, {
        { [this](double t) {return abs(this->getValue(t));}, 0x007fbf, 2 },
        });
}

Image CTDomain::plotPhase(Plot& plot)
{
    plot.setHorName("Time(s)");
    plot.setVerName("Phase(rad)");
    plot.setTitle("C-T Phase");

    return plot.plot(data->front().index, data->back().index, 4097, {
        { [this](double t) {return arg(this->getValue(t));}, 0x007fbf, 2 },
        });
}
