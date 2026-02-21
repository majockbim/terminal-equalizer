#include "../inc/visualizer.hpp"

#include <iostream>

void RenderEqualizer::Display() {
    std::cout << "Value of x: " << x << "\n";
}

int main() {
    RenderEqualizer vis;

    vis.Display();

    return 0;
}