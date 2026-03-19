// Simple implementation of absl::string_view for compilation purposes
#ifndef ABSL_STRINGS_STRING_VIEW_H_
#define ABSL_STRINGS_STRING_VIEW_H_

#include <cstddef>
#include <string>

namespace absl {

class string_view {
public:
  string_view() : data_(nullptr), size_(0) {}
  string_view(const char* s) : data_(s), size_(0) {
    if (s) {
      while (s[size_]) ++size_;
    }
  }
  string_view(const char* s, size_t len) : data_(s), size_(len) {}
  string_view(const std::string& s) : data_(s.data()), size_(s.size()) {}

  const char* data() const { return data_; }
  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  char operator[](size_t i) const { return data_[i]; }
  const char* begin() const { return data_; }
  const char* end() const { return data_ + size_; }

private:
  const char* data_;
  size_t size_;
};

}  // namespace absl

#endif  // ABSL_STRINGS_STRING_VIEW_H_
