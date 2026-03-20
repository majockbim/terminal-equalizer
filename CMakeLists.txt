cmake_minimum_required(VERSION 3.10)
project(TerminalEqualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# defining target
add_executable(terminal-equalizer 
    src/main.cpp 
    src/engine.cpp 
    src/visualizer.cpp 
    third_party/src/fftw++.cc
)

# include directories -I and -isystem
target_include_directories(terminal-equalizer PRIVATE inc)
target_include_directories(terminal-equalizer SYSTEM PRIVATE third_party/inc)

# library directories -L
target_link_directories(terminal-equalizer PRIVATE third_party/lib)

# link libraries -lfftw3-3 -lole32 -lmmdevapi
target_link_libraries(terminal-equalizer PRIVATE fftw3-3 ole32 mmdevapi)

# compiler and linker flags -fopenmp, -mconsole
target_compile_options(terminal-equalizer PRIVATE -fopenmp)
target_link_options(terminal-equalizer PRIVATE -fopenmp -mconsole -Wl,-subsystem,console)