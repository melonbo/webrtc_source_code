/*
 *  Example application for WebRTC noise suppression
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "modules/audio_processing/ns/noise_suppressor.h"

// WAV file header structure
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

// Read WAV file
bool readWavFile(const std::string& filename, std::vector<float>& samples, size_t& sample_rate, size_t& num_channels) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }

  // Read RIFF header
  char chunkId[4];
  uint32_t chunkSize;
  char format[4];
  
  file.read(chunkId, 4);
  file.read(reinterpret_cast<char*>(&chunkSize), 4);
  file.read(format, 4);

  // Check if it's a valid WAV file
  if (chunkId[0] != 'R' || chunkId[1] != 'I' || chunkId[2] != 'F' || chunkId[3] != 'F') {
    std::cerr << "Error: Not a valid WAV file" << std::endl;
    return false;
  }

  if (format[0] != 'W' || format[1] != 'A' || format[2] != 'V' || format[3] != 'E') {
    std::cerr << "Error: Not a valid WAV file" << std::endl;
    return false;
  }

  // Read fmt subchunk
  char subchunk1Id[4];
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  
  file.read(subchunk1Id, 4);
  file.read(reinterpret_cast<char*>(&subchunk1Size), 4);
  file.read(reinterpret_cast<char*>(&audioFormat), 2);
  file.read(reinterpret_cast<char*>(&numChannels), 2);
  file.read(reinterpret_cast<char*>(&sampleRate), 4);
  file.read(reinterpret_cast<char*>(&byteRate), 4);
  file.read(reinterpret_cast<char*>(&blockAlign), 2);
  file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

  // Only support PCM format
  if (audioFormat != 1) {
    std::cerr << "Error: Only PCM format is supported" << std::endl;
    return false;
  }

  // Only support 16-bit audio
  if (bitsPerSample != 16) {
    std::cerr << "Error: Only 16-bit audio is supported" << std::endl;
    return false;
  }

  // Skip any extra format data
  if (subchunk1Size > 16) {
    file.seekg(subchunk1Size - 16, std::ios::cur);
  }

  // Find data subchunk
  char subchunk2Id[4];
  uint32_t subchunk2Size;
  
  while (true) {
    file.read(subchunk2Id, 4);
    if (file.eof()) {
      std::cerr << "Error: Could not find data subchunk" << std::endl;
      return false;
    }
    file.read(reinterpret_cast<char*>(&subchunk2Size), 4);
    if (subchunk2Id[0] == 'd' && subchunk2Id[1] == 'a' && subchunk2Id[2] == 't' && subchunk2Id[3] == 'a') {
      break;
    }
    // Skip other subchunks
    file.seekg(subchunk2Size, std::ios::cur);
  }

  sample_rate = sampleRate;
  num_channels = numChannels;

  // Calculate number of samples
  size_t num_samples = subchunk2Size / (bitsPerSample / 8) / num_channels;
  samples.resize(num_samples * num_channels);

  // Read samples
  std::vector<int16_t> int_samples(num_samples * num_channels);
  file.read(reinterpret_cast<char*>(int_samples.data()), subchunk2Size);

  // Convert to float
  for (size_t i = 0; i < int_samples.size(); ++i) {
    samples[i] = int_samples[i] / 32768.0f;
  }

  return true;
}

// Calculate SNR in dB
float calculateSNR(const std::vector<float>& signal, const std::vector<float>& noise) {
  float signal_power = 0.0f;
  float noise_power = 0.0f;
  
  for (size_t i = 0; i < signal.size(); ++i) {
    signal_power += signal[i] * signal[i];
    noise_power += noise[i] * noise[i];
  }
  
  if (noise_power < 1e-10) {
    return 99.9f; // Infinity
  }
  
  return 10.0f * std::log10(signal_power / noise_power);
}

// Calculate noise reduction in dB
float calculateNoiseReduction(const std::vector<float>& input, const std::vector<float>& output) {
  float input_power = 0.0f;
  float output_power = 0.0f;
  
  for (size_t i = 0; i < input.size(); ++i) {
    input_power += input[i] * input[i];
    output_power += output[i] * output[i];
  }
  
  if (output_power < 1e-10) {
    return 99.9f; // Infinity
  }
  
  return 10.0f * std::log10(input_power / output_power);
}

// Write WAV file
bool writeWavFile(const std::string& filename, const std::vector<float>& samples, size_t sample_rate, size_t num_channels) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }

  // Create header
  WavHeader header;
  memcpy(header.chunkId, "RIFF", 4);
  header.chunkSize = 36 + samples.size() * 2;
  memcpy(header.format, "WAVE", 4);
  memcpy(header.subchunk1Id, "fmt ", 4);
  header.subchunk1Size = 16;
  header.audioFormat = 1;
  header.numChannels = num_channels;
  header.sampleRate = sample_rate;
  header.byteRate = sample_rate * num_channels * 2;
  header.blockAlign = num_channels * 2;
  header.bitsPerSample = 16;
  memcpy(header.subchunk2Id, "data", 4);
  header.subchunk2Size = samples.size() * 2;

  // Write header
  file.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));

  // Convert to int16 and write
  std::vector<int16_t> int_samples(samples.size());
  for (size_t i = 0; i < samples.size(); ++i) {
    int_samples[i] = static_cast<int16_t>(samples[i] * 32767.0f);
  }
  file.write(reinterpret_cast<const char*>(int_samples.data()), samples.size() * 2);

  return true;
}

void printUsage() {
  std::cout << "Usage: webrtc_ns_example [input.wav] [output.wav] [options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -l LEVEL   Noise suppression level (6, 12, 18, 21)" << std::endl;
  std::cout << "  -h         Show this help" << std::endl;
  std::cout << "If no input file is provided, a test signal will be generated." << std::endl;
  std::cout << "If no output file is provided, results will be printed to console." << std::endl;
}

int main(int argc, char* argv[]) {
  // Default configuration
  webrtc::NsConfig config;
  config.target_level = webrtc::NsConfig::SuppressionLevel::k12dB;
  size_t sample_rate = 16000;
  size_t num_channels = 1;
  size_t frame_size = 160; // 10ms at 16kHz
  std::string input_file;
  std::string output_file;

  // Parse command line arguments
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-l") {
      if (i + 1 < argc) {
        int level = std::stoi(argv[i + 1]);
        switch (level) {
          case 6:
            config.target_level = webrtc::NsConfig::SuppressionLevel::k6dB;
            break;
          case 12:
            config.target_level = webrtc::NsConfig::SuppressionLevel::k12dB;
            break;
          case 18:
            config.target_level = webrtc::NsConfig::SuppressionLevel::k18dB;
            break;
          case 21:
            config.target_level = webrtc::NsConfig::SuppressionLevel::k21dB;
            break;
          default:
            std::cerr << "Error: Invalid suppression level. Use 6, 12, 18, or 21." << std::endl;
            return 1;
        }
        i++;
      } else {
        std::cerr << "Error: Missing suppression level value." << std::endl;
        return 1;
      }
    } else if (std::string(argv[i]) == "-h") {
      printUsage();
      return 0;
    } else if (input_file.empty()) {
      input_file = argv[i];
    } else if (output_file.empty()) {
      output_file = argv[i];
    } else {
      std::cerr << "Error: Unknown argument " << argv[i] << std::endl;
      printUsage();
      return 1;
    }
  }

  std::vector<float> input;
  std::vector<float> output;
  bool use_test_signal = input_file.empty();

  if (use_test_signal) {
    // Generate test signal (white noise + sine wave)
    input.resize(frame_size * num_channels);
    output.resize(frame_size * num_channels);
    std::cout << "Testing WebRTC noise suppression with test signal..." << std::endl;
  } else {
    // Read input file
    if (!readWavFile(input_file, input, sample_rate, num_channels)) {
      return 1;
    }
    output.resize(input.size());
    std::cout << "Processing audio file: " << input_file << std::endl;
    std::cout << "Sample rate: " << sample_rate << " Hz" << std::endl;
    std::cout << "Channels: " << num_channels << std::endl;
    std::cout << "Samples: " << input.size() / num_channels << std::endl;
  }

  // Create noise suppressor
  webrtc::NoiseSuppressor ns(config, sample_rate, num_channels);

  if (use_test_signal) {
    // Process 10 frames
    for (int i = 0; i < 10; ++i) {
      // Generate test signal: sine wave + noise
      std::vector<float> clean_signal(frame_size * num_channels);
      std::vector<float> noise_signal(frame_size * num_channels);
      
      for (size_t j = 0; j < frame_size; ++j) {
        float t = (i * frame_size + j) / static_cast<float>(sample_rate);
        float sine = 1.0f * std::sin(2.0f * static_cast<float>(M_PI) * 440.0f * t); // 440Hz sine wave
        float noise = 0.5f * (2.0f * rand() / RAND_MAX - 1.0f); // White noise
        clean_signal[j] = sine;
        noise_signal[j] = noise;
        input[j] = sine + noise;
      }

      // Process the frame
      ns.Analyze(input.data(), frame_size);
      ns.Process(input.data(), frame_size);

      // Copy to output
      std::copy(input.begin(), input.end(), output.begin());

      // Calculate energy before and after
      float energy_before = 0.0f;
      float energy_after = 0.0f;
      for (size_t j = 0; j < frame_size; ++j) {
        energy_before += input[j] * input[j];
        energy_after += output[j] * output[j];
      }

      // Calculate SNR
      float input_snr = calculateSNR(clean_signal, noise_signal);
      float output_snr = calculateSNR(clean_signal, output);
      float noise_reduction = calculateNoiseReduction(noise_signal, output);

      std::cout << "Frame " << i << ": "
                << "Energy before: " << energy_before << ", "
                << "Energy after: " << energy_after << ", "
                << "Input SNR: " << input_snr << " dB, "
                << "Output SNR: " << output_snr << " dB, "
                << "Noise reduction: " << noise_reduction << " dB" << std::endl;
    }
  } else {
    // Process the entire file in frames
    size_t processed_samples = 0;
    size_t total_frames = input.size() / (frame_size * num_channels);
    
    std::cout << "Processing " << total_frames << " frames..." << std::endl;
    
    // Create a copy of the input for noise reduction calculation
    std::vector<float> original_input = input;
    
    while (processed_samples + frame_size * num_channels <= input.size()) {
      // Process current frame
      ns.Analyze(&input[processed_samples], frame_size);
      ns.Process(&input[processed_samples], frame_size);
      
      // Copy to output
      std::copy(&input[processed_samples], &input[processed_samples + frame_size * num_channels], 
                &output[processed_samples]);
      
      processed_samples += frame_size * num_channels;
    }
    
    std::cout << "Processed " << processed_samples / num_channels << " samples" << std::endl;
    
    // Calculate noise reduction for the entire file
    float noise_reduction = calculateNoiseReduction(original_input, output);
    std::cout << "Noise reduction: " << noise_reduction << " dB" << std::endl;
  }

  // Write output file if specified
  if (!output_file.empty()) {
    if (writeWavFile(output_file, output, sample_rate, num_channels)) {
      std::cout << "Output written to: " << output_file << std::endl;
    } else {
      std::cerr << "Error: Could not write output file." << std::endl;
      return 1;
    }
  }

  std::cout << "Processing completed!" << std::endl;
  return 0;
}
