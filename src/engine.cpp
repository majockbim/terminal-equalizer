#include "../inc/engine.hpp"

class AudioEngine {
public:
    AudioEngine(const AudioEngine&) = delete; // remove copy constructor
    AudioEngine(AudioEngine&&) = delete; // remove move constructor

    // Access class statically
    static AudioEngine& Get()
    {
        static AudioEngine instance;
        return instance;
    }

    float GenVolLevel() { return Get().InternalGenVol(); }

private:
    // Initialize object
    AudioEngine() 
    { 
        CoInitialize(NULL);

        // Get the default audio endpoint
        CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
        pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

        // Activate volume interface
        pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);

        // https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nf-endpointvolume-iaudioendpointvolume-getchannelcount
        pEndpointVolume->GetChannelCount(&ChannelCount);

        // https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nf-endpointvolume-iaudioendpointvolume-getchannelcount
        HRESULT result = pEndpointVolume->GetChannelCount(&ChannelCount);
    }
    
    // Initialize member variables
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;

    // Count of channels in audio stream that enter/leave audio enpoint
    UINT ChannelCount = 0;

    // Cleanup
    ~AudioEngine()
    {
        pEndpointVolume->Release();
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
    }

    float InternalGenVol() 
    { 
        float vol = 0;

        // Volume for channel 0
        return pEndpointVolume->GetChannelVolumeLevelScalar(0, &vol);
    }

};