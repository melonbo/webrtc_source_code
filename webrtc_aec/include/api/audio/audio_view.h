// Simple implementation of audio_view.h for compilation purposes
#ifndef API_AUDIO_AUDIO_VIEW_H_
#define API_AUDIO_AUDIO_VIEW_H_

#include <cstddef>

namespace webrtc {

// Simple AudioView implementation for compilation purposes
template <typename T>
class AudioView {
public:
  AudioView() : data_(nullptr), size_(0) {}
  AudioView(T* data, size_t size) : data_(data), size_(size) {}
  
  T* data() const { return data_; }
  size_t size() const { return size_; }
  T& operator[](size_t index) const { return data_[index]; }
  
private:
  T* data_;
  size_t size_;
};

// Simple DeinterleavedView implementation for compilation purposes
template <typename T>
class DeinterleavedView {
public:
  DeinterleavedView() : data_(nullptr), num_frames_(0), num_channels_(0) {}
  DeinterleavedView(T* const* data, size_t num_frames, size_t num_channels)
      : data_(data), num_frames_(num_frames), num_channels_(num_channels) {}
  
  T* const* data() const { return data_; }
  size_t num_frames() const { return num_frames_; }
  size_t num_channels() const { return num_channels_; }
  T* operator[](size_t channel) const { return data_[channel]; }
  
private:
  T* const* data_;
  size_t num_frames_;
  size_t num_channels_;
};

}  // namespace webrtc

#endif  // API_AUDIO_AUDIO_VIEW_H_
