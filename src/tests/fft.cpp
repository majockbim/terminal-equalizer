#include "../inc/main.h"

int main() {
    
    SignalProcessor s;
    FFTEngine f;

    // FFT Test
    while(1) 
    {
        s.Accumulate();

        // 2400
        if(s.isFull())
        {
            std::cout << "full" << "\n";

            auto pass  = s.GetFFTBuffer();
            auto magnitudes  = f.Run(pass);

            std::cout << magnitudes.size() << "\n";

            for (int16_t i = 0; i < magnitudes.size(); ++i)
            {
                std::cout << i << ". " << magnitudes[i] << "\n";
            }
        }

    }

    return 0;
}