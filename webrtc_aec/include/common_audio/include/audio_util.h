// Simple implementation of audio_util.h for compilation purposes
#ifndef COMMON_AUDIO_INCLUDE_AUDIO_UTIL_H_
#define COMMON_AUDIO_INCLUDE_AUDIO_UTIL_H_

namespace webrtc {

// Simple audio utility functions
inline float FloatS16ToFloat(int16_t value) {
  return static_cast<float>(value) / 32768.0f;
}

inline int16_t FloatToFloatS16(float value) {
  if (value > 1.0f) value = 1.0f;
  if (value < -1.0f) value = -1.0f;
  return static_cast<int16_t>(value * 32767.0f);
}

}  // namespace webrtc

#endif  // COMMON_AUDIO_INCLUDE_AUDIO_UTIL_H_
