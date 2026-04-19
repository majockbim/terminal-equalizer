#pragma once

#include "../processing/signal_processor.hpp"
#include "fftw3.h"

class FFTEngine {
private:
    int n;
    int np; 

    double *f;
    fftw_complex *F;
    fftw_plan ForwardPlan;

public:
    FFTEngine();
    ~FFTEngine();

    std::vector<double> Run(std::array<double, 2400>& audioBuffer);
};