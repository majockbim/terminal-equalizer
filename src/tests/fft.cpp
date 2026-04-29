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