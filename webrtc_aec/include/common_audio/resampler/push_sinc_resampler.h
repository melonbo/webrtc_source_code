// Simple implementation of push_sinc_resampler.h for compilation purposes
#ifndef COMMON_AUDIO_RESAMPLER_PUSH_SINC_RESAMPLER_H_
#define COMMON_AUDIO_RESAMPLER_PUSH_SINC_RESAMPLER_H_

#include <cstddef>

namespace webrtc {

// Simple PushSincResampler implementation for compilation purposes
class PushSincResampler {
public:
  PushSincResampler(size_t input_size, size_t output_size) {
    // Dummy implementation
  }
  
  ~PushSincResampler() {
    // Dummy implementation
  }
  
  void Resample(const float* input, size_t input_size, float* output, size_t output_size) {
    // Dummy implementation: just copy the input to output
    // In a real implementation, this would perform resampling
    for (size_t i = 0; i < output_size && i < input_size; ++i) {
      output[i] = input[i];
    }
  }
};

}  // namespace webrtc

#endif  // COMMON_AUDIO_RESAMPLER_PUSH_SINC_RESAMPLER_H_
