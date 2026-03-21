#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "../audio/engine.hpp"

#include <vector>
#include <string>

class RenderEqualizer {
public:
    void Display(); // general volume level
    void DisplayBuffer(); // print float values of buffer

private:
    int x = 0;

    std::vector<std::string> levels = 
    {   "#", 
        "##",
        "###",
        "####", 
        "#####", 
        "######", 
        "#######", 
        "########", 
        "#########", 
        "##########"
    };
};

#endif