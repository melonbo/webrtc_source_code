// Simple implementation of race_checker.h for compilation purposes
#ifndef RTC_BASE_RACE_CHECKER_H_
#define RTC_BASE_RACE_CHECKER_H_

#include "rtc_base/thread_annotations.h"

namespace webrtc {

// Simple RaceChecker implementation for compilation purposes
class RaceChecker {
public:
  RaceChecker() {}
  ~RaceChecker() {}
  
  void Acquire() const {}
  void Release() const {}
  
private:
  mutable volatile int access_count_ = 0;
  mutable volatile int accessing_thread_ = 0;
};

// Simple RaceCheckerScope implementation for compilation purposes
class RaceCheckerScope {
public:
  explicit RaceCheckerScope(const RaceChecker* race_checker) : race_checker_(race_checker) {
    if (race_checker_) {
      race_checker_->Acquire();
    }
  }
  
  ~RaceCheckerScope() {
    if (race_checker_) {
      race_checker_->Release();
    }
  }
  
private:
  const RaceChecker* const race_checker_;
};

// Simple RaceCheckerScopeDoNothing implementation for compilation purposes
class RaceCheckerScopeDoNothing {
public:
  explicit RaceCheckerScopeDoNothing(const RaceChecker* race_checker) {}
  ~RaceCheckerScopeDoNothing() {}
};

}  // namespace webrtc

#endif  // RTC_BASE_RACE_CHECKER_H_
