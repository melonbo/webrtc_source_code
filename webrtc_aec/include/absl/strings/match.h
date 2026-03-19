// Simple implementation of absl::strings::match for compilation purposes
#ifndef ABSL_STRINGS_MATCH_H_
#define ABSL_STRINGS_MATCH_H_

#include "absl/strings/string_view.h"

namespace absl {
namespace strings {

// Simple implementation of StartsWith
inline bool StartsWith(string_view str, string_view prefix) {
  if (str.size() < prefix.size()) return false;
  for (size_t i = 0; i < prefix.size(); ++i) {
    if (str[i] != prefix[i]) return false;
  }
  return true;
}

// Simple implementation of EndsWith
inline bool EndsWith(string_view str, string_view suffix) {
  if (str.size() < suffix.size()) return false;
  for (size_t i = 0; i < suffix.size(); ++i) {
    if (str[str.size() - suffix.size() + i] != suffix[i]) return false;
  }
  return true;
}

}  // namespace strings
}  // namespace absl

#endif  // ABSL_STRINGS_MATCH_H_
