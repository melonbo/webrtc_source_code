// Simple implementation of thread_annotations.h for compilation purposes
#ifndef RTC_BASE_THREAD_ANNOTATIONS_H_
#define RTC_BASE_THREAD_ANNOTATIONS_H_

// Define thread annotation macros for compatibility
#define RTC_GUARDED_BY(x) 
#define RTC_PT_GUARDED_BY(x) 
#define RTC_REQUIRES(x) 
#define RTC_REQUIRES_SHARED(x) 
#define RTC_EXCLUSIVE_LOCKS_REQUIRED(...) 
#define RTC_SHARED_LOCKS_REQUIRED(...) 
#define RTC_LOCKS_EXCLUDED(...) 
#define RTC_SCOPED_LOCKABLE 
#define RTC_EXCLUSIVE_LOCK_FUNCTION(...) 
#define RTC_SHARED_LOCK_FUNCTION(...) 
#define RTC_UNLOCK_FUNCTION(...) 
#define RTC_ASSERT_EXCLUSIVE_LOCK(...) 
#define RTC_ASSERT_SHARED_LOCK(...) 
#define RTC_DCHECK_RUNS_SERIALIZED(x)  

#endif  // RTC_BASE_THREAD_ANNOTATIONS_H_
