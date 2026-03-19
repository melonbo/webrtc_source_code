/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/signal_model_estimator.h"

#include <cmath>

namespace webrtc {

struct SignalModelEstimator::SignalModelEstimatorImpl {
  float lrt_gain = 1.0f;
  float lrt_offset = 0.0f;
  float spectral_flatness = 0.5f;
  float spectral_diff = 0.5f;
};

SignalModelEstimator::SignalModelEstimator() : impl_(std::unique_ptr<SignalModelEstimatorImpl>(new SignalModelEstimatorImpl())) {}

SignalModelEstimator::~SignalModelEstimator() = default;

void SignalModelEstimator::Update(const float* signal_spectrum, float signal_spectral_sum) {
  if (signal_spectral_sum < 0.0001f) {
    return;
  }

  // Compute spectral flatness
  float geometric_mean = 1.0f;
  float arithmetic_mean = 0.0f;
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    if (signal_spectrum[i] > 0.0f) {
      geometric_mean *= signal_spectrum[i];
    }
    arithmetic_mean += signal_spectrum[i];
  }
  geometric_mean = std::pow(geometric_mean, 1.0f / kFftSizeBy2Plus1);
  arithmetic_mean /= kFftSizeBy2Plus1;
  impl_->spectral_flatness = geometric_mean / (arithmetic_mean + 0.0001f);

  // Update LRT parameters (simplified)
  impl_->lrt_gain = 1.0f;
  impl_->lrt_offset = 0.0f;

  // Update spectral difference (simplified)
  impl_->spectral_diff = 0.5f;
}

float SignalModelEstimator::get_lrt_parameters(float* lrt_gain, float* lrt_offset) const {
  *lrt_gain = impl_->lrt_gain;
  *lrt_offset = impl_->lrt_offset;
  return 1.0f;
}

float SignalModelEstimator::get_spectral_flatness_parameter() const {
  return impl_->spectral_flatness;
}

float SignalModelEstimator::get_spectral_diff_parameter() const {
  return impl_->spectral_diff;
}

}  // namespace webrtc