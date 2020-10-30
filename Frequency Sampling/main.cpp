#include "DFDomain.h"
#include "DTDomain.h"
#include "Plot/Plot.h"
#include <complex>

int main()
{
    Plot App;
    Font font("font/DengXian_ASCII_128x.bin", 128);

    App.setSize(1500, 1000);
    App.setDefaultFont(font);
    App.setRuler(0x000000, 24);
    App.setHorName("Time(s)", 0x000000, 32);
    App.setVerName("Amplitude(V)", 0x000000, 32);
    App.setTitle("Signal", 0x000000, 48);

    //abs(x - PI) > 0.6 * PI ? 1 : abs(x - PI) > 0.4 * PI ? (abs(x - PI) - (0.4 * PI)) / 0.2 / PI : 0
    auto func = [](double x) {return x < 0.5 * PI ? 1 : 0;};
    
    CTDomain CTSignal;
    DTDomain DTSignal;
    CFDomain CFSignal;
    DFDomain DFSignal;
    CFSignal.fromFunction(func, 0, 2 * PI);
    DFSignal = CFSignal.sample(64);
    DFSignal.getData()[15] = 0.75;
    DFSignal.getData()[16] = 0.25;
    DFSignal.linearPhase();
    CFSignal = DFSignal.interpolate();
    DTSignal = DFSignal.IDFT();
    //CFSignal = DTSignal.DTFT();

    CFSignal.plotAmplitude(App).saveBMP("output_AF.bmp");
    CFSignal.plotPhase(App).saveBMP("output_PF.bmp");
    DTSignal.plotAmplitude(App).saveBMP("output_AT.bmp");
    DTSignal.plotPhase(App).saveBMP("output_PT.bmp");
}
