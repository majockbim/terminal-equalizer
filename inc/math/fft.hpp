#pragma once

#include <complex>
#include "Complex.h"
#undef I
#undef O

#include "Array.h"
#include "fftw++.h"
#include "signal_processor.hpp"

#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// namespaces
using namespace utils;
using namespace fftwpp;
using namespace Array; 
using namespace parallel;

class FFTEngine {
private:
    size_t n = 2400; // input sample (real)
    size_t np = n / 2 + 1; // returning sample size (real + complex)
    size_t align = sizeof(Complex);
public:
    FFTEngine();
    ~FFTEngine();
    std::array<double, 2400> Run(std::array<double, 2400>& audioBuffer);
};