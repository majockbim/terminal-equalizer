#include <iostream>
#include <cstddef>
#include "fftw++.h"
#include "../inc/ui/visualizer.hpp"

// FFTW++ global variables
namespace parallel { void Threshold(size_t threads) {} }
size_t threshold = 1024; 
namespace utils { size_t ALIGNMENT = 32; }

int main() {
    
    RenderEqualizer o;
    o.Display();

    return 0;
}