#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cstring>

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
  // 打印文件路径
  std::cout << "Attempting to read file: " << filename << std::endl;
  
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return {};
  }

  // 检查文件大小
  file.seekg(0, std::ios::end);
  std::streampos fileSize = file.tellg();
  file.seekg(0, std::ios::beg);
  std::cout << "File size: " << fileSize << " bytes" << std::endl;
  
  if (fileSize < sizeof(WavHeader)) {
    std::cerr << filename << ", File too small to be a valid WAV file" << std::endl;
    return {};
  }

  // 读取WAV头
  file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));
  
  // 打印WAV文件头的各个字段
  std::cout << "WAV header fields:" << std::endl;
  std::cout << "chunkId: " << std::string(header.chunkId, 4) << std::endl;
  std::cout << "chunkSize: " << header.chunkSize << std::endl;
  std::cout << "format: " << std::string(header.format, 4) << std::endl;
  std::cout << "subchunk1Id: " << std::string(header.subchunk1Id, 4) << std::endl;
  std::cout << "subchunk1Size: " << header.subchunk1Size << std::endl;
  std::cout << "audioFormat: " << header.audioFormat << std::endl;
  std::cout << "numChannels: " << header.numChannels << std::endl;
  std::cout << "sampleRate: " << header.sampleRate << std::endl;
  std::cout << "byteRate: " << header.byteRate << std::endl;
  std::cout << "blockAlign: " << header.blockAlign << std::endl;
  std::cout << "bitsPerSample: " << header.bitsPerSample << std::endl;
  std::cout << "subchunk2Id: " << std::string(header.subchunk2Id, 4) << std::endl;
  std::cout << "subchunk2Size: " << header.subchunk2Size << std::endl;

  // 检查WAV文件头的标识符
  bool valid = true;
  if (std::string(header.chunkId, 4) != "RIFF") {
    std::cerr << filename << ", Invalid chunkId: " << std::string(header.chunkId, 4) << std::endl;
    valid = false;
  }
  
  if (std::string(header.format, 4) != "WAVE") {
    std::cerr << filename << ", Invalid format: " << std::string(header.format, 4) << std::endl;
    valid = false;
  }
  
  if (std::string(header.subchunk1Id, 4) != "fmt ") {
    std::cerr << filename << ", Invalid subchunk1Id: " << std::string(header.subchunk1Id, 4) << std::endl;
    valid = false;
  }
  
  if (!valid) {
    return {};
  }
  
  // 处理LIST子块
  if (std::string(header.subchunk2Id, 4) == "LIST") {
    std::cout << "Found LIST subchunk, skipping..." << std::endl;
    // 跳过LIST子块
    file.seekg(header.subchunk2Size, std::ios::cur);
    
    // 读取data子块头
    char dataChunkId[4];
    uint32_t dataChunkSize;
    file.read(dataChunkId, 4);
    file.read(reinterpret_cast<char*>(&dataChunkSize), 4);
    
    if (std::string(dataChunkId, 4) != "data") {
      std::cerr << filename << ", Invalid subchunkId after LIST: " << std::string(dataChunkId, 4) << std::endl;
      return {};
    }
    
    // 更新subchunk2Id和subchunk2Size
    std::memcpy(header.subchunk2Id, dataChunkId, 4);
    header.subchunk2Size = dataChunkSize;
    std::cout << "Updated subchunk2Id: " << std::string(header.subchunk2Id, 4) << std::endl;
    std::cout << "Updated subchunk2Size: " << header.subchunk2Size << std::endl;
  } else if (std::string(header.subchunk2Id, 4) != "data") {
    std::cerr << filename << ", Invalid subchunk2Id: " << std::string(header.subchunk2Id, 4) << std::endl;
    return {};
  }

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

  // 检查是否读取了足够的数据
  if (file.gcount() != static_cast<std::streamsize>(numSamples * sizeof(int16_t))) {
    std::cerr << "Failed to read audio data" << std::endl;
    std::cerr << "Expected: " << numSamples * sizeof(int16_t) << " bytes" << std::endl;
    std::cerr << "Read: " << file.gcount() << " bytes" << std::endl;
    return {};
  }

  // 转换为float
  for (size_t i = 0; i < numSamples; ++i) {
    samples[i] = intSamples[i] / 32768.0f;
  }

  // 打印WAV文件信息
  std::cout << "Read WAV file: " << filename << std::endl;
  std::cout << "Sample rate: " << header.sampleRate << " Hz" << std::endl;
  std::cout << "Channels: " << header.numChannels << std::endl;
  std::cout << "Bits per sample: " << header.bitsPerSample << std::endl;
  std::cout << "Number of samples: " << numSamples << std::endl;
  std::cout << "Subchunk2Size: " << header.subchunk2Size << std::endl;
  std::cout << "ChunkSize: " << header.chunkSize << std::endl;

  return samples;
}

