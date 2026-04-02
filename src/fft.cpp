#include "../inc/math/fft.hpp"

FFTEngine::FFTEngine()
{
    n = 2400; // input sample (real)
    np = n / 2 + 1; // retursning sample size (real + complex)
    align = sizeof(Complex);

    f.Allocate(n, align);
    F.Allocate(np, align);

    // build plan once 
    ForwardPlan = new fftwpp::rcfft1d(n, f, F);
}

FFTEngine::~FFTEngine()
{
    delete ForwardPlan;
}

std::vector<double> FFTEngine::Run(std::array<double, 2400>& audioBuffer)
{
    std::vector<double> magnitudes;

    for(size_t i = 0; i < audioBuffer.size(); ++i)
    {
        f[i] = audioBuffer[i];
    }

    // perform fft
    ForwardPlan->fft(f, F);

    magnitudes.reserve(1201);
    for(size_t i = 0; i < F.Size(); ++i)
    {
        magnitudes.push_back(std::abs(F[i]));
    }

    return magnitudes;
}