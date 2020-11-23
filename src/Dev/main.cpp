#include "Signals/Signals.h"
#include "Designer/Designer.h"
#include "Plot.h"
#include <complex>

int main()
{
    Plot plot;
    Font font("../font/DengXian_ASCII_128x.bin", 128);
    Designer designer;

    plot.setSize(1500, 1000);
    plot.setDefaultFont(font);
    plot.setRuler(0x000000, 24);
    plot.setHorName("Time(s)", 0x000000, 32);
    plot.setVerName("Amplitude(V)", 0x000000, 32);
    plot.setTitle("Signal", 0x000000, 48);
    
    designer.targetLowPass(0.5 * PI);
    designer.setLength(64);
    designer.setTransZone({ 0.5 });
    designer.frequencySampling();
    designer.calcParameters();
    designer.gradDescOptimize();

    cout << "Overshot High: " << 20 * log10(abs(designer.getOvershotHigh().value)) << "dB" << endl
         << "Overshot Low: " << 20 * log10(abs(designer.getOvershotLow().value)) << "dB" << endl;

    plot.setAutoRangeEnabled(false);
    plot.setRange(-100, 0);
    designer.frequencyDomain().plotAmplitude(plot).saveBMP("../data/output_AF.bmp");
    plot.setAutoRangeEnabled(true);
    designer.frequencyDomain().plotPhase(plot).saveBMP("../data/output_PF.bmp");
    designer.timeDomain().plotAmplitude(plot).saveBMP("../data/output_AT.bmp");
    designer.timeDomain().plotPhase(plot).saveBMP("../data/output_PT.bmp");
}
