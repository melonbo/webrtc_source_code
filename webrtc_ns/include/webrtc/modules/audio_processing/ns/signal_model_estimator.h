/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_NS_SIGNAL_MODEL_ESTIMATOR_H_
#define MODULES_AUDIO_PROCESSING_NS_SIGNAL_MODEL_ESTIMATOR_H_

#include <array>
#include <memory>

#include "modules/audio_processing/ns/ns_common.h"

namespace webrtc {

// Estimates the signal model parameters.
class SignalModelEstimator {
 public:
  SignalModelEstimator();
  ~SignalModelEstimator();

  // Updates the signal model estimate.
  void Update(const float* signal_spectrum, float signal_spectral_sum);

  // Returns the signal model parameters.
  float get_lrt_parameters(float* lrt_gain, float* lrt_offset) const;
  float get_spectral_flatness_parameter() const;
  float get_spectral_diff_parameter() const;

 private:
  struct SignalModelEstimatorImpl;
  std::unique_ptr<SignalModelEstimatorImpl> impl_;
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_NS_SIGNAL_MODEL_ESTIMATOR_H_