// 写入WAV文件
void WriteWavFile(const std::string& filename, const std::vector<float>& samples, const WavHeader& header) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // 创建一个新的WAV头，确保字段是正确的
  WavHeader newHeader = header;
  
  // 确保WAV文件头的标识符是正确的
  std::memcpy(newHeader.chunkId, "RIFF", 4);
  std::memcpy(newHeader.format, "WAVE", 4);
  std::memcpy(newHeader.subchunk1Id, "fmt ", 4);
  std::memcpy(newHeader.subchunk2Id, "data", 4);
  
  // 确保subchunk1Size是正确的（对于PCM格式，应该是16）
  newHeader.subchunk1Size = 16;
  
  // 更新subchunk2Size字段，它应该是样本数 * 通道数 * (位深度 / 8)
  newHeader.subchunk2Size = static_cast<uint32_t>(samples.size() * newHeader.numChannels * (newHeader.bitsPerSample / 8));
  
  // 更新chunkSize字段，它应该是整个文件的大小减去8字节
  newHeader.chunkSize = sizeof(WavHeader) + newHeader.subchunk2Size - 8;
  
  // 更新byteRate字段，它应该是采样率 * 通道数 * (位深度 / 8)
  newHeader.byteRate = newHeader.sampleRate * newHeader.numChannels * (newHeader.bitsPerSample / 8);
  
  // 更新blockAlign字段，它应该是通道数 * (位深度 / 8)
  newHeader.blockAlign = newHeader.numChannels * (newHeader.bitsPerSample / 8);

  // 打印WAV文件头信息
  std::cout << "Writing WAV file: " << filename << std::endl;
  std::cout << "Sample rate: " << newHeader.sampleRate << " Hz" << std::endl;
  std::cout << "Channels: " << newHeader.numChannels << std::endl;
  std::cout << "Bits per sample: " << newHeader.bitsPerSample << std::endl;
  std::cout << "Number of samples: " << samples.size() << std::endl;
  std::cout << "Subchunk2Size: " << newHeader.subchunk2Size << std::endl;
  std::cout << "ChunkSize: " << newHeader.chunkSize << std::endl;
  std::cout << "ByteRate: " << newHeader.byteRate << std::endl;
  std::cout << "BlockAlign: " << newHeader.blockAlign << std::endl;

  // 写入WAV头
  file.write(reinterpret_cast<const char*>(&newHeader), sizeof(WavHeader));

  // 转换为int16_t并写入
  std::vector<int16_t> intSamples(samples.size());
  for (size_t i = 0; i < samples.size(); ++i) {
    // 确保值在有效范围内
    float clampedValue = std::max(-1.0f, std::min(1.0f, samples[i]));
    intSamples[i] = static_cast<int16_t>(clampedValue * 32767.0f);
  }

  file.write(reinterpret_cast<const char*>(intSamples.data()), samples.size() * sizeof(int16_t));

  // 检查写入是否成功
  if (file.good()) {
    std::cout << "Successfully wrote WAV file" << std::endl;
  } else {
    std::cerr << "Failed to write WAV file" << std::endl;
  }
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

  // 简单的回声消除模拟（只是复制捕获的音频）
  std::vector<float> outputSamples = captureSamples;

  // 写入输出文件
  WriteWavFile(outputFile, outputSamples, captureHeader);

  std::cout << "Processing completed successfully!" << std::endl;
  std::cout << "Input render file: " << renderFile << std::endl;
  std::cout << "Input capture file: " << captureFile << std::endl;
  std::cout << "Output file: " << outputFile << std::endl;
  std::cout << "Sample rate: " << renderHeader.sampleRate << " Hz" << std::endl;
  std::cout << "Processed " << outputSamples.size() << " samples" << std::endl;

  return 0;
}
