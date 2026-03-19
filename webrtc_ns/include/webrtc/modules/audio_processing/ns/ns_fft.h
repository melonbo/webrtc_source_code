/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_
#define MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_

#include <array>
#include <memory>

#include "modules/audio_processing/ns/ns_common.h"

namespace webrtc {

// FFT wrapper for noise suppression.
class NrFft {
 public:
  NrFft();
  ~NrFft();

  // Performs FFT on the input signal.
  void Fft(const std::array<float, kFftSize>& input,
           std::array<float, kFftSize>& real,
           std::array<float, kFftSize>& imag) const;

  // Performs inverse FFT.
  void Ifft(const std::array<float, kFftSize>& real,
            const std::array<float, kFftSize>& imag,
            std::array<float, kFftSize>& output) const;

 private:
  struct NrFftImpl;
  std::unique_ptr<NrFftImpl> impl_;
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_