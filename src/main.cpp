#include "../inc/visualizer.hpp"
#include <iostream>

int main() {
   
    // Init visualizer object
    RenderEqualizer test;

    // continuous loop displaying data
    for(;;) {
        std::cout << test.Get() << "\n";
    }

    return 0;
}