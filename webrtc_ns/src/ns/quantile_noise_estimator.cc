/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/quantile_noise_estimator.h"

namespace webrtc {

struct QuantileNoiseEstimator::QuantileNoiseEstimatorImpl {
  std::array<float, kFftSizeBy2Plus1> noise_spectrum;

  QuantileNoiseEstimatorImpl() {
    noise_spectrum.fill(1.f);
  }
};

QuantileNoiseEstimator::QuantileNoiseEstimator() : impl_(std::unique_ptr<QuantileNoiseEstimatorImpl>(new QuantileNoiseEstimatorImpl())) {}

QuantileNoiseEstimator::~QuantileNoiseEstimator() = default;

void QuantileNoiseEstimator::Update(const float* signal_spectrum, float learning_rate) {
  for (size_t i = 0; i < kFftSizeBy2Plus1; ++i) {
    impl_->noise_spectrum[i] = learning_rate * signal_spectrum[i] + (1.0f - learning_rate) * impl_->noise_spectrum[i];
  }
}

const float* QuantileNoiseEstimator::get_noise_spectrum() const {
  return impl_->noise_spectrum.data();
}

}  // namespace webrtc