#include "DFDomain.h"

void DFDomain::linearPhase()
{
    int N = data->size();
    double phase;
    for (int i = 0; i < N; i++)
    {
        if (i < N / 2.0)
        {
            phase = -PI / N * (N - 1) * i;
        }
        else if (i > N / 2.0)
        {
            phase = PI / N * (N - 1) * (N - i);
        }
        else
        {
            phase = 0;
        }
        data->at(i) = std::polar(abs(data->at(i > N / 2 ? N - i : i)), phase);
    }
}

CFDomain DFDomain::interpolate(int num)
{
    CFDomain cfd;
    double omega;
    double temp;
    double sinc;
    int N = data->size();
    std::complex<double> sum;
    for (int i = 0; i < num; i++)
    {
        omega = (double)i / num * 2 * PI;
        sum = 0;
        for (int j = 0; j < N; j++)
        {
            temp = sin(omega / 2 - (j * PI / N));
            sinc = abs(temp) < 1e-6 ? (j % 2) ? -1 : 1 : sin(N * omega / 2) / temp / N;
            sum += data->at(j) * std::polar(1.0, -j * PI / N) * sinc;
        }
        sum *= std::polar(1.0, -(N - 1) * omega / 2);
        cfd.append({ sum, omega });
    }
    cfd.append({ cfd.getData().front().value, 2 * PI });
    return cfd;
}

DTDomain DFDomain::IDFT()
{
    DTDomain dtd;
    std::complex<double> sum;
    int N = data->size();
    for (int i = 0; i < N; i++)
    {
        sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum += data->at(j) * std::polar(1.0, 2 * PI / N * i * j);
        }
        sum /= N;
        dtd.append(sum);
    }
    return dtd;
}

Image DFDomain::plotAmplitude(Plot& plot)
{
    plot.setHorName("Index(1)");
    plot.setVerName("Amplitude(1)");
    plot.setTitle("D-F Amplitude");

    double* buffer = new double[data->size()];
    for (int i = 0; i < data->size(); i++)
        buffer[i] = abs(data->at(i));

    Image res = plot.plot(data->size(), {
        { buffer, 0x007fbf, 3 },
        });

    delete[] buffer;

    return res;
}

Image DFDomain::plotPhase(Plot& plot)
{
    plot.setHorName("Index(1)");
    plot.setVerName("Phase(rad)");
    plot.setTitle("D-F Phase");

    double* buffer = new double[data->size()];
    for (int i = 0; i < data->size(); i++)
        buffer[i] = arg(data->at(i));

    Image res = plot.plot(data->size(), {
        { buffer, 0x007fbf, 3 },
        });

    delete[] buffer;

    return res;
}
