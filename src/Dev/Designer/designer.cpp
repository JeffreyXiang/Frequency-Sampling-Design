#include "Designer.h"

void Designer::targetLowPass(double cutoff)
{
    cutOffFrequency = cutoff;
    auto func = [cutoff](double x) {return x < cutoff ? 1 : 0;};
    cfd.fromFunction(func, 0, 2 * PI);
}

void Designer::setLength(int num)
{
    dfd = cfd.sample(num);
    opdfd = dfd;
}

void Designer::setTransZone(std::vector<double> value)
{
    opdfd = dfd;
    transZoneBegin = ceil(cutOffFrequency / (2 * PI) * opdfd.getData().size() - value.size() / 2.0);
    transZoneEnd = floor(cutOffFrequency / (2 * PI) * opdfd.getData().size() + value.size() / 2.0);
    for (int i = transZoneBegin; i <= transZoneEnd; i++)
        opdfd.getData()[i] = value[i - transZoneBegin];
    opdfd.linearPhase();
}

void Designer::frequencySampling()
{
    cfd = opdfd.interpolate();
    dtd = opdfd.IDFT();
}
