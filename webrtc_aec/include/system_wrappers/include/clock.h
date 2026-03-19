// Simple implementation of clock.h for compilation purposes
#ifndef SYSTEM_WRAPPERS_INCLUDE_CLOCK_H_
#define SYSTEM_WRAPPERS_INCLUDE_CLOCK_H_

namespace webrtc {

class Clock {
public:
  virtual ~Clock() = default;
  // Add necessary methods here
};

}  // namespace webrtc

#endif  // SYSTEM_WRAPPERS_INCLUDE_CLOCK_H_
