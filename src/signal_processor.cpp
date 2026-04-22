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

#include "../inc/processing/signal_processor.hpp"

SignalProcessor::SignalProcessor()
{
    // reserve space for 5 sample packets of 480
    samples.reserve(2400);
}

void SignalProcessor::Accumulate()
{
    std::vector<double> currentAudio = AudioEngine::Get().GetCurrentBuffer();

    if(currentAudio.size() == 0) return;
    
    samples.insert(samples.end(), currentAudio.begin(), currentAudio.end());    
}

// if the vector somehow contains more than 2400 items don't remove them
std::array<double, 2400> SignalProcessor::GetFFTBuffer()
{   
    std::array<double, 2400> FFTBuffer;

    if(isFull()) {
        // assign 0 - 2400
        std::copy(samples.begin(), samples.begin() + 2400, FFTBuffer.begin());

        // remove only the values added to FFTBuffer
        samples.erase(samples.begin(), samples.begin() + 2400);
    }

    return FFTBuffer;
}

bool SignalProcessor::isFull()
{
    if(samples.size() >= 2400) return true;
    return false;
}