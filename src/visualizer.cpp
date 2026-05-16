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

#define AsciiRgb(k, r, g, b) "\033[" #k ";2;" #r ";" #g ";" #b "m"

template <typename _divide_number_t, typename _divide_number1_t>
inline _divide_number_t __cdecl SafeDivide(_In_ _divide_number_t a, _In_ _divide_number1_t b) noexcept 
{
    if (b == 0) {return(1.00f);} return(_divide_number_t(_divide_number1_t(a)/b));
}

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

bool __cdecl IsCorrectRow(int row) {
    int index = 0;
    const int Rows[] = {1, 3, 6, 4, 8, 19, 13, 16, 45, 82, 17, 36};
    while (index < sizeof(Rows) / sizeof(const int)) {
        if (row == Rows[index]) return true;
        index++;
    }
    return false;
}

void RenderEqualizer::EnableVisualizer(std::vector<double>& freq, std::mutex& magMutex, int sampleRate, JsonFileReader& jsonFileReader) 
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    termWidth  = csbi.srWindow.Right - csbi.srWindow.Left;
    termHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    N_BARS = (termWidth - 1) / 2;

    unsigned int frameCount = 0;
    std::string frame;
    std::string menu;
    frame.reserve(termWidth * termHeight);
    std::vector<float> barValues;
    barValues.resize(N_BARS);

    const struct ThemeModeManager themeModeManager[] = {
        {"Default Mode", ThemeMode::Mode0},
        {"Mode 1", ThemeMode::Mode1},
        {"Mode 2", ThemeMode::Mode2},
        {"Mode 3", ThemeMode::Mode3},
        {"Mode 4", ThemeMode::Mode4}
    };
    size_t sizeOfTMM = sizeof(themeModeManager) / sizeof(const struct ThemeModeManager);
    enum ThemeMode themeMode = ThemeMode::Mode0;
    
    // switch to alternate screen buffer and hide cursor
    // \033[?1049h: switch to alternate buffer
    // \033[H: move cursor to home
    // \033[2J: clear entire screen
    // \033[?25l: hide cursor
    std::cout << "\033[?1049h\033[H\033[2J\033[?25l" << std::flush;

    int codesIndex = 0;
    int codesIndex1 = 0;
    int timer = 0;
    bool themeChanged = true;
    int i1 = 0;

    while (AudioEngine::Get().IsRunning()) {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        size_t index0 = 0;
        size_t themesArraySize = jsonFileReader.themes.size();
        while (index0 < themesArraySize) {
            if (GetAsyncKeyState(jsonFileReader.themes.at(index0).key) & 0x8000) {
                std::memcpy(&jsonFileReader.currentTheme, &jsonFileReader.themes.at(index0), sizeof(struct Theme));
                strcpy_s(jsonFileReader.currentTheme.themeName, sizeof(jsonFileReader.currentTheme.themeName), jsonFileReader.themes.at(index0).themeName);
                strcpy_s(jsonFileReader.currentTheme.themeId, sizeof(jsonFileReader.currentTheme.themeId), jsonFileReader.themes.at(index0).themeId);
                strcpy_s(jsonFileReader.currentTheme.themeMode, sizeof(jsonFileReader.currentTheme.themeMode), jsonFileReader.themes.at(index0).themeMode);
                themeChanged = true;
            }
            index0++;
        }

        if (themeChanged) {
            size_t tmIndex = 0;
            while (tmIndex < sizeOfTMM) {
                if (strcmp(themeModeManager[tmIndex].themeModeName, jsonFileReader.currentTheme.themeMode) == 0) {
                    themeMode = themeModeManager[tmIndex].themeMode;
                    break;
                }
                themeChanged = false;
                tmIndex++;
            }
        }
        
        termWidth  = csbi.srWindow.Right - csbi.srWindow.Left + 2;
        termHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 2;
        if (termHeight != lastHeight || termWidth != lastWidth) {
            lastHeight = termHeight;
            lastWidth = termWidth;
            
            N_BARS = (termWidth - 1) / 2;
            if (N_BARS < 1) N_BARS = 1;
            barValues.resize(N_BARS);
            frame.reserve(termWidth * termHeight);
            // clear alternate screen on resize
            std::cout << "\033[2J" << std::flush;
        }
        if (frameCount < UINT_MAX) {
            frameCount++;
        } else {
            frameCount = 0;
        }

        float masterVol = AudioEngine::Get().GenVolLevel();
        bool firstTime = true;

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
                    
                    binLow  = max(0, min(binLow, 1200));
                    binHigh = max(0, min(binHigh, 1200));
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
        menu.clear();

        if (jsonFileReader.showMenu) {
            const char* volumeLevels[] = {
                "[]--------------------",
                "=[]-------------------",
                "==[]------------------",
                "===[]-----------------",
                "====[]----------------",
                "=====[]---------------",
                "======[]--------------",
                "=======[]-------------",
                "========[]------------",
                "=========[]-----------",
                "==========[]---------",
                "===========[]--------",
                "============[]-------",
                "=============[]------",
                "==============[]-----",
                "===============[]----",
                "================[]---",
                "=================[]--",
                "==================[]-",
                "===================[]",
            };

            menu += AsciiRgb(48, 0, 25, 23) AsciiRgb(38, 0, 255, 236) " * ╭─╮╭─╮╭─╴╭─╴╶┬╴╭─╮╷ ╷╭┬╮ * Master Volume: ";

            int levelNumber = int((masterVol > 1) ? 1.0f : masterVol * int((sizeof(volumeLevels)  - sizeof(volumeLevels[0])) / sizeof(volumeLevels[0])));
            menu += volumeLevels[levelNumber];

            menu += "(";
            menu += std::to_string(int(masterVol * 100));
            menu += ") |";

            menu += "\033[0m";
            menu += "\n";

            menu += AsciiRgb(48, 0, 18, 25) AsciiRgb(38, 0, 180, 255)  " = ╰─╮├─╯├╴ │   │ ├┬╯│ ││││ = \n";
            menu += AsciiRgb(48, 0, 7, 25) AsciiRgb(38, 0, 73, 255) " * ╰─╯╵  ╰─╴╰─╴ ╵ ╵╰╴╰─╯╵ ╵ * \n";
        }
        // use termHeight - 1 to allow bars to touch the top row
        int added = (jsonFileReader.showMenu) ? 3 : 1;
        int renderHeight = termHeight - added;
        if (renderHeight < 1) renderHeight = 1;

        const char* UTF8Codes[] = {
            "─", "│", "─", "│", "┌", "┐", "└", "┘", "├", "┤", "┬", "┴", "┼",
            "♪", "♫", "♬", "♩", "⣿", "⣶", "⣤", "⣀", "⡇", "#", "a", "b", "c", "e",
            "f", "g", "h", "♬", "♬", " ", "C", "R", "E", "D", "I", "T", "S", " ", "♬", "♬",
            " ", "♬", "C", "o", "l", "o", "r", "s", " ", "B", "y", " ", "J", "o", "e", ".", "r", "♬",
            " ", "♬", "V", "i", "s", "u", "a", "l", "i", "z", "e", "r", " ", "B", "y", " ", "M", "j", "♬"
        };
        size_t sizeofCodes = sizeof(UTF8Codes) / sizeof(UTF8Codes[0]);

        float rowA = 0;
        float rowB = 0;
        float rowC = 0;
        float effect = 1.0f;
        int time = 100;
        for (int row = renderHeight; row > 0; row--) {
            codesIndex1 = 0;
            for (int i = 0; i < N_BARS; i++) {
                firstTime = false;
                int barHeight = (int)(barValues[i] * renderHeight);

                if (themeMode == ThemeMode::Mode4) {
                    effect = barValues[i] * 2.5f;
                }

                char tempArray[64];
                char tempArray1[64];
                int color8BitRaw[3] = {
                    (int)((255 * rowA * effect) * (float)jsonFileReader.currentTheme.colorRed), 
                    (int)((255 * rowB * effect) * (float)jsonFileReader.currentTheme.colorGreen), 
                    (int)((255 * rowC * effect) * (float)jsonFileReader.currentTheme.colorBlue)
                };

                int color8BitRawbg[3] = {
                    (int)((255 * rowA * effect) * (float)jsonFileReader.currentTheme.colorRed), 
                    (int)((255 * rowB * effect) * (float)jsonFileReader.currentTheme.colorGreen), 
                    (int)((255 * rowC * effect) * (float)jsonFileReader.currentTheme.colorBlue)
                };

                unsigned char color8Bitbg[3] = {
                    (color8BitRawbg[0] > 255) ? (unsigned char)255 : (unsigned char)color8BitRawbg[0],
                    (color8BitRawbg[1] > 255) ? (unsigned char)255 : (unsigned char)color8BitRawbg[1],
                    (color8BitRawbg[2] > 255) ? (unsigned char)255 : (unsigned char)color8BitRawbg[2]
                };

                unsigned char color8Bit[3] = {
                    (color8BitRaw[0] > 255) ? (unsigned char)255 : (unsigned char)color8BitRaw[0],
                    (color8BitRaw[1] > 255) ? (unsigned char)255 : (unsigned char)color8BitRaw[1],
                    (color8BitRaw[2] > 255) ? (unsigned char)255 : (unsigned char)color8BitRaw[2]
                };
                if (codesIndex >= sizeofCodes) {
                    codesIndex = 0;
                }
                if (codesIndex1 + codesIndex >= sizeofCodes) {
                    codesIndex1 = 0;
                }
                rowA = float(row) / float(renderHeight);
                rowB = -rowA + 1.0f;
                rowC = -fabs(rowA - 0.5f) + 1.0f;

                int barTop, barBottom;
                if (themeMode == ThemeMode::Mode1) {
                    barTop = (int)((renderHeight - barHeight) / 2);
                    barBottom = (int)(renderHeight - ((renderHeight - barHeight) / 2));
                }

                if (themeMode == ThemeMode::Mode2) {
                    barTop = barHeight;
                    barBottom = barHeight;
                }

                if (themeMode == ThemeMode::Mode3) {
                    int barHeight1 = renderHeight - barHeight;
                    barTop = (int)((renderHeight - barHeight1) / 2);
                    barBottom = (int)(renderHeight - ((renderHeight - barHeight1) / 2));
                }

                char character[2] = {jsonFileReader.currentTheme.customCharacter, '\0'};
                
                sprintf_s(tempArray, sizeof(tempArray), "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm:%s\033[0m", color8Bit[0], color8Bit[1], color8Bit[2], 
                    SafeDivide<unsigned char, unsigned char>(color8Bitbg[0], ((16 * rowA) + 8)), 
                    SafeDivide<unsigned char, unsigned char>(color8Bitbg[1], ((16 * rowA) + 8)), 
                    SafeDivide<unsigned char, unsigned char>(color8Bitbg[2], ((16 * rowA) + 8)), (jsonFileReader.currentTheme.useRandomCharacters) ? ((IsCorrectRow(row)) ? UTF8Codes[codesIndex + codesIndex1] : UTF8Codes[rand() % sizeofCodes]) : character);
                
                sprintf_s(tempArray1, sizeof(tempArray1), "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm  \033[0m", color8Bit[0], color8Bit[1], color8Bit[2], 
                    SafeDivide<unsigned char, unsigned char>(color8Bitbg[0], ((16 * rowA) + 8)), 
                    SafeDivide<unsigned char, unsigned char>(color8Bitbg[1], ((16 * rowA) + 8)), 
                    SafeDivide<unsigned char, unsigned char>(color8Bitbg[2], ((16 * rowA) + 8)));

                if (themeMode == ThemeMode::Mode0) { 
                    if (row <= barHeight) {
                        frame += tempArray;
                    } else {
                        frame += (!jsonFileReader.noBgColor) ? tempArray1 : "\033[0m  ";
                    }
                } else if (themeMode == ThemeMode::Mode1 || themeMode == ThemeMode::Mode2 || themeMode == ThemeMode::Mode3) {
                    if (row <= barBottom && row >= barTop) {
                        frame += (themeMode == ThemeMode::Mode1 || themeMode == ThemeMode::Mode2) ? tempArray : tempArray1;
                    } else {
                        frame += (!jsonFileReader.noBgColor) ? ((themeMode == ThemeMode::Mode1 || themeMode == ThemeMode::Mode2) ? tempArray1 : tempArray) : "\033[0m  ";
                    }
                } else if (themeMode == ThemeMode::Mode4) {
                    frame += tempArray;
                }
                if (timer >= time) {
                    codesIndex++;
                    timer = 0;
                }
                codesIndex1++;
            }
            if (row > 1) frame += '\n'; 
            timer++;
        }

        // Use \033[H to jump to top-left, \033[J to clear what's below the new frame
        std::cout << "\033[H" << menu << frame << "\033[J" << std::flush;        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
    }
    
    // clear alternate screen, switch back to main buffer, and show cursor
    // \033[2J: clear screen
    // \033[H: home cursor
    // \033[?1049l: switch back to main buffer
    // \033[?25h: show cursor
    std::cout << "\033[2J\033[H\033[?1049l\033[?25h" << std::flush;
}
