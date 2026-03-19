// Simple implementation of absl::HasAbslStringify for compilation purposes
#ifndef ABSL_STRINGS_HAS_ABSL_STRINGIFY_H_
#define ABSL_STRINGS_HAS_ABSL_STRINGIFY_H_

namespace absl {

// Default implementation: types do not have AbslStringify by default
template <typename T, typename = void>
struct HasAbslStringify : std::false_type {};

}  // namespace absl

#endif  // ABSL_STRINGS_HAS_ABSL_STRINGIFY_H_
