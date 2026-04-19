#include "../../inc/main.h"

int main() {
    
    RenderEqualizer o;
    SignalProcessor s;
    o.Display();

    //signal_processor test
    while(true) {
        o.DisplayBuffer();
        s.Accumulate();

        if(s.isFull()) {
            auto buff = s.GetFFTBuffer();
            std::cout << "Size of buffer: " << buff.size() << "\n";
        }
    }

    return 0;
}