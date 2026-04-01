#include "../inc/math/fft.hpp"

FFTEngine::FFTEngine()
{
    size_t n = 2400; // input sample (real)
    size_t np = n / 2 + 1; // returning sample size (real + complex)
    size_t align = sizeof(Complex);

    array1<double> f(n, align);
    array1<Complex> F(np, align);
}

std::array<double, 2400> FFTEngine::Run(std::array<double, 2400>& audioBuffer)
{

}