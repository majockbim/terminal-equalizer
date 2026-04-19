pet project written in C++17, using the MinGW compiler (GCC version 15.2.0)

## high level analysis:
* `main.cpp`: the entry point of the application. Initializes and calls audio engine and visualizer. <br>
* `engine.cpp`: the data pipline. Responsible for capturing the audio levels and samples for the visualizer. <br>
* `signal_processor.cpp`: the FIFO queue that is responsible for processing the audio samples for FFT. <br>
* `fft.cpp`: responsible for performing all the math, including fast fourier transforms and decibel conversions. <br>
* `visualizer.cpp`: to display the audio levels in a graphical way (spectral visualizer). <br>

## prereqs
* **MSYS2 / MinGW-w64**
* **CMake**
* **FFTW3** library installed via MSYS2

##  build and setup
run these commands to compile the project:

```powershell
# compile the project
cmake -S . -B output -G "MinGW Makefiles"
cmake --build output

# run (assuming path is terminal-equalizer)
.\output\terminal-equalizer.exe
```

## References & Libraries

**Documentation & Readings:** <br>
[WASAPI: IAudioEndpointVolume](https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudioendpointvolume) <br>
[WASAPI: audioclient.h](https://learn.microsoft.com/en-us/windows/win32/api/audioclient/) <br>
[WASAPI: IAudioClient::Initialize](https://learn.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-initialize) [4] <br>
[Fast Fourier Transform (Wiki)](https://en.wikipedia.org/wiki/Fast_Fourier_transform) <br>

**Third-Party Libraries**: <br>
[FFTW (org)](https://www.fftw.org/) <br>
[FFTW (GitHub)](https://github.com/FFTW/fftw3)
