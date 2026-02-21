#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

class RenderEqualizer {
public:
    int Get() { return x; }
    void Display();
    
    /* To implement:
    function to call functions of singleton

    */

private:
    int x = 0;
};

#endif