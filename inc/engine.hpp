#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

class AudioEngine {
public:
    AudioEngine(const AudioEngine&) = delete; // remove copy constructor
    AudioEngine(AudioEngine&&) = delete; // remove move constructor

    // Access class statically
    static AudioEngine& Get();

    float GenVolLevel();

private:
    // Initialize object
    AudioEngine();

    ~AudioEngine();

    // Initialize member variables
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;
    
    float InternalGenVol();

};

#endif