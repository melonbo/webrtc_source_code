/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_ARRAY_VIEW_H_
#define API_ARRAY_VIEW_H_

#include <stddef.h>
#include <type_traits>
#include <vector>

namespace webrtc {

// A read-only view of an array of T. The data is not owned, so the lifetime of
// the data must exceed the lifetime of the ArrayView.
//
// Use ArrayView<T> when you want to accept either a:
//   - T* + size
//   - std::vector<T>
//   - ArrayView<T>
//   - std::array<T, N>
//   - T[N] (implicitly decays to T* + size)
//
// and you only need to read from it. ArrayView is a lightweight value type with
// zero overhead compared to passing a T* and size.
//
// To pass a mutable view, use MutableArrayView<T>.

template <typename T>
class ArrayView {
 public:
  using value_type = T;
  using iterator = const T*;
  using const_iterator = const T*;
  using size_type = size_t;

  // Construct an empty ArrayView.
  ArrayView() : data_(nullptr), size_(0) {}

  // Construct an ArrayView from a pointer and size.
  ArrayView(const T* data, size_t size) : data_(data), size_(size) {}

  // Construct an ArrayView from a std::vector.
  template <typename A>
  ArrayView(const std::vector<T, A>& v) : data_(v.data()), size_(v.size()) {}

  // Construct an ArrayView from a std::array.
  template <size_t N>
  ArrayView(const std::array<T, N>& a) : data_(a.data()), size_(N) {}

  // Construct an ArrayView from a C array. The array size is deduced.
  template <size_t N>
  ArrayView(const T (&a)[N]) : data_(a), size_(N) {}

  // Construct an ArrayView from another ArrayView.
  ArrayView(const ArrayView&) = default;
  ArrayView& operator=(const ArrayView&) = default;

  // Return a pointer to the first element.
  const T* data() const { return data_; }

  // Return the number of elements.
  size_t size() const { return size_; }

  // Return whether the array is empty.
  bool empty() const { return size_ == 0; }

  // Return the i-th element.
  const T& operator[](size_t i) const { return data_[i]; }

  // Return the first element.
  const T& front() const { return data_[0]; }

  // Return the last element.
  const T& back() const { return data_[size_ - 1]; }

  // Return an iterator to the first element.
  const_iterator begin() const { return data_; }

  // Return an iterator past the last element.
  const_iterator end() const { return data_ + size_; }

 private:
  const T* data_;
  size_t size_;
};

// A mutable view of an array of T. The data is not owned, so the lifetime of
// the data must exceed the lifetime of the MutableArrayView.
//
// Use MutableArrayView<T> when you want to accept either a:
//   - T* + size
//   - std::vector<T>
//   - MutableArrayView<T>
//   - std::array<T, N>
//   - T[N] (implicitly decays to T* + size)
//
// and you need to modify the data. MutableArrayView is a lightweight value type
// with zero overhead compared to passing a T* and size.

template <typename T>
class MutableArrayView {
 public:
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = size_t;

  // Construct an empty MutableArrayView.
  MutableArrayView() : data_(nullptr), size_(0) {}

  // Construct a MutableArrayView from a pointer and size.
  MutableArrayView(T* data, size_t size) : data_(data), size_(size) {}

  // Construct a MutableArrayView from a std::vector.
  template <typename A>
  MutableArrayView(std::vector<T, A>& v) : data_(v.data()), size_(v.size()) {}

  // Construct a MutableArrayView from a std::array.
  template <size_t N>
  MutableArrayView(std::array<T, N>& a) : data_(a.data()), size_(N) {}

  // Construct a MutableArrayView from a C array. The array size is deduced.
  template <size_t N>
  MutableArrayView(T (&a)[N]) : data_(a), size_(N) {}

  // Construct a MutableArrayView from another MutableArrayView.
  MutableArrayView(const MutableArrayView&) = default;
  MutableArrayView& operator=(const MutableArrayView&) = default;

  // Implicit conversion to ArrayView<T> (loss of mutability).
  operator ArrayView<T>() const { return ArrayView<T>(data_, size_); }

  // Return a pointer to the first element.
  T* data() const { return data_; }

  // Return the number of elements.
  size_t size() const { return size_; }

  // Return whether the array is empty.
  bool empty() const { return size_ == 0; }

  // Return the i-th element.
  T& operator[](size_t i) const { return data_[i]; }

  // Return the first element.
  T& front() const { return data_[0]; }

  // Return the last element.
  T& back() const { return data_[size_ - 1]; }

  // Return an iterator to the first element.
  iterator begin() const { return data_; }

  // Return an iterator past the last element.
  iterator end() const { return data_ + size_; }

 private:
  T* data_;
  size_t size_;
};

// Helper function to create an ArrayView from a container.
template <typename T>
ArrayView<T> MakeArrayView(const T* data, size_t size) {
  return ArrayView<T>(data, size);
}

// Helper function to create an ArrayView from a std::vector.
template <typename T, typename A>
ArrayView<T> MakeArrayView(const std::vector<T, A>& v) {
  return ArrayView<T>(v);
}

// Helper function to create an ArrayView from a std::array.
template <typename T, size_t N>
ArrayView<T> MakeArrayView(const std::array<T, N>& a) {
  return ArrayView<T>(a);
}

// Helper function to create an ArrayView from a C array.
template <typename T, size_t N>
ArrayView<T> MakeArrayView(const T (&a)[N]) {
  return ArrayView<T>(a);
}

// Helper function to create a MutableArrayView from a container.
template <typename T>
MutableArrayView<T> MakeMutableArrayView(T* data, size_t size) {
  return MutableArrayView<T>(data, size);
}

// Helper function to create a MutableArrayView from a std::vector.
template <typename T, typename A>
MutableArrayView<T> MakeMutableArrayView(std::vector<T, A>& v) {
  return MutableArrayView<T>(v);
}

// Helper function to create a MutableArrayView from a std::array.
template <typename T, size_t N>
MutableArrayView<T> MakeMutableArrayView(std::array<T, N>& a) {
  return MutableArrayView<T>(a);
}

// Helper function to create a MutableArrayView from a C array.
template <typename T, size_t N>
MutableArrayView<T> MakeMutableArrayView(T (&a)[N]) {
  return MutableArrayView<T>(a);
}

}  // namespace webrtc

#endif  // API_ARRAY_VIEW_H_