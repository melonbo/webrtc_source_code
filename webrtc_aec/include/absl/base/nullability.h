// Simple implementation of absl::nullability for compilation purposes
#ifndef ABSL_BASE_NULLABILITY_H_
#define ABSL_BASE_NULLABILITY_H_

namespace absl {

// Nullability annotations
template <typename T>
using nullable = T;

template <typename T>
using nonnull = T;

}  // namespace absl

#endif  // ABSL_BASE_NULLABILITY_H_
