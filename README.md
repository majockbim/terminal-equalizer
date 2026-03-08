pet project written in C++17

## high level analysis of the application:
main.cpp: entry point of the application, calls the necessary functions to initialize the application and start the main loop. <br>
visualizer.cpp: contains the code for the visualizer, which is responsible for displaying the audio levels in a graphical way. <br>
engine.cpp: contains the code for the audio engine, which is responsible for capturing the audio levels and providing them to the visualizer.

##  build and setup
run these commands to compile the project and move the required DLLs into the executable folder:

```powershell
# compile the project
cmake -S . -B output -G "MinGW Makefiles"
cmake --build output

# move required DLLs to the output folder
Copy-Item C:\msys64\ucrt64\bin\libgomp-1.dll -Destination .\output\
Copy-Item .\third_party\lib\libfftw3-3.dll -Destination .\output\

# run (assuming path is terminal-equalizer)
.\output\terminal-equalizer.exe
```

## References

**readings:** <br>
https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudioendpointvolume <br>
https://en.wikipedia.org/wiki/Fast_Fourier_transform <br>

**libraries used**: <br>
https://fftwpp.sourceforge.net/, 
https://github.com/dealias/fftwpp 