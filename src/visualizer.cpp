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

#include "../inc/ui/visualizer.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

void RenderEqualizer::Display() {
    int level;
    float temp;
    float vol;
    while(AudioEngine::Get().IsRunning()) {
        vol = AudioEngine::Get().GenVolLevel();
        temp = vol * 9.0;
        level = (int)temp;

        if(vol == 0.0) std::cout << "\r" << " " << "          " << std::flush;
        else std::cout << "\r" << levels[level] << "          " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void RenderEqualizer::DisplayBuffer() {
    std::vector<double> CurrentBuffer =  AudioEngine::Get().GetCurrentBuffer(); 
    std::cout << "Size of buffer: " << CurrentBuffer.size() << "\n";

    for(int i = 0; i < (int)CurrentBuffer.size(); ++i) {
        std::cout << i << ". " << CurrentBuffer[i] << "\n";
    }
}

void RenderEqualizer::EnableVisualizer(std::vector<double>& freq, std::mutex& magMutex, int sampleRate) 
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    termWidth  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    termHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    N_BARS = (termWidth - 1) / 2;

    unsigned int frameCount = 0;
    std::string frame;
    frame.reserve(termWidth * termHeight);
    std::vector<float> barValues;
    barValues.resize(N_BARS);

    SetConsoleOutputCP(CP_UTF8);
    
    // switch to alternate screen buffer and hide cursor
    // \033[?1049h: switch to alternate buffer
    // \033[H: move cursor to home
    // \033[2J: clear entire screen
    // \033[?25l: hide cursor
    std::cout << "\033[?1049h\033[H\033[2J\033[?25l" << std::flush;

    while (AudioEngine::Get().IsRunning()) {

        if (frameCount % 30 == 0) {
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            int newWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int newHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            
            if (newWidth != termWidth || newHeight != termHeight) {
                termWidth = newWidth;
                termHeight = newHeight;
                N_BARS = (termWidth - 1) / 2;
                if (N_BARS < 1) N_BARS = 1;
                barValues.resize(N_BARS);
                frame.reserve(termWidth * termHeight);
                // clear alternate screen on resize
                std::cout << "\033[2J" << std::flush;
            }
        }
        frameCount++;

        float masterVol = AudioEngine::Get().GenVolLevel();

        bool hasData = false;
        {
            std::lock_guard<std::mutex> lock(magMutex);

            if (freq.size() >= 1201) {
                hasData = true;
                
                for (int i = 0; i < N_BARS; i++) {
                    float fLow = 20.0f * pow(20000.0f / 20.0f, (float)i / N_BARS);
                    float fHigh = 20.0f * pow(20000.0f / 20.0f, (float)(i + 1) / N_BARS);
                    
                    int binLow  = (int)(fLow * 1201 / (sampleRate / 2.f));
                    int binHigh = (int)(fHigh * 1201 / (sampleRate / 2.f));
                    
                    binLow  = std::max(0, std::min(binLow, 1200));
                    binHigh = std::max(0, std::min(binHigh, 1200));
                    if (binLow >= binHigh) binHigh = binLow + 1;
                    if (binHigh > (int)freq.size()) binHigh = (int)freq.size();
                    
                    double peak = *std::max_element(freq.begin() + binLow, freq.begin() + binHigh);

                    // with dB+100, silent is ~0 and loud is ~100
                    double noiseFloor = 20.0; 
                    double maxVolume = 90.0; 

                    float percentage = (float)((peak - noiseFloor) / (maxVolume - noiseFloor));

                    if (percentage < 0.0f) percentage = 0.0f;
                    if (percentage > 1.0f) percentage = 1.0f;

                    // scale by master volume slider
                    percentage *= masterVol;

                    // smoothing: 70% new, 30% old
                    barValues[i] = barValues[i] * 0.3f + percentage * 0.7f;
                }
            }
        }

        if (!hasData) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        frame.clear();
        // use termHeight - 1 to allow bars to touch the top row
        int renderHeight = termHeight - 1;
        if (renderHeight < 1) renderHeight = 1;

        for (int row = renderHeight; row > 0; row--) {
            for (int i = 0; i < N_BARS; i++) {
                int barHeight = (int)(barValues[i] * renderHeight);

                if (row <= barHeight) {
                    frame += "██";
                } else {
                    frame += "  ";
                }
            }
            if (row > 1) frame += '\n'; 
        }

        // Use \033[H to jump to top-left, \033[J to clear what's below the new frame
        std::cout << "\033[H" << frame << "\033[J" << std::flush;        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
    }
    
    // clear alternate screen, switch back to main buffer, and show cursor
    // \033[2J: clear screen
    // \033[H: home cursor
    // \033[?1049l: switch back to main buffer
    // \033[?25h: show cursor
    std::cout << "\033[2J\033[H\033[?1049l\033[?25h" << std::flush;
}