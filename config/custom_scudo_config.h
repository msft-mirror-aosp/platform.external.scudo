/*
 * Copyright (C) 2023 The Android Open Source Project
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

//===-- custom_scudo-config.h -----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

// Use a custom config instead of the config found in allocator_config.h
namespace scudo {

struct AndroidNormalConfig {
  static const bool MaySupportMemoryTagging = true;
  template <class A>
  using TSDRegistryT = TSDRegistrySharedT<A, 8U, 2U>; // Shared, max 8 TSDs.

  struct Primary {
    using SizeClassMap = AndroidSizeClassMap;
#if SCUDO_CAN_USE_PRIMARY64
    static const uptr RegionSizeLog = 28U;
    typedef u32 CompactPtrT;
    static const uptr CompactPtrScale = SCUDO_MIN_ALIGNMENT_LOG;
    static const uptr GroupSizeLog = 20U;
    static const bool EnableRandomOffset = true;
    static const uptr MapSizeIncrement = 1UL << 18;
#else
    static const uptr RegionSizeLog = 18U;
    static const uptr GroupSizeLog = 18U;
    typedef uptr CompactPtrT;
#endif
    static const s32 MinReleaseToOsIntervalMs = 1000;
    static const s32 MaxReleaseToOsIntervalMs = 1000;
  };
#if SCUDO_CAN_USE_PRIMARY64
  template <typename Config> using PrimaryT = SizeClassAllocator64<Config>;
#else
  template <typename Config> using PrimaryT = SizeClassAllocator32<Config>;
#endif

  struct Secondary {
    struct Cache {
      static const u32 EntriesArraySize = 256U;
      static const u32 QuarantineSize = 32U;
      static const u32 DefaultMaxEntriesCount = 32U;
      static const uptr DefaultMaxEntrySize = 2UL << 20;
      static const s32 MinReleaseToOsIntervalMs = 0;
      static const s32 MaxReleaseToOsIntervalMs = 1000;
    };
    template <typename Config> using CacheT = MapAllocatorCache<Config>;
  };

  template <typename Config> using SecondaryT = MapAllocator<Config>;
};

typedef AndroidNormalConfig Config;

} // namespace scudo
