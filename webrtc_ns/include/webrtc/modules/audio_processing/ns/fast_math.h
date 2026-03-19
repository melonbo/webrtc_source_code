/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_NS_FAST_MATH_H_
#define MODULES_AUDIO_PROCESSING_NS_FAST_MATH_H_

#include <cmath>

namespace webrtc {

inline float SqrtFastApproximation(float x) {
  return sqrtf(x);
}

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_NS_FAST_MATH_H_