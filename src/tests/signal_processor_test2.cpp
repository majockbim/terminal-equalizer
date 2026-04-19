#include "../../inc/main.h"

int main() {

    RenderEqualizer o;
    SignalProcessor s;

    // Get a single buffer, and loop through each index
    for(UINT32 i = 0; i < 5; i++) {

        /*
         NOTE:
        * this 'Accumulate()' function may need to
        * be called multiple times before 'GetFFTBuffer()'
        * is called, since each packet has a size of 480
        * and we are Accumulating a singular packet upon
        * each call of the function
        */
        
        s.Accumulate();
    }
    auto buff = s.GetFFTBuffer();
    std::cout << "Size of buffer: " <<  buff.size() << "\n";
    for(auto i : buff) {
        std::cout << i << "\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return 0;
}