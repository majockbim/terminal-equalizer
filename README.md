pet project written in C++17

## high level analysis of the application:
main.cpp: entry point of the application, calls the necessary functions to initialize the application and start the main loop. <br>
visualizer.cpp: contains the code for the visualizer, which is responsible for displaying the audio levels in a graphical way. <br>
engine.cpp: contains the code for the audio engine, which is responsible for capturing the audio levels and providing them to the visualizer.

## recommended compilation command (GCC compiler)
```
g++ src/main.cpp src/engine.cpp src/visualizer.cpp -I./inc -lole32 -lmmdevapi -o src/executables/name_of_executable.exe
```

readings: <br>
https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudioendpointvolume
