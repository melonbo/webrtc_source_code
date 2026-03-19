// Simple implementation of platform_thread_types.h for compilation purposes
#ifndef RTC_BASE_PLATFORM_THREAD_TYPES_H_
#define RTC_BASE_PLATFORM_THREAD_TYPES_H_

#include <cstdint>

namespace rtc {

// Platform thread ID type
typedef uint32_t PlatformThreadId;

}  // namespace rtc

namespace webrtc {

// Use rtc::PlatformThreadId for compatibility
typedef rtc::PlatformThreadId PlatformThreadId;

}  // namespace webrtc

#endif  // RTC_BASE_PLATFORM_THREAD_TYPES_H_
