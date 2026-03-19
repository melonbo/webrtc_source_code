/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/suppression_params.h"

namespace webrtc {

SuppressionParams::SuppressionParams(NsConfig::SuppressionLevel level) {
  switch (level) {
    case NsConfig::SuppressionLevel::k6dB:
      over_subtraction_factor = 1.1f;
      gain_floor = 0.5f;
      minimum_attenuating_gain = 0.9f;
      attack_probability = 0.8f;
      release_probability = 0.8f;
      break;
    case NsConfig::SuppressionLevel::k12dB:
      over_subtraction_factor = 1.1f;
      gain_floor = 0.25f;
      minimum_attenuating_gain = 0.7f;
      attack_probability = 0.8f;
      release_probability = 0.8f;
      break;
    case NsConfig::SuppressionLevel::k18dB:
      over_subtraction_factor = 1.2f;
      gain_floor = 0.125f;
      minimum_attenuating_gain = 0.5f;
      attack_probability = 0.7f;
      release_probability = 0.7f;
      break;
    case NsConfig::SuppressionLevel::k21dB:
      over_subtraction_factor = 1.3f;
      gain_floor = 0.06f;
      minimum_attenuating_gain = 0.3f;
      attack_probability = 0.6f;
      release_probability = 0.6f;
      break;
  }
}

}  // namespace webrtc