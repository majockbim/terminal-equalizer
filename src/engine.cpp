#include "../inc/audio/engine.hpp"

// Initialize object
AudioEngine::AudioEngine() 
{ 
    HRESULT hr;
    CoInitialize(NULL);

    // Get the default audio endpoint
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    // Activate general volume interface
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);

    // Activate sample/buffer interface
    hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);

    // Mix Format = How client captures
    hr = pAudioClient->GetMixFormat(&pWAVEFORMATEX);

    //initialize for loopback
    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 10000000, 0, pWAVEFORMATEX, NULL);

    // get capture service
    hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pAudioCaptureClient);

    // start audio client
    hr = pAudioClient->Start();

    // Count of channels in audio stream that enter/leave audio enpoint
    UINT ChannelCount = 0;

    // https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nf-endpointvolume-iaudioendpointvolume-getchannelcount
    hr = pEndpointVolume->GetChannelCount(&ChannelCount);

    // https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nf-endpointvolume-iaudioendpointvolume-getchannelcount
    HRESULT result = pEndpointVolume->GetChannelCount(&ChannelCount);

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

std::vector<float> AudioEngine::GetCurrentBuffer()
{
    return Get().InternalBuffer();
}

// Cleanup
AudioEngine::~AudioEngine()
{
    // general volume
    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();

    // audio samples
    if (pAudioCaptureClient) pAudioCaptureClient->Release();
    if (pAudioClient) pAudioClient->Release();

    // free COM-allocated memory (windows allocates it manually)
    if (pWAVEFORMATEX) {
        CoTaskMemFree(pWAVEFORMATEX); // [4]
    }

    CoUninitialize();
}

float AudioEngine::InternalGenVol() 
{ 
    float vol = 0;

    // Update volume level for channel 0 (CBR)
    pEndpointVolume->GetChannelVolumeLevelScalar(0, &vol);

    return vol;
}

// capture loop
std::vector<float> AudioEngine::InternalBuffer()
{
    UINT32 packetLength = 0;
    HRESULT hr = pAudioCaptureClient->GetNextPacketSize(&packetLength);

    // clear previous data
    if(packetLength != 0) {
        std::cout << "Caught packet of size: " << packetLength << "\n";
        buffer.clear();
    }

    while(packetLength != 0)
    {
        BYTE* pData;
        UINT32 availableFrames;
        DWORD flags;

        // lock windows audio buffer
        hr = pAudioCaptureClient->GetBuffer(&pData, &availableFrames, &flags, NULL, NULL);

        // cast raw bytes into floats
        float* fData = (float*)pData;

        for(UINT32 i = 0; i < availableFrames; ++i)
        {
            // *2 for left and right values of stereo :) ([left][right])
            float left = fData[i * 2];
            float right = fData[i * 2 + 1];
            buffer.push_back((left + right) / 2.0f);
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