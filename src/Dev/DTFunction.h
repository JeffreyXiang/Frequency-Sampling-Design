#pragma once

#include <vector>

# define PI           3.14159265358979323846  /* pi */

template <typename T>
class CTFunction;

template <typename T>
class DTFunction
{
protected:
    std::vector<T>* data;
    double samplingRate;

public:
    //基本函数
    DTFunction() : data(new std::vector<T>) {}
    DTFunction(std::vector<T>& data) : data(&data) {}
    DTFunction(DTFunction& other) : data(new std::vector<T>), samplingRate(other.samplingRate) { *data = *other.data; }
    DTFunction(DTFunction&& other) : data(other.data), samplingRate(other.samplingRate) { other.data = NULL; }
    ~DTFunction() { delete data; }

    DTFunction& operator=(DTFunction& other) { *data = *other.data; samplingRate = other.samplingRate; return *this; }
    DTFunction& operator=(DTFunction&& other) { delete data; data = other.data; other.data = NULL; samplingRate = other.samplingRate; return *this; }

    //操作数据列
    void setData(std::vector<T>& data) { this->data = &data; }
    std::vector<T>& getData() { return *data; }

    //操作单个数据
    void setValue(T value, int idx);
    T getValue(int idx);

    //操作采样率
    void setSamplingRate(double Fs) { samplingRate = Fs; }
    double getSamplingRate() { return samplingRate; }

    //末尾加入数据
    void append(T value) { data->push_back(value); }

    //插值
    //CTFunction<T> interpolate(int num = 4097);
};

#include "DTFunction.inl"
#include "CTFunction.h"

