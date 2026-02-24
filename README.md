pet project written in C++17

## high level analysis of the application:
main.cpp: entry point of the application, calls the necessary functions to initialize the application and start the main loop. <br>
visualizer.cpp: contains the code for the visualizer, which is responsible for displaying the audio levels in a graphical way. <br>
engine.cpp: contains the code for the audio engine, which is responsible for capturing the audio levels and providing them to the visualizer.

readings: <br>
https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudioendpointvolume
https://en.wikipedia.org/wiki/Fast_Fourier_transform
https://fftwpp.sourceforge.net/