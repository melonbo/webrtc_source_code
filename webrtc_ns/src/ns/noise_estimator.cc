/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/noise_estimator.h"

namespace webrtc {

NoiseEstimator::NoiseEstimator(const SuppressionParams& suppression_params)
    : suppression_params_(suppression_params) {
  prev_noise_spectrum_.fill(1.f);
  conservative_noise_spectrum_.fill(1.f);
  parametric_noise_spectrum_.fill(1.f);
  noise_spectrum_.fill(1.f);
}

void NoiseEstimator::PrepareAnalysis() {
  // No preparation needed in this simplified implementation
}

void NoiseEstimator::PreUpdate(int32_t num_analyzed_frames, const float* signal_spectrum, float signal_spectral_sum) {
  // Simplified noise estimation
  quantile_noise_estimator_.Update(signal_spectrum, 0.01f);
}

void NoiseEstimator::PostUpdate(const float* speech_probability, const float* signal_spectrum) {
  // Update noise spectrum based on speech probability
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    float learning_rate = 0.05f * (1.0f - speech_probability[i]);
    noise_spectrum_[i] = learning_rate * signal_spectrum[i] + (1.0f - learning_rate) * noise_spectrum_[i];
    conservative_noise_spectrum_[i] = std::max(noise_spectrum_[i], conservative_noise_spectrum_[i] * 0.99f);
  }

  // Update parametric noise spectrum (simplified)
  float total_energy = 0.f;
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    total_energy += noise_spectrum_[i];
  }
  float avg_energy = total_energy / kFftSizeBy2Plus1;
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    parametric_noise_spectrum_[i] = avg_energy;
  }

  // Update previous noise spectrum
  prev_noise_spectrum_ = noise_spectrum_;
}

}  // namespace webrtc