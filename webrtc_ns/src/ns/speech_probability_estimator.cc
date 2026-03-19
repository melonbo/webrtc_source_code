/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/speech_probability_estimator.h"

#include <cmath>

namespace webrtc {

SpeechProbabilityEstimator::SpeechProbabilityEstimator() {
  speech_probability_.fill(0.5f);
}

void SpeechProbabilityEstimator::Update(
    int32_t num_analyzed_frames,
    const float* prior_snr,
    const float* post_snr,
    const float* conservative_noise_spectrum,
    const float* signal_spectrum,
    float signal_spectral_sum,
    float signal_energy) {
  // Update signal model
  signal_model_estimator_.Update(signal_spectrum, signal_spectral_sum);

  // Compute speech probability based on SNR
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    float snr = post_snr[i];
    // Simplified speech probability calculation
    speech_probability_[i] = 1.0f / (1.0f + expf(-(snr - 5.0f) / 2.0f));
  }

  // Update prior speech probability
  float current_prob = 0.f;
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    current_prob += speech_probability_[i];
  }
  current_prob /= kFftSizeBy2Plus1;
  prior_speech_prob_ = 0.9f * prior_speech_prob_ + 0.1f * current_prob;
}

}  // namespace webrtc