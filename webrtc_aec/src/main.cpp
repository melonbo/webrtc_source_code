#include "modules/audio_processing/aec3/echo_canceller3.h"
#include "api/audio/echo_canceller3_config.h"
#include "api/environment/environment.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

// WAV文件头结构
struct WavHeader {
  char chunkId[4];
  uint32_t chunkSize;
  char format[4];
  char subchunk1Id[4];
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  char subchunk2Id[4];
  uint32_t subchunk2Size;
};

// 读取WAV文件
std::vector<float> ReadWavFile(const std::string& filename, WavHeader& header) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return {};
  }

  // 读取WAV头
  file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

  // 检查文件格式
  if (header.audioFormat != 1) {
    std::cerr << "Only PCM WAV files are supported" << std::endl;
    return {};
  }

  if (header.bitsPerSample != 16) {
    std::cerr << "Only 16-bit WAV files are supported" << std::endl;
    return {};
  }

  // 计算样本数
  size_t numSamples = header.subchunk2Size / (header.numChannels * (header.bitsPerSample / 8));
  std::vector<float> samples(numSamples);

  // 读取音频数据
  std::vector<int16_t> intSamples(numSamples);
  file.read(reinterpret_cast<char*>(intSamples.data()), numSamples * sizeof(int16_t));

  // 转换为float
  for (size_t i = 0; i < numSamples; ++i) {
    samples[i] = intSamples[i] / 32768.0f;
  }

  return samples;
}

// 写入WAV文件
void WriteWavFile(const std::string& filename, const std::vector<float>& samples, const WavHeader& header) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // 写入WAV头
  file.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));

  // 转换为int16_t并写入
  std::vector<int16_t> intSamples(samples.size());
  for (size_t i = 0; i < samples.size(); ++i) {
    intSamples[i] = static_cast<int16_t>(samples[i] * 32767.0f);
  }

  file.write(reinterpret_cast<const char*>(intSamples.data()), samples.size() * sizeof(int16_t));
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: aec3_test <render_wav> <capture_wav> <output_wav>" << std::endl;
    return 1;
  }

  std::string renderFile = argv[1];
  std::string captureFile = argv[2];
  std::string outputFile = argv[3];

  // 读取WAV文件
  WavHeader renderHeader, captureHeader;
  std::vector<float> renderSamples = ReadWavFile(renderFile, renderHeader);
  std::vector<float> captureSamples = ReadWavFile(captureFile, captureHeader);

  if (renderSamples.empty() || captureSamples.empty()) {
    return 1;
  }

  // 检查采样率是否一致
  if (renderHeader.sampleRate != captureHeader.sampleRate) {
    std::cerr << "Sample rates of input files must match" << std::endl;
    return 1;
  }

  // 检查通道数是否一致
  if (renderHeader.numChannels != 1 || captureHeader.numChannels != 1) {
    std::cerr << "Only mono WAV files are supported" << std::endl;
    return 1;
  }

  // 创建AEC3配置
  webrtc::EchoCanceller3Config config;
  
  // 创建环境
  webrtc::Environment env = webrtc::Environment::Create();
  
  // 创建AEC3实例
  int sample_rate_hz = renderHeader.sampleRate;
  size_t num_render_channels = 1;
  size_t num_capture_channels = 1;
  
  auto aec3 = std::make_unique<webrtc::EchoCanceller3>(
      env,
      config,
      std::nullopt,
      sample_rate_hz,
      num_render_channels,
      num_capture_channels);

  // 处理音频帧
  const size_t kFrameSize = 160; // 10ms @ 16kHz
  std::vector<float> outputSamples(captureSamples.size());
  size_t processedSamples = 0;

  // 创建AudioBuffer实例
  webrtc::AudioBuffer renderBuffer(sample_rate_hz, 1, sample_rate_hz, 1, sample_rate_hz, 1);
  webrtc::AudioBuffer captureBuffer(sample_rate_hz, 1, sample_rate_hz, 1, sample_rate_hz, 1);

  while (processedSamples < captureSamples.size()) {
    size_t currentFrameSize = std::min(kFrameSize, captureSamples.size() - processedSamples);
    
    // 提取当前帧
    std::vector<float> renderFrame(renderSamples.begin() + processedSamples, 
                                   renderSamples.begin() + processedSamples + currentFrameSize);
    std::vector<float> captureFrame(captureSamples.begin() + processedSamples, 
                                    captureSamples.begin() + processedSamples + currentFrameSize);

    // 确保帧大小为kFrameSize
    if (renderFrame.size() < kFrameSize) {
      renderFrame.resize(kFrameSize, 0.0f);
    }
    if (captureFrame.size() < kFrameSize) {
      captureFrame.resize(kFrameSize, 0.0f);
    }

    // 填充AudioBuffer
    float* renderChannels[1] = { renderFrame.data() };
    float* captureChannels[1] = { captureFrame.data() };
    
    webrtc::StreamConfig config(sample_rate_hz, 1);
    renderBuffer.CopyFrom(renderChannels, config);
    captureBuffer.CopyFrom(captureChannels, config);

    // 处理音频
    aec3->ProcessCapture(&captureBuffer, &renderBuffer, false);

    // 从AudioBuffer获取处理结果
    float* const* outputChannels = captureBuffer.channels();

    // 保存处理结果
    for (size_t i = 0; i < currentFrameSize; ++i) {
      outputSamples[processedSamples + i] = outputChannels[0][i];
    }

    processedSamples += currentFrameSize;
  }

  // 写入输出文件
  WriteWavFile(outputFile, outputSamples, captureHeader);

  std::cout << "AEC3 processing completed successfully!" << std::endl;
  std::cout << "Input render file: " << renderFile << std::endl;
  std::cout << "Input capture file: " << captureFile << std::endl;
  std::cout << "Output file: " << outputFile << std::endl;
  std::cout << "Sample rate: " << sample_rate_hz << " Hz" << std::endl;
  std::cout << "Processed " << outputSamples.size() << " samples" << std::endl;

  return 0;
}