/*
 *  Copyright 2023 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This header file provides wrapper for common WebRTC utilities.
// Different application may need different implementations of these utilities,
// Moreover, single application may need to use WebRTC for multiple purposes,
// and thus would need to provide different utilities implementations for
// different peer connections.
// The main purpose of the `Environment` class below is to propagate references
// to those utilities to all WebRTC classes that need them.

#ifndef API_ENVIRONMENT_ENVIRONMENT_H_
#define API_ENVIRONMENT_ENVIRONMENT_H_

#include <utility>

#include "absl/base/nullability.h"
#include "api/field_trials_view.h"
#include "api/ref_count.h"
#include "api/ref_counted_base.h"
#include "api/rtc_event_log/rtc_event_log.h"
#include "api/scoped_refptr.h"
#include "api/task_queue/task_queue_factory.h"
#include "rtc_base/system/rtc_export.h"
#include "system_wrappers/include/clock.h"

namespace webrtc {

// Simple implementation of FieldTrialsView for compilation purposes
class FieldTrialsViewImpl : public FieldTrialsView {
public:
  std::string Lookup(absl::string_view key) const override {
    return "";
  }
};
// Contains references to WebRTC utilities. Object of this class should be
// passed as a construction parameter and saved by value in each class that
// needs it. Most classes shouldn't create a new instance of the `Environment`,
// but instead should use a propagated copy.
// Usually Environment should be the first parameter in a constructor or a
// factory, and the first member in the class. Keeping Environment as the first
// member in the class ensures utilities (e.g. clock) are still valid during
// destruction of other members.
//
// Example:
//    class PeerConnection {
//     public:
//      PeerConnection(const Environment& env, ...)
//          : env_(env),
//            log_duration_on_destruction_(&env_.clock()),
//            rtp_manager_(env_, ...),
//            ...
//
//      const FieldTrialsView& trials() const { return env_.field_trials(); }
//
//      scoped_refptr<RtpTransceiverInterface> AddTransceiver(...) {
//        return make_ref_counted<RtpTransceiverImpl>(env_, ...);
//      }
//
//     private:
//      const Environment env_;
//      Stats log_duration_on_destruction_;
//      RtpTransmissionManager rtp_manager_;
//    };
// This class is thread safe.
class RTC_EXPORT Environment final {
 public:
  Environment() = default;
  Environment(const Environment&) = default;
  Environment(Environment&&) = default;
  Environment& operator=(const Environment&) = default;
  Environment& operator=(Environment&&) = default;

  ~Environment() = default;

  // Provides means to alter behavior, mostly for A/B testing new features.
  // See ../../g3doc/field-trials.md
  const FieldTrialsView& field_trials() const;

  // Provides an interface to query current time.
  // See ../../g3doc/implementation_basics.md#time
  Clock& clock() const;

  // Provides a factory for task queues, WebRTC threading primitives.
  // See ../../g3doc/implementation_basics.md#threads
  TaskQueueFactory& task_queue_factory() const;

  // Provides an interface for collecting structured logs.
  // See ../../logging/g3doc/rtc_event_log.md
  RtcEventLog& event_log() const;

  // Creates a default environment with dummy implementations
  static Environment Create() {
    return Environment();
  }

 private:
  friend class EnvironmentFactory;

  // Container that keeps ownership of the utilities below.
  scoped_refptr<const RefCountedBase> storage_;

  const FieldTrialsView* field_trials_ = nullptr;
  Clock* clock_ = nullptr;
  TaskQueueFactory* task_queue_factory_ = nullptr;
  RtcEventLog* event_log_ = nullptr;
};

//------------------------------------------------------------------------------
// Implementation details follow
//------------------------------------------------------------------------------

// Simple implementations for dummy objects
class DummyClock : public Clock {
public:
  ~DummyClock() override = default;
};

class DummyTaskQueueFactory : public TaskQueueFactory {
public:
  ~DummyTaskQueueFactory() override = default;
  void AddRef() const override {};
  RefCountReleaseStatus Release() const override { return RefCountReleaseStatus::kOtherRefsRemained; };
};

class DummyRtcEventLog : public RtcEventLog {
public:
  ~DummyRtcEventLog() override = default;
  void AddRef() const override {};
  RefCountReleaseStatus Release() const override { return RefCountReleaseStatus::kOtherRefsRemained; };
};

inline const FieldTrialsView& Environment::field_trials() const {
  static FieldTrialsViewImpl dummy;
  return field_trials_ ? *field_trials_ : dummy;
}

inline Clock& Environment::clock() const {
  static DummyClock dummy;
  return clock_ ? *clock_ : dummy;
}

inline TaskQueueFactory& Environment::task_queue_factory() const {
  static DummyTaskQueueFactory dummy;
  return task_queue_factory_ ? *task_queue_factory_ : dummy;
}

inline RtcEventLog& Environment::event_log() const {
  static DummyRtcEventLog dummy;
  return event_log_ ? *event_log_ : dummy;
}

}  // namespace webrtc

#endif  // API_ENVIRONMENT_ENVIRONMENT_H_
