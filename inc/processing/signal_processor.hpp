#ifndef SIGNAL_PROCESSOR_HPP
#define SIGNAL_PROCESSOR_HPP

#include "../audio/engine.hpp"
#include <vector>
#include <array>
#include <algorithm>

class SignalProcessor {
public:
    SignalProcessor();
    void Accumulate();
    std::array<double, 2400> GetFFTBuffer();
    bool isFull();

private:
    std::vector<double> samples;

};


#endif