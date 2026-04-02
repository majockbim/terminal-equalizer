#pragma once

#include <complex>
#include "Complex.h"
#undef I
#undef O

#include "Array.h"
#include "fftw++.h"
#include "../processing/signal_processor.hpp"

#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
// namespaces
using namespace utils;
using namespace fftwpp;
using namespace Array; 
using namespace parallel;
*/

class FFTEngine {
private:
    size_t n;
    size_t np; 
    size_t align; 

    Array::array1<double> f;
    Array::array1<Complex> F;

    fftwpp::rcfft1d* ForwardPlan = nullptr;
public:
    FFTEngine();
    ~FFTEngine();

    std::vector<double> Run(std::array<double, 2400>& audioBuffer);
};