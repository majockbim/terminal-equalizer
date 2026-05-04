cmake_minimum_required(VERSION 3.10)
project(spectrum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# defining target
add_executable(spectrum
    src/main.cpp 
    src/engine.cpp 
    src/visualizer.cpp 
    src/signal_processor.cpp 
    src/fft.cpp
)

# include directories -I and -isystem
target_include_directories(spectrum PRIVATE inc)
target_include_directories(spectrum SYSTEM PRIVATE third_party/inc)

# library directories -L
target_link_directories(spectrum PRIVATE third_party/lib)

# link libraries -lfftw3-3 -lole32 -lmmdevapi
target_link_libraries(spectrum PRIVATE fftw3-3 ole32 mmdevapi)

# compiler and linker flags -fopenmp, -mconsole
target_compile_options(spectrum PRIVATE -fopenmp)
target_link_options(spectrum PRIVATE -fopenmp -mconsole -Wl,-subsystem,console)
