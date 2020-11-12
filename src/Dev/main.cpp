#include "DFDomain.h"
#include "DTDomain.h"
#include "Plot.h"
#include <complex>

int main()
{
    Plot App;
    Font font("../Data/DengXian_ASCII_128x.bin", 128);

    App.setSize(1500, 1000);
    App.setDefaultFont(font);
    App.setRuler(0x000000, 24);
    App.setHorName("Time(s)", 0x000000, 32);
    App.setVerName("Amplitude(V)", 0x000000, 32);
    App.setTitle("Signal", 0x000000, 48);

    auto func = [](double x) {return x < 0.5 * PI ? 1 : 0;};
    
    CTDomain CTSignal;
    DTDomain DTSignal;
    CFDomain CFSignal;
    DFDomain DFSignal;
    CFSignal.fromFunction(func, 0, 2 * PI);
    DFSignal = CFSignal.sample(64);
    DFSignal.getData()[15] = 0.9;
    DFSignal.getData()[16] = 0.5;
    DFSignal.getData()[17] = 0.1;
    DFSignal.linearPhase();
    CFSignal = DFSignal.interpolate();
    DTSignal = DFSignal.IDFT();
    //CFSignal = DTSignal.DTFT();

    App.setAutoRangeEnabled(false);
    App.setRange(-100, 0);
    CFSignal.plotAmplitude(App).saveBMP("../data/output_AF.bmp");
    App.setAutoRangeEnabled(true);
    CFSignal.plotPhase(App).saveBMP("../data/output_PF.bmp");
    DTSignal.plotAmplitude(App).saveBMP("../data/output_AT.bmp");
    DTSignal.plotPhase(App).saveBMP("../data/output_PT.bmp");
}
