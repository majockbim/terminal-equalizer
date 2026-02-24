#include "../inc/visualizer.hpp"

#include <iostream>

void RenderEqualizer::Display() {
    int level;
    float temp;
    float vol;
    while(true) {
        vol = AudioEngine::Get().GenVolLevel();
        temp = vol * 9.0;
        level = (int)temp;

        if(vol == 0.0) std::cout << "\r" << " " << "          " << std::flush;
        else std::cout << "\r" << levels[level] << "          " << std::flush;
    }
}