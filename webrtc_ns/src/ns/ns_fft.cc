/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/audio_processing/ns/ns_fft.h"

#include <cmath>
#include <complex>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace webrtc {

struct NrFft::NrFftImpl {
  void Fft(const std::array<float, kFftSize>& input, std::array<float, kFftSize>& real, std::array<float, kFftSize>& imag) const {
    std::vector<std::complex<float>> in(kFftSize);
    for (size_t i = 0; i < kFftSize; ++i) {
      in[i] = input[i];
    }
    fft(in, real, imag);
  }

  void Ifft(const std::array<float, kFftSize>& real, const std::array<float, kFftSize>& imag, std::array<float, kFftSize>& output) const {
    std::vector<std::complex<float>> in(kFftSize);
    for (size_t i = 0; i < kFftSize; ++i) {
      in[i] = std::complex<float>(real[i], imag[i]);
    }
    ifft(in, output);
  }

private:
  void fft(std::vector<std::complex<float>>& x, std::array<float, kFftSize>& real, std::array<float, kFftSize>& imag) const {
    size_t n = x.size();
    if (n == 1) return;

    std::vector<std::complex<float>> even(n/2), odd(n/2);
    for (size_t i = 0; i < n/2; ++i) {
      even[i] = x[2*i];
      odd[i] = x[2*i+1];
    }

    fft(even, real, imag);
    fft(odd, real, imag);

    for (size_t k = 0; k < n/2; ++k) {
      std::complex<float> t = std::polar(1.0f, -2.0f * static_cast<float>(M_PI) * k / n) * odd[k];
      x[k] = even[k] + t;
      x[k + n/2] = even[k] - t;
    }

    for (size_t i = 0; i < n; ++i) {
      real[i] = x[i].real();
      imag[i] = x[i].imag();
    }
  }

  void ifft(std::vector<std::complex<float>>& x, std::array<float, kFftSize>& output) const {
    size_t n = x.size();
    std::vector<std::complex<float>> conjugated(n);
    for (size_t i = 0; i < n; ++i) {
      conjugated[i] = std::conj(x[i]);
    }

    std::array<float, kFftSize> real, imag;
    fft(conjugated, real, imag);

    for (size_t i = 0; i < n; ++i) {
      output[i] = real[i] / n;
    }
  }
};

NrFft::NrFft() : impl_(std::unique_ptr<NrFftImpl>(new NrFftImpl())) {}

NrFft::~NrFft() = default;

void NrFft::Fft(const std::array<float, kFftSize>& input, std::array<float, kFftSize>& real, std::array<float, kFftSize>& imag) const {
  impl_->Fft(input, real, imag);
}

void NrFft::Ifft(const std::array<float, kFftSize>& real, const std::array<float, kFftSize>& imag, std::array<float, kFftSize>& output) const {
  impl_->Ifft(real, imag, output);
}

}  // namespace webrtc