// Simple implementation of absl::StrCat for compilation purposes
#ifndef ABSL_STRINGS_STR_CAT_H_
#define ABSL_STRINGS_STR_CAT_H_

#include <string>
#include <sstream>

namespace absl {

// Simple implementation of StrCat that concatenates values into a string
template <typename... Args>
std::string StrCat(const Args&... args) {
  std::ostringstream oss;
  (oss << ... << args);
  return oss.str();
}

}  // namespace absl

#endif  // ABSL_STRINGS_STR_CAT_H_
