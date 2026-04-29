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

#include "../inc/audio/engine.hpp"

// Initialize object
AudioEngine::AudioEngine() 
{ 
    HRESULT hr;

    // Get the default audio endpoint
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) return;

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) return;

    // Activate general volume interface
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);

    // Activate sample/buffer interface
    hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
    if (FAILED(hr)) return;

    // Mix Format = How client captures
    hr = pAudioClient->GetMixFormat(&pWAVEFORMATEX);
    if (FAILED(hr)) return;

    bitsPerSample = pWAVEFORMATEX->wBitsPerSample;
    nChannels = pWAVEFORMATEX->nChannels;
    
    if (pWAVEFORMATEX->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
        isFloat = true;
    } else if (pWAVEFORMATEX->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        WAVEFORMATEXTENSIBLE* pEx = (WAVEFORMATEXTENSIBLE*)pWAVEFORMATEX;
        // Check for KSDATAFORMAT_SUBTYPE_IEEE_FLOAT: {00000003-0000-0010-8000-00aa00389b71}
        if (pEx->SubFormat.Data1 == 0x00000003 && pEx->SubFormat.Data2 == 0x0000 && pEx->SubFormat.Data3 == 0x0010 &&
            pEx->SubFormat.Data4[0] == 0x80 && pEx->SubFormat.Data4[1] == 0x00 && pEx->SubFormat.Data4[2] == 0x00 &&
            pEx->SubFormat.Data4[3] == 0xAA && pEx->SubFormat.Data4[4] == 0x00 && pEx->SubFormat.Data4[5] == 0x38 &&
            pEx->SubFormat.Data4[6] == 0x9B && pEx->SubFormat.Data4[7] == 0x71) {
            isFloat = true;
        }
    }

    //initialize for loopback
    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 10000000, 0, pWAVEFORMATEX, NULL);
    if (FAILED(hr)) return;

    // get capture service
    hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pAudioCaptureClient);
    if (FAILED(hr)) return;

    // start audio client
    hr = pAudioClient->Start();

    // allow time for everything to process before moving on (20ms)
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

// Access class statically
AudioEngine& AudioEngine::Get()
{
    static AudioEngine instance;
    return instance;
}

float AudioEngine::GenVolLevel() 
{ 
    return Get().InternalGenVol();
}

std::vector<double> AudioEngine::GetCurrentBuffer()
{
    return Get().InternalBuffer();
}

// Cleanup
AudioEngine::~AudioEngine()
{
    // general volume
    if (pEndpointVolume) pEndpointVolume->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();

    // audio samples
    if (pAudioCaptureClient) pAudioCaptureClient->Release();
    if (pAudioClient) pAudioClient->Release();

    // free COM-allocated memory (windows allocates it manually)
    if (pWAVEFORMATEX) {
        CoTaskMemFree(pWAVEFORMATEX); // [4]
    }
}

float AudioEngine::InternalGenVol() 
{ 
    float vol = 0;
    if (!pEndpointVolume) return 0;

    // Update volume level for channel 0 (CBR)
    pEndpointVolume->GetChannelVolumeLevelScalar(0, &vol);

    return vol;
}

// capture loop
std::vector<double> AudioEngine::InternalBuffer()
{
    UINT32 packetLength = 0;
    if (!pAudioCaptureClient) return {};

    HRESULT hr = pAudioCaptureClient->GetNextPacketSize(&packetLength);

    buffer.clear();

    while(packetLength != 0)
    {
        BYTE* pData;
        UINT32 availableFrames;
        DWORD flags;

        // lock windows audio buffer
        hr = pAudioCaptureClient->GetBuffer(&pData, &availableFrames, &flags, NULL, NULL);
        if (FAILED(hr)) break;

        if (isFloat) {
            float* fData = (float*)pData;
            for(UINT32 i = 0; i < availableFrames; ++i)
            {
                double sum = 0;
                for (int c = 0; c < nChannels; ++c) {
                    sum += fData[i * nChannels + c];
                }
                buffer.push_back(sum / (double)nChannels);
            }
        } else if (bitsPerSample == 64) {
            double* fData = (double*)pData;
            for(UINT32 i = 0; i < availableFrames; ++i)
            {
                double sum = 0;
                for (int c = 0; c < nChannels; ++c) {
                    sum += fData[i * nChannels + c];
                }
                buffer.push_back(sum / (double)nChannels);
            }
        } else if (bitsPerSample == 16) {
            short* fData = (short*)pData;
            for(UINT32 i = 0; i < availableFrames; ++i)
            {
                double sum = 0;
                for (int c = 0; c < nChannels; ++c) {
                    sum += fData[i * nChannels + c];
                }
                buffer.push_back(sum / (double)nChannels / 32768.0);
            }
        }

        // release lock
        hr = pAudioCaptureClient->ReleaseBuffer(availableFrames);

        // check if there is another packet waiting
        hr = pAudioCaptureClient->GetNextPacketSize(&packetLength);
    }

    return buffer;
}

void AudioEngine::DisplayMaster() 
{
    // display each float in buffer
}