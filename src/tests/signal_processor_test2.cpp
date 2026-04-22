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