// Simple implementation of metrics.h for compilation purposes
#ifndef SYSTEM_WRAPPERS_INCLUDE_METRICS_H_
#define SYSTEM_WRAPPERS_INCLUDE_METRICS_H_

#include <string>
#include <memory>
#include <map>

namespace webrtc {

// Simple SampleInfo struct
struct SampleInfo {
  // Add necessary fields here
};

// Simple metrics implementation
class Metrics {
public:
  static Metrics* GetInstance();
  // Add necessary methods here
private:
  std::map<std::string, std::unique_ptr<SampleInfo>> sample_infos_;
};

}  // namespace webrtc

#endif  // SYSTEM_WRAPPERS_INCLUDE_METRICS_H_
