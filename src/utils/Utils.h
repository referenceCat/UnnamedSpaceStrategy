//
// Created by referencecat on 26.02.24.
//

#ifndef UNTITLEDSPACESTRATEGY_UTILS_H
#define UNTITLEDSPACESTRATEGY_UTILS_H

#include <utility>
#include "chrono"
#include "iostream"
#include "sstream"
#include "iomanip"

class Utils {
public:
    template<typename Func, typename... Args>
    double measure_execution_time(Func &&func, Args &&...args) {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e6;
    }

    static std::string toString(double value, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << value;
        std::string s = stream.str();
        return s;
    }
};



#endif //UNTITLEDSPACESTRATEGY_UTILS_H
