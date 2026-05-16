cmake_minimum_required(VERSION 3.10)
project(spectrum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find OpenMP in a compiler-agnostic way
find_package(OpenMP REQUIRED)

# 2. Defining target
add_executable(spectrum
    src/main.cpp 
    src/engine.cpp 
    src/visualizer.cpp 
    src/signal_processor.cpp 
    src/fft.cpp
    src/settings.cpp
    third_party/ref/yyjson.c
    resources/resources.rc
)

# 3. Include directories
target_include_directories(spectrum PRIVATE inc)
target_include_directories(spectrum SYSTEM PRIVATE third_party/inc)

# 4. Library directories
target_link_directories(spectrum PRIVATE third_party/lib)

# 5. Link libraries
# ole32 and mmdevapi are standard Windows libraries
target_link_libraries(spectrum PRIVATE 
    fftw3 
    ole32 
    mmdevapi
    OpenMP::OpenMP_CXX
)

# 6. Compiler & Linker Options (Agnostic handling)
if(MSVC)
    # MSVC specific flags
    add_definitions(-DNOMINMAX)
    target_compile_options(spectrum PRIVATE /W4 /openmp)
    
    # Request static linking of the C++ Runtime (equivalent to -static for the CRT)
    set_property(TARGET spectrum PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    # MSVC doesn't need a special subsystem flag for console by default, 
    # but we can be explicit:
    target_link_options(spectrum PRIVATE /SUBSYSTEM:CONSOLE)
else()
    # MinGW / GCC / Clang specific flags
    target_compile_options(spectrum PRIVATE -Wall -fopenmp)
    
    # Static linking for portability and console subsystem
    target_link_options(spectrum PRIVATE 
        -fopenmp 
        -mconsole 
        -Wl,-subsystem,console 
        -static
    )
endif()
