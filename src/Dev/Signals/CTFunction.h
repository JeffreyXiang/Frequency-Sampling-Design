#pragma once

#include <vector>
#include <functional>

template <typename T>
class DTFunction;

template <typename T>
class CTFunction
{
public:
    typedef struct
    {
        T value;
        double index;
    }dataPoint;

protected:
    std::vector<dataPoint>* data;

public:
    //基本函数
    CTFunction() : data(new std::vector<dataPoint>) {}
    CTFunction(std::vector<dataPoint>& data) : data(&data) {}
    CTFunction(CTFunction& other) : data(new std::vector<dataPoint>) { *data = *other.data; }
    CTFunction(CTFunction&& other) : data(other.data) { other.data = NULL; }
    ~CTFunction() { delete data; }

    CTFunction& operator=(CTFunction& other) { *data = *other.data; return *this; }
    CTFunction& operator=(CTFunction&& other) { delete data; data = other.data; other.data = NULL; return *this; }

    //操作数据列
    void setData(std::vector<dataPoint>& data) { this->data = &data; }
    std::vector<dataPoint>& getData() { return *data; }

    //获取单个数据
    T getValue(double idx);

    //末尾添加数据
    void append(dataPoint value) { data->push_back(value); }

    //从函数构造
    void fromFunction(std::function<T(double)> func, double xMin, double xMax, int num = 4097);

    //采样
    DTFunction<T> sample(double Fs);
};

#include "CTFunction.inl"
#include "DTFunction.h"

