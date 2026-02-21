#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "../inc/engine.hpp"

class RenderEqualizer {
public:
    int Get() { return x; }
    void Display() {
        float vol = AudioEngine::Get().GenVolLevel();
    };
    
    /* To implement:
    function to call functions of singleton

    */

private:
    int x = 0;
};

#endif