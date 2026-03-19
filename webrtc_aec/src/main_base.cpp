#include "modules/audio_processing/aec3/echo_canceller3.h"
#include "api/audio/echo_canceller3_config.h"
#include "api/environment/environment.h"

int main() {
  // 创建AEC3配置
  webrtc::EchoCanceller3Config config;
  
  // 创建环境
  webrtc::Environment env = webrtc::Environment::Create();
  
  // 创建AEC3实例
  int sample_rate_hz = 16000;
  size_t num_render_channels = 1;
  size_t num_capture_channels = 1;
  
  auto aec3 = std::make_unique<webrtc::EchoCanceller3>(
      env,
      config,
      std::nullopt,
      sample_rate_hz,
      num_render_channels,
      num_capture_channels);
  
  return 0;
}