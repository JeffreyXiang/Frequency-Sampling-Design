#include "CTFunction.h"
#include <iostream>

template <typename T>
T CTFunction<T>::getValue(double idx)
{
    if (idx < data->front().index || idx > data->back().index)
    {
        std::cout << "ERROR | double CTFunction::getValue(double) : Index out of range.\n";
        return 0;
    }
    for (auto itr = data->begin() + 1; itr < data->end(); itr++)
    {
        if (itr->index >= idx)
            return (itr->index - idx) / (itr->index - (itr - 1)->index) * (itr - 1)->value
                 + (idx - (itr - 1)->index) / (itr->index - (itr - 1)->index) * itr->value;
    }
}

template <typename T>
void CTFunction<T>::fromFunction(std::function<T(double)> func, double xMin, double xMax, int num)
{
    double idx;
    for (size_t i = 0; i < num; i++)
    {
        idx = (double)i / (num - 1) * (xMax - xMin) + xMin;
        data->push_back({ func(idx), idx });
    }
}

template <typename T>
DTFunction<T> CTFunction<T>::sample(double Fs)
{
    DTFunction<T> dt;
    dt.setSamplingRate(Fs);
    double idx;
    for (int i = ceil(data->front().index * Fs); i <= floor(data->back().index * Fs); i++)
    {
        idx = i / Fs;
        dt.append(getValue(idx));
    }
    return dt;
}
