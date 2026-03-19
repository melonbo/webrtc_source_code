// Simple implementation of rtc_base/checks.h for compilation purposes
#ifndef RTC_BASE_CHECKS_H_
#define RTC_BASE_CHECKS_H_

#include <cassert>

namespace webrtc {
namespace webrtc_checks_impl {

// Simple LogStreamer implementation
class LogStreamer {
public:
  template <typename T>
  LogStreamer& operator<<(const T& /*arg*/) {
    return *this;
  }
};

}  // namespace webrtc_checks_impl

// Simplified check macros
#define RTC_CHECK(condition) assert(condition)
#define RTC_CHECK_EQ(val1, val2) assert((val1) == (val2))
#define RTC_CHECK_NE(val1, val2) assert((val1) != (val2))
#define RTC_CHECK_LT(val1, val2) assert((val1) < (val2))
#define RTC_CHECK_LE(val1, val2) assert((val1) <= (val2))
#define RTC_CHECK_GT(val1, val2) assert((val1) > (val2))
#define RTC_CHECK_GE(val1, val2) assert((val1) >= (val2))

#define RTC_DCHECK(condition) assert(condition)
#define RTC_DCHECK_EQ(val1, val2) assert((val1) == (val2))
#define RTC_DCHECK_NE(val1, val2) assert((val1) != (val2))
#define RTC_DCHECK_LT(val1, val2) assert((val1) < (val2))
#define RTC_DCHECK_LE(val1, val2) assert((val1) <= (val2))
#define RTC_DCHECK_GT(val1, val2) assert((val1) > (val2))
#define RTC_DCHECK_GE(val1, val2) assert((val1) >= (val2))

#define RTC_CHECK_NOTREACHED() assert(false)
#define RTC_DCHECK_NOTREACHED() assert(false)

#define RTC_CHECK_NULL(ptr) assert((ptr) == nullptr)
#define RTC_DCHECK_NULL(ptr) assert((ptr) == nullptr)

#define RTC_CHECK_NOTNULL(ptr) assert((ptr) != nullptr)
#define RTC_DCHECK_NOTNULL(ptr) assert((ptr) != nullptr)

}  // namespace webrtc

#endif  // RTC_BASE_CHECKS_H_
