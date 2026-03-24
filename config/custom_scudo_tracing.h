/*
 * Copyright (C) 2025 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include "private/bionic_systrace.h"

// Enable tracing on scudo.
#define SCUDO_SCOPED_TRACE(name) ScopedTrace(name)

static inline const char *
GetReleaseToOSTraceName(scudo::ReleaseToOS ReleaseType) {
  static const char *Names[] = {
      "Scudo releaseToOS normal", "Scudo releaseToOS force",
      "Scudo releaseToOS forceall", "Scudo releaseToOS forcefast"};

  return Names[static_cast<size_t>(ReleaseType)];
}

static inline const char *
GetPrimaryReleaseToOSTraceName(scudo::ReleaseToOS ReleaseType) {
  static const char *Names[] = {"Scudo primary releaseToOS normal",
                                "Scudo primary releaseToOS force",
                                "Scudo primary releaseToOS forceall",
                                "Scudo primary releaseToOS forcefast"};

  return Names[static_cast<size_t>(ReleaseType)];
}

static inline const char *
GetPrimaryReleaseToOSMaybeTraceName(scudo::ReleaseToOS ReleaseType) {
  static const char *Names[] = {"Scudo primary releaseToOSMaybe normal",
                                "Scudo primary releaseToOSMaybe force",
                                "Scudo primary releaseToOSMaybe forceall",
                                "Scudo primary releaseToOSMaybe forcefast"};

  return Names[static_cast<size_t>(ReleaseType)];
}

static inline const char *
GetSecondaryReleaseToOSTraceName(scudo::ReleaseToOS ReleaseType) {
  static const char *Names[] = {"Scudo secondary releaseToOS normal",
                                "Scudo secondary releaseToOS force",
                                "Scudo secondary releaseToOS forceall",
                                "Scudo secondary releaseToOS forcefast"};

  return Names[static_cast<size_t>(ReleaseType)];
}

static inline const char *GetSecondaryReleaseOlderThanTraceName() {
  return "Scudo secondary releaseOlderThan";
}
