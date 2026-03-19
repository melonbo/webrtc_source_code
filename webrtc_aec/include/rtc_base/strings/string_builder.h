// Simple implementation of string_builder.h for compilation purposes
#ifndef RTC_BASE_STRINGS_STRING_BUILDER_H_
#define RTC_BASE_STRINGS_STRING_BUILDER_H_

#include <string>

namespace rtc {

class StringBuilder {
public:
  StringBuilder() = default;
  // Add necessary methods here
  template <typename T>
  StringBuilder& operator<<(const T& value) {
    // Simple implementation
    return *this;
  }
  std::string str() const {
    return "";
  }
};

}  // namespace rtc

namespace webrtc {

// Use rtc::StringBuilder for compatibility
typedef rtc::StringBuilder StringBuilder;

}  // namespace webrtc

#endif  // RTC_BASE_STRINGS_STRING_BUILDER_H_
