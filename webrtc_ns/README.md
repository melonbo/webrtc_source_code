# WebRTC 噪声抑制模块

这是从WebRTC项目中提取的独立噪声抑制模块，可以轻松集成到其他项目中。

## 特性

- 基于频域的噪声抑制算法
- 支持16kHz、32kHz和48kHz采样率
- 支持单通道和多通道音频
- 可配置的降噪级别（6dB、12dB、18dB、21dB）
- 实时噪声估计和语音概率估计
- 维纳滤波器实现

## 文件结构

```
webrtc_ns/
├── include/webrtc/
│   ├── api/
│   │   └── array_view.h          # 数组视图工具类
│   └── modules/audio_processing/ns/
│       ├── noise_suppressor.h       # 主噪声抑制器
│       ├── noise_estimator.h        # 噪声估计器
│       ├── speech_probability_estimator.h  # 语音概率估计器
│       ├── wiener_filter.h          # 维纳滤波器
│       ├── ns_common.h              # 公共定义
│       ├── ns_config.h              # 配置结构
│       ├── ns_fft.h                 # FFT包装器
│       ├── suppression_params.h       # 抑制参数
│       ├── quantile_noise_estimator.h # 分位数噪声估计器
│       ├── signal_model_estimator.h   # 信号模型估计器
│       └── fast_math.h             # 快速数学函数
├── src/
│   ├── ns/                       # 实现文件
│   └── main.cpp                   # 示例程序
├── Makefile                       # Make构建文件
└── CMakeLists.txt                 # CMake构建文件
```

## 编译

### 使用Makefile

```bash
cd webrtc_ns
make
```

### 手动编译

```bash
cd webrtc_ns

# 编译所有源文件
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/noise_suppressor.cc -o noise_suppressor.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/noise_estimator.cc -o noise_estimator.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/speech_probability_estimator.cc -o speech_probability_estimator.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/wiener_filter.cc -o wiener_filter.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/ns_fft.cc -o ns_fft.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/suppression_params.cc -o suppression_params.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/quantile_noise_estimator.cc -o quantile_noise_estimator.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/ns/signal_model_estimator.cc -o signal_model_estimator.o
g++ -std=c++11 -Wall -O2 -I./include/webrtc -c src/main.cpp -o main.o

# 链接
g++ -o webrtc_ns_example noise_suppressor.o noise_estimator.o speech_probability_estimator.o wiener_filter.o ns_fft.o suppression_params.o quantile_noise_estimator.o signal_model_estimator.o main.o -lm
```

## 使用方法

### 基本使用

```cpp
#include "modules/audio_processing/ns/noise_suppressor.h"

// 配置降噪器
webrtc::NsConfig config;
config.target_level = webrtc::NsConfig::SuppressionLevel::k12dB; // 可选: k6dB, k12dB, k18dB, k21dB

// 创建降噪器实例
size_t sample_rate = 16000;  // 16kHz, 32kHz, 或 48kHz
size_t num_channels = 1;     // 通道数
webrtc::NoiseSuppressor ns(config, sample_rate, num_channels);

// 处理音频帧
size_t frame_size = 160;  // 10ms at 16kHz
float* audio_data = new float[frame_size * num_channels];

// 分析信号（通常在AEC之前调用）
ns.Analyze(audio_data, frame_size);

// 应用噪声抑制
ns.Process(audio_data, frame_size);

// audio_data现在包含降噪后的音频
```

### 集成到现有项目

1. 将`include/webrtc`目录添加到项目的include路径
2. 将`src/ns`目录下的所有`.cc`文件添加到项目
3. 链接数学库（`-lm`）

### 配置选项

```cpp
enum class SuppressionLevel { 
    k6dB,   // 轻度降噪
    k12dB,  // 中度降噪（推荐）
    k18dB,  // 强度降噪
    k21dB   // 最大降噪
};
```

## 示例程序

运行示例程序：

```bash
./webrtc_ns_example
```

示例程序生成一个包含440Hz正弦波和白色噪声的测试信号，然后应用噪声抑制。

## 技术细节

### 算法流程

1. **分析阶段**：
   - 对输入音频进行FFT变换
   - 计算信号频谱
   - 估计噪声频谱
   - 计算语音存在的概率

2. **处理阶段**：
   - 应用维纳滤波器进行噪声抑制
   - 对高频带应用额外的噪声衰减
   - 使用重叠相加法重建时域信号
   - 对输出进行限幅处理

### 核心组件

- **NoiseEstimator**: 实时估计背景噪声
- **SpeechProbabilityEstimator**: 判断当前帧是否包含语音
- **WienerFilter**: 基于信噪比的频域滤波
- **NrFft**: FFT/IFFT变换

## 注意事项

- 降噪效果需要一些帧来适应，前几帧可能效果不明显
- 对于强噪声环境，建议使用更高的降噪级别
- 降噪器会引入一些延迟，大约为帧大小（10ms）
- 对于实时应用，确保及时调用Analyze和Process方法

## 许可证

本代码基于WebRTC项目的BSD许可证。原始版权信息保留在源文件中。

## 参考资料

- [WebRTC项目](https://webrtc.org/)
- [音频处理模块文档](https://webrtc.googlesource.com/src/+/refs/heads/master/modules/audio_processing/)