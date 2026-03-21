#include "../inc/ui/visualizer.hpp"

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

void RenderEqualizer::DisplayBuffer() {
    std::vector<float> CurrentBuffer =  AudioEngine::Get().GetCurrentBuffer(); 
    std::cout << "Size of buffer: " << CurrentBuffer.size() << "\n";

    /* Warning:
    *  Currently, buffer size is 480, and usually the program does not have enough time to
    *  print each value before the next buffer comes in, so they stay in a queue meaning the 
    *  next buffer size will be >480. This has been tested, and is just an interesting latency 'issue.'
    */
    for(int i = 0; i < CurrentBuffer.size(); ++i) {
        std::cout << i << ". " << CurrentBuffer[i] << "\n";
    }

}