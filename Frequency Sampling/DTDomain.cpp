#include "DTDomain.h"

double sinc(double x)
{
    return (abs(x) < 1e-6 ? 1 : sin(x * PI) / (x * PI));
}

CTDomain DTDomain::interpolate(int num)
{
    CTDomain ctd;
    double t;
    double temp;
    std::complex<double> sum;
    for (int i = 0; i < num; i++)
    {
        t = (double)i / (num - 1) * (data->size() - 1) / samplingRate;
        sum = 0;
        for (size_t j = 0; j < data->size(); j++)
        {
            temp = j - t * samplingRate;
            sum += data->at(j) * sinc(temp);
        }
        ctd.append({ sum, t });
    }
    return ctd;
}

DFDomain DTDomain::DFT()
{
    DFDomain dfd;
    std::complex<double> sum;
    int N = data->size();
    for (int i = 0; i < N; i++)
    {
        sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum += data->at(j) * std::polar(1.0, -2 * PI / N * i * j);
        }
        dfd.append(sum);
    }
    return dfd;
}

CFDomain DTDomain::DTFT(int num)
{
    CFDomain cfd;
    double omega;
    int N = data->size();
    std::complex<double> sum;
    for (int i = 0; i < num; i++)
    {
        omega = (double)i / num * 2 * PI;
        sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum += data->at(j) * std::polar(1.0, -omega * j);
        }
        cfd.append({ sum, omega });
    }
    return cfd;
}

Image DTDomain::plotAmplitude(Plot& plot)
{
    plot.setHorName("Index(1)");
    plot.setVerName("Amplitude(1)");
    plot.setTitle("D-T Amplitude");

    double* buffer = new double[data->size()];
    for (int i = 0; i < data->size(); i++)
        buffer[i] = abs(data->at(i));

    Image res = plot.plot(data->size(), {
        { buffer, 0x007fbf, 3 },
        });

    delete[] buffer;

    return res;
}

Image DTDomain::plotPhase(Plot& plot)
{
    plot.setHorName("Index(1)");
    plot.setVerName("Phase(rad)");
    plot.setTitle("D-T Phase");

    double* buffer = new double[data->size()];
    for (int i = 0; i < data->size(); i++)
        buffer[i] = arg(data->at(i));

    Image res = plot.plot(data->size(), {
        { buffer, 0x007fbf, 3 },
        });

    delete[] buffer;

    return res;
}
