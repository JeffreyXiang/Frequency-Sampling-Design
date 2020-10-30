#include "CFDomain.h"

DFDomain CFDomain::sample(int num)
{
    DFDomain dfd;
    dfd.setSamplingRate(num / (2 * PI));
    double omega;
    for (int i = 0; i < num; i++)
    {
        omega = (double)i / num * 2 * PI;
        dfd.append(getValue(omega));
    }
    return dfd;
}

Image CFDomain::plotAmplitude(Plot& plot)
{
    plot.setHorName("Frequency(rad/s)");
    plot.setVerName("Amplitude(1)");
    plot.setTitle("C-F Amplitude");

    return plot.plot(data->front().index, data->back().index, 4097, {
        { [this](double t) {return abs(this->getValue(t));}, 0x007fbf, 3 },
        });
}

Image CFDomain::plotPhase(Plot& plot)
{
    plot.setHorName("Frequency(rad/s)");
    plot.setVerName("Phase(rad)");
    plot.setTitle("C-F Phase");

    return plot.plot(data->front().index, data->back().index, 4097, {
        { [this](double t) {return arg(this->getValue(t));}, 0x007fbf, 3 },
        });
}
