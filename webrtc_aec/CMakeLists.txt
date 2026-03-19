cmake_minimum_required(VERSION 3.10)
project(webrtc_aec)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCE_FILES
    src/main.cpp
    src/audio_buffer.cc
    src/high_pass_filter.cc
    src/logging/apm_data_dumper.cc
    src/aec3/echo_canceller3.cc
    src/aec3/block_processor.cc
    src/aec3/block_processor_metrics.cc
    src/aec3/block_framer.cc
    src/aec3/block_delay_buffer.cc
    src/aec3/frame_blocker.cc
    src/aec3/echo_path_variability.cc
    src/aec3/echo_remover.cc
    src/aec3/echo_remover_metrics.cc
    src/aec3/render_delay_buffer.cc
    src/aec3/render_delay_controller.cc
    src/aec3/aec3_fft.cc
    src/aec3/aec_state.cc
    src/aec3/comfort_noise_generator.cc
    src/aec3/render_buffer.cc
    src/aec3/render_signal_analyzer.cc
    src/aec3/residual_echo_estimator.cc
    src/aec3/subtractor.cc
    src/aec3/suppression_filter.cc
    src/aec3/suppression_gain.cc
    src/aec3/adaptive_fir_filter.cc
    src/aec3/adaptive_fir_filter_erl.cc
    src/aec3/coarse_filter_update_gain.cc
    src/aec3/refined_filter_update_gain.cc
)

add_executable(webrtc_aec ${SOURCE_FILES})
