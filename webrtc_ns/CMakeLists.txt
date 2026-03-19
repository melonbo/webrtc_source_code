cmake_minimum_required(VERSION 3.10)
project(webrtc_ns)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include/webrtc
)

# Source files
set(SOURCE_FILES
    src/ns/noise_suppressor.cc
    src/ns/noise_estimator.cc
    src/ns/speech_probability_estimator.cc
    src/ns/wiener_filter.cc
    src/ns/ns_fft.cc
    src/ns/suppression_params.cc
    src/ns/quantile_noise_estimator.cc
    src/ns/signal_model_estimator.cc
    src/main.cpp
)

# Create executable
add_executable(webrtc_ns_example ${SOURCE_FILES})

# Link libraries
if(WIN32)
    target_link_libraries(webrtc_ns_example PRIVATE msvcrt)
else()
    target_link_libraries(webrtc_ns_example PRIVATE m)
endif()
