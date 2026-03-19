// Simple implementation of gtest_prod_util.h for compilation purposes
#ifndef RTC_BASE_GTEST_PROD_UTIL_H_
#define RTC_BASE_GTEST_PROD_UTIL_H_

// Define GTEST_PROD macro for compatibility
#define GTEST_PROD(test_case_name) friend class test_case_name##_Test

#endif  // RTC_BASE_GTEST_PROD_UTIL_H_
