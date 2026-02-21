#include "../inc/visualizer.hpp"
#include "../inc/engine.hpp" // to call singleton (implement later)

#include <iostream>

void RenderEqualizer::Display() {
    std::cout << "Value of x: " << x << "\n";
}

int main() {
    RenderEqualizer vis;

    vis.Display();

    return 0;
}