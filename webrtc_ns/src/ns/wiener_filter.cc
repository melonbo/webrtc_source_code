/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/wiener_filter.h"

#include <cmath>

namespace webrtc {

WienerFilter::WienerFilter(const SuppressionParams& suppression_params)
    : suppression_params_(suppression_params) {
  spectrum_prev_process_.fill(1.f);
  initial_spectral_estimate_.fill(1.f);
  filter_.fill(1.f);
}

void WienerFilter::Update(
    int32_t num_analyzed_frames,
    const float* noise_spectrum,
    const float* prev_noise_spectrum,
    const float* parametric_noise_spectrum,
    const float* signal_spectrum) {
  // Compute Wiener filter gain
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    float snr = signal_spectrum[i] / (noise_spectrum[i] + 0.0001f);
    float gain = snr / (snr + 1.0f);
    // Apply gain floor
    filter_[i] = std::max(gain, suppression_params_.gain_floor);
  }

  // Update previous spectrum
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    spectrum_prev_process_[i] = signal_spectrum[i];
  }
}

float WienerFilter::ComputeOverallScalingFactor(int32_t num_analyzed_frames, float prior_speech_probability, float energy_before_filtering, float energy_after_filtering) const {
  // Simplified scaling factor calculation
  if (energy_before_filtering < 0.0001f) {
    return 1.0f;
  }
  float gain = std::sqrt(energy_after_filtering / energy_before_filtering);
  // Adjust based on speech probability
  return gain * (0.5f + 0.5f * prior_speech_probability);
}

}  // namespace webrtc