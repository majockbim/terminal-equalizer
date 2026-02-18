#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

int main() {
    CoInitialize(NULL);
    
    // Initializations
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;

    // Get the default audio endpoint
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    // Activate volume interface
    pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);

    // Get a count of channels in audio stream that enter/leave audio enpoint
    UINT ChannelCount = 0;

    // https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nf-endpointvolume-iaudioendpointvolume-getchannelcount
    HRESULT result = pEndpointVolume->GetChannelCount(&ChannelCount);

    std::cout << "Result: " << result << "\n";
    std::cout << "Channel Count: " << ChannelCount;


    // Print volume level
    while(true) {

        float vol = 0;

        pEndpointVolume->GetChannelVolumeLevelScalar(0, &vol);

        std::cout << vol << "\n";

    }

    // cleanup
    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();

    return 0;
}