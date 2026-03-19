/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_NS_QUANTILE_NOISE_ESTIMATOR_H_
#define MODULES_AUDIO_PROCESSING_NS_QUANTILE_NOISE_ESTIMATOR_H_

#include <array>
#include <memory>

#include "modules/audio_processing/ns/ns_common.h"

namespace webrtc {

// Estimates the noise spectrum using a quantile-based approach.
class QuantileNoiseEstimator {
 public:
  QuantileNoiseEstimator();
  ~QuantileNoiseEstimator();

  // Updates the noise estimate.
  void Update(const float* signal_spectrum, float learning_rate);

  // Returns the noise estimate.
  const float* get_noise_spectrum() const;

 private:
  struct QuantileNoiseEstimatorImpl;
  std::unique_ptr<QuantileNoiseEstimatorImpl> impl_;
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_NS_QUANTILE_NOISE_ESTIMATOR_H_