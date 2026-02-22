#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "../inc/engine.hpp"

#include <vector>
#include <string>

class RenderEqualizer {
public:
    void Display();
    
    /* To implement:
    function to call functions of singleton

    */

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