/*
    Terminal Equalizer - A real-time command line audio visualizer
    Copyright (C) 2026 Majock Bim

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../inc/main.h"
#include <consoleapi3.h>
#include <csignal>
#include <atomic>
#include <thread>

#if defined(_MSC_VER) && defined(__clang__)
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "libfftw3-3.lib")
#pragma comment(lib, "user32.lib")
#endif

#define AsciiRgb(k, r, g, b) "\033[" #k ";2;" #r ";" #g ";" #b "m"

std::atomic<bool> keepRunning(true);

void signalHandler(_In_ int signum) {
    keepRunning = false;
    AudioEngine::Get().Stop();
}

int __cdecl main(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitleA("Initializing...");

    HINSTANCE hInstance = GetModuleHandle(NULL);
    HWND ConsoleWindow = GetConsoleWindow();

    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(100)); // 100 = icon.ico
    if (hIcon) {
        SendMessage(ConsoleWindow, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(ConsoleWindow, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
    
    // register signal handler for clean exit
    signal(SIGINT, signalHandler);

    SignalProcessor signal;
    FFTEngine fft;
    RenderEqualizer equalizer;

    std::mutex magMutex;
    std::vector<double> sharedMagnitudes;

    std::thread t1([&]() {
        CoInitialize(NULL);
        while (keepRunning) {
            signal.Accumulate();
            if (signal.isFull()) {
                auto pass = signal.GetFFTBuffer();
                auto magnitudes = fft.Run(pass);
                std::lock_guard<std::mutex> lock(magMutex);
                sharedMagnitudes = magnitudes;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        CoUninitialize();
    });

    std::thread t2([&]() {
        CoInitialize(NULL);
        int sampleRate = AudioEngine::Get().GetSampleRate();
        JsonFileFinder jsonFileFinder;
        JsonFileReader jsonFileReader;

        SetConsoleTitleA("Choose Theme");
        int findResult = jsonFileFinder.FindJsonFiles(&jsonFileReader);
        if (findResult == 1) {
            std::cout << " * Can't find Themes: Resetting to default settings * " << std::endl;
        }
        SetConsoleTitleA("...");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        SetConsoleTitleA("SPECTRUM");
        equalizer.EnableVisualizer(sharedMagnitudes, magMutex, sampleRate, jsonFileReader);
        CoUninitialize();
    });

    t1.join();
    t2.join();

    return 0;
}
