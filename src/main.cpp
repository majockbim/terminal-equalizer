#include "../inc/main.h"

int main() {
    SignalProcessor signal;
    FFTEngine fft;
    RenderEqualizer equalizer;

    
    std::mutex magMutex;
    std::vector<double> sharedMagnitudes;

    std::thread t1([&]() {
        CoInitialize(NULL);
        while (1) {
            signal.Accumulate();
            if (signal.isFull()) {
                auto pass = signal.GetFFTBuffer();
                auto magnitudes = fft.Run(pass);
                std::lock_guard<std::mutex> lock(magMutex);
                sharedMagnitudes = magnitudes;
                // std::cout << "audio: " << sharedMagnitudes.size() << "\n";
            }
        }
        CoUninitialize();
    });

    std::thread t2([&]() {
        CoInitialize(NULL);
        int sampleRate = AudioEngine::Get().GetSampleRate();
        equalizer.EnableVisualizer(sharedMagnitudes, magMutex, sampleRate);
        CoUninitialize();
    });

    t1.join();
    t2.join();

    return 0;
}