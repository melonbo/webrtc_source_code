// Simple implementation of splitting_filter.h for compilation purposes
#ifndef MODULES_AUDIO_PROCESSING_SPLITTING_FILTER_H_
#define MODULES_AUDIO_PROCESSING_SPLITTING_FILTER_H_

#include "common_audio/channel_buffer.h"

namespace webrtc {

// Simple SplittingFilter implementation for compilation purposes
class SplittingFilter {
public:
  SplittingFilter(size_t num_channels, size_t num_bands, size_t num_frames) {
    // Dummy implementation
  }
  
  ~SplittingFilter() {
    // Dummy implementation
  }
  
  void Analysis(ChannelBuffer<float>* data, ChannelBuffer<float>* split_data) {
    // Dummy implementation: just copy the data to split_data
    // In a real implementation, this would split the data into frequency bands
  }
  
  void Synthesis(ChannelBuffer<float>* split_data, ChannelBuffer<float>* data) {
    // Dummy implementation: just copy the split_data to data
    // In a real implementation, this would merge the frequency bands
  }
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_SPLITTING_FILTER_H_
