#include "../inc/main.h"

// FFTW++ global variables
namespace parallel { void Threshold(size_t threads) {} }
size_t threshold = 1024; 
namespace utils { size_t ALIGNMENT = 32; }

int main() {
    
    RenderEqualizer o;
    SignalProcessor s;
    FFTEngine f;
    // o.Display();

    /* signal_processor test
    // Get a single buffer, and loop through each index
    for(UINT32 i = 0; i < 5; i++) {

         NOTE:
        * this 'Accumulate()' function may need to
        * be called multiple times before 'GetFFTBuffer()'
        * is called, since each packet has a size of 480
        * and we are Accumulating a singular packet upon
        * each call of the function
        
        s.Accumulate();
    }
    auto buff = s.GetFFTBuffer();
    std::cout << "Size of buffer: " <<  buff.size() << "\n";
    for(auto i : buff) {
        std::cout << i << "\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    */

    /* signal_processor test
    while(true) {
        // o.DisplayBuffer();
        s.Accumulate();

        if(s.isFull()) {
            buff = s.GetFFTBuffer();
            std::cout << "Size of buffer: " << buff.size() << "\n";
        }
    }
    */

    


    return 0;
}