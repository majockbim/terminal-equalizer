
<div align="center">

  <img width="520" alt="spectrum - Copy" src="https://github.com/user-attachments/assets/aeecb34f-0132-4a16-acf4-084cd1a19049" />
  
  **A real-time, optimized C++ audio visualizer for the Windows console.**
  
  <!-- Badges -->
  <a href="https://github.com/majockbim/spectrum/stargazers">
    <img src="https://img.shields.io/github/stars/majockbim/spectrum?style=flat&logo=github&color=red" alt="Stars" />
  </a>
  <img src="https://img.shields.io/badge/C%2B%2B17-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white" alt="C++17" />
  <img src="https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows&logoColor=white" alt="Windows" />
  <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat" alt="PRs Welcome" />
  <a href="https://github.com/majockbim/spectrum/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/majockbim/spectrum?style=flat&color=purple" alt="License" />
  </a>

  <br>
  <br>

  <img width="1920" alt="spectrum-ezgif com-video-to-gif-converter" src="https://github.com/user-attachments/assets/199c8057-843c-4021-84d8-cbcff293bef8" />

</div>

## System Architecture
**Spectrum** captures raw system audio directly from the soundcard, processes it through a real-time DSP pipeline, and renders it to the console without screen tearing.

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'fontFamily': 'Comic Sans MS, Comic Neue, Chalkboard SE, cursive', 'lineColor': '#000000', 'primaryTextColor': '#000000', 'edgeLabelBackground':'#ffffff'}, 'flowchart': {'curve': 'basis'}}}%%
graph LR
    classDef default fill:#ffffff,stroke:#000000,stroke-width:2px,color:#000000
    
    subgraph T1 [Thread 1: Audio Capture]
        A([WASAPI]) --> B[Accumulate<br/>Audio]
        B --> C{2400<br/>Samples?}
        C -->|No| B
        C -->|Yes| D[Lock Mutex &<br/>Notify Ready]
        D -->|Reset Index| B
    end

    subgraph Shared [Shared State]
        M[(Thread-Safe<br/>readyBuffer)]
    end

    subgraph T2 [Thread 2: DSP & Render]
        E{Buffer<br/>Ready?}
        
        %% The true self loop (Mermaid will force this into a square)
        E -->|Wait| E
        
        E -->|Yes| F[Execute FFTW3<br/>& Decibel Math]
        F --> G[Group to a # of Bins]
        G --> H([Draw ASCII Bars<br/>Zero-Flicker])
        H --> I[Reset Flag]
        I -->|Loop| E
    end

    %% Data flow across threads
    D == Copy Data ==> M
    M == Read Data ==> E
```

## The DSP Engine
At the core of the visualizer is the **Discrete Fourier Transform (DFT)**, powered by the [FFTW3 C-API](https://www.fftw.org/). <br>
The engine takes a time-domain window of audio samples and transforms it into a number of distinct frequency magnitudes.

$$X_k = \sum_{n=0}^{N-1} x_n e^{-i 2\pi k n / N}$$

<img width="1251" height="300" alt="image" src="https://github.com/user-attachments/assets/8cda5963-9526-4b9e-8dd5-b2c83174b093" /> <br>

To make the output visually accurate to human hearing:
1. **Data Scrubbing:** Acts as a firewall against WASAPI driver glitches, dropping `inf`, `NaN`, and integer overflows.
2. **Decibel Conversion:** Raw amplitudes are normalized and converted to a logarithmic $\text{log}_{10}$ scale.
3. **Frequency Binning:** The pitches are averaged down into a number visual UI bins, depending on the terminal size.

## Performance
Benchmarked on an AMD Ryzen 5 7520U (2.80 GHz) at a 2400-sample window size:

| Metric | Value |
|---|---|
| CPU usage | ~0% |
| Memory footprint | 0.4 MB |

*spectrum is designed to be lightweight, it should never compete with your music.*

## Quick Start (Casuals)
For those who just want to run the visualizer without compiling:
1. Download the latest release (**v1.1.0**) from the [Releases Page](https://github.com/majockbim/spectrum/releases).
2. Extract the ZIP file to your preferred location.
3. Run `spectrum.exe`.
4. Play some music and enjoy the show!

## Getting Started
**Prerequisites**
* **Windows 10/11**
* **CMake** (v3.10+)
* **FFTW3** (Pre-compiled binaries included in `third_party/`)
* **OpenMP** (Usually included with your compiler)

## Building from Source (Contributors)
First, clone the repository:
```bash
git clone https://github.com/majockbim/spectrum
cd spectrum
```

### Option 1: g++ (MinGW-w64)
Best for those using MSYS2 or a standalone MinGW installation.
```bash
# Generate build files
cmake -B build_mingw -G "MinGW Makefiles"

# Compile
cmake --build build_mingw

# Run!
.\build_mingw\spectrum.exe
```

### Option 2: cl (MSVC Command Line)
Best for those who prefer the Microsoft C++ compiler but want to stay in the terminal. <br>
**Note**: Because this uses the Visual Studio CMake generator, you can run these commands directly in standard PowerShell.
```bash
# Generate build files (Ensure x64 architecture)
cmake -S . -B build_msvc -G "Visual Studio 17 2022" -A x64

# Compile
cmake --build build_msvc --config Release

# Run!!
.\build_msvc\Release\spectrum.exe
```

### Option 3: Visual Studio (MSVC IDE)
The easiest way for Windows developers.
1. Open Visual Studio.
2. Select **Open a local folder** and choose the `spectrum` directory.
3. Visual Studio will automatically detect `CMakeLists.txt` and configure the project.
4. Select `spectrum.exe` in the "Select Startup Item" dropdown.
5. Press **F5** to build and run!!!

## References & Libraries

**Documentation & Readings:** <br>
[WASAPI: IAudioEndpointVolume](https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudioendpointvolume) <br>
[WASAPI: audioclient.h](https://learn.microsoft.com/en-us/windows/win32/api/audioclient/) <br>
[WASAPI: IAudioClient::Initialize](https://learn.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-initialize) [4] <br>
[Fast Fourier Transform (Wiki)](https://en.wikipedia.org/wiki/Fast_Fourier_transform) <br>

**Third-Party Libraries**: <br>
[FFTW (org)](https://www.fftw.org/) <br>
[FFTW (GitHub)](https://github.com/FFTW/fftw3)

## Contributors
A massive thank you to everyone who has helped build and optimize spectrum! <br>
Check out [Contributors Hall of Fame](CONTRIBUTORS.md).
