/*
    spectrum - A real-time command line audio visualizer
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

#pragma once

#include "../audio/engine.hpp"
#include "../inc/settings/settings.hpp"

#include <windows.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <winuser.h>

class RenderEqualizer {
public:
    void Display(); // general volume level
    void DisplayBuffer(); // print float values of buffer

    void EnableVisualizer(std::vector<double>& freq, std::mutex& magMutex, int sampleRate, JsonFileReader& jsonFileReader);

private:
    CONSOLE_SCREEN_BUFFER_INFO csbi;        
    int termWidth;
    int termHeight;
    int lastHeight;
    int lastWidth;

    int N_BARS;

    std::vector<std::string> levels = 
    {   "#", 
        "##",
        "###",
        "####", 
        "#####", 
        "######", 
        "#######", 
        "########", 
        "#########", 
        "##########"
    };
};
