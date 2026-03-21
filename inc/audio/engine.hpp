#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>
#include <mmeapi.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

class AudioEngine {
public:
    AudioEngine(const AudioEngine&) = delete; // remove copy constructor
    AudioEngine(AudioEngine&&) = delete; // remove move constructor

    // Access class statically
    static AudioEngine& Get();

    float GenVolLevel();

    std::vector<float> GetCurrentBuffer();

    // todo: get audio which is passable via an array of floats/doubles
    float FloatVolume();

private:
    // Initialize object
    AudioEngine();

    ~AudioEngine();

    // Initialize member variables (volume level)
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;

    // Initialize member variables (audio samples)
    IAudioClient* pAudioClient = nullptr; // manage audio client
    IAudioCaptureClient* pAudioCaptureClient = nullptr; // capture buffer
    WAVEFORMATEX* pWAVEFORMATEX = nullptr; // [4]

    // temp retreival functions 
    float InternalGenVol();
    std::vector<float> InternalBuffer();

    // temp visualizer functions
    void DisplayMaster(); // display all audio samples

    std::vector<float> buffer;

};

#endif