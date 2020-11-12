#include "DTFunction.h"
#include <iostream>

template <typename T>
void DTFunction<T>::setValue(T value, int idx)
{
    if (idx < 0 || idx >= data->size())
    {
        std::cout << "ERROR | void DTFunction::setValue(double, int) : Index out of range.\n";
        throw 1;
    }
    data->at(idx) = value;
}

template <typename T>
T DTFunction<T>::getValue(int idx)
{
    if (idx < 0 || idx >= data->size())
    {
        std::cout << "ERROR | double DTFunction::getValue(int) : Index out of range.\n";
        return 0;
    }
    return data->at(idx);
}

//double sinc(double x)
//{
//    return (abs(x) < 1e-6 ? 1 : sin(x * PI) / (x * PI));
//}
//
//template<typename T>
//CTFunction<T> DTFunction<T>::interpolate(int num)
//{
//    CTFunction<T> dt;
//    double idx;
//    double t;
//    T sum;
//    for (int i = 0; i < num; i++)
//    {
//        idx = (double)i / (num - 1) * (data->size() - 1) / samplingRate;
//        sum = 0;
//        for (size_t j = 0; j < data->size(); j++)
//        {
//            t = j - idx * samplingRate;
//            sum += data->at(j) * sinc(t);
//        }
//        dt.append({ sum, idx });
//    }
//    return dt;
//}
