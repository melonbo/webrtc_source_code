// Simple implementation of channel_buffer.h for compilation purposes
#ifndef COMMON_AUDIO_CHANNEL_BUFFER_H_
#define COMMON_AUDIO_CHANNEL_BUFFER_H_

#include <cstddef>

namespace webrtc {

// Simple ChannelBuffer implementation for compilation purposes
template <typename T>
class ChannelBuffer {
public:
  ChannelBuffer(size_t num_channels, size_t num_frames) : 
      num_channels_(num_channels), num_frames_(num_frames) {
    data_ = new T*[num_channels];
    for (size_t i = 0; i < num_channels; ++i) {
      data_[i] = new T[num_frames];
    }
  }
  
  ~ChannelBuffer() {
    for (size_t i = 0; i < num_channels_; ++i) {
      delete[] data_[i];
    }
    delete[] data_;
  }
  
  T* const* channels() const { return data_; }
  T* const* channels(size_t band) const { return data_; }
  T* operator[](size_t channel) { return data_[channel]; }
  const T* operator[](size_t channel) const { return data_[channel]; }
  size_t num_channels() const { return num_channels_; }
  size_t num_frames() const { return num_frames_; }
  
  // Add bands method for compatibility
  T* const* bands(size_t channel) const { return data_; }
  
private:
  size_t num_channels_;
  size_t num_frames_;
  T** data_;
};

}  // namespace webrtc

#endif  // COMMON_AUDIO_CHANNEL_BUFFER_H_
