// Simple implementation of timestamp.h for compilation purposes
#ifndef API_UNITS_TIMESTAMP_H_
#define API_UNITS_TIMESTAMP_H_

namespace webrtc {

class Timestamp {
public:
  Timestamp() = default;
  
  // Static method to return a timestamp representing minus infinity
  static Timestamp MinusInfinity() {
    return Timestamp();
  }
};

}  // namespace webrtc

#endif  // API_UNITS_TIMESTAMP_H_
