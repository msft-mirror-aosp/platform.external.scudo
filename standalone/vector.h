//===-- vector.h ------------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef SCUDO_VECTOR_H_
#define SCUDO_VECTOR_H_

#include "common.h"

#include <string.h>

namespace scudo {

// A low-level vector based on map. May incur a significant memory overhead for
// small vectors. The current implementation supports only POD types.
template <typename T> class VectorNoCtor {
public:
  constexpr void init(uptr InitialCapacity = 0) {
    Data = &LocalData[0];
    CapacityBytes = sizeof(LocalData);
    if (InitialCapacity > capacity())
      reserve(InitialCapacity);
  }
  void destroy() {
    if (Data != &LocalData[0])
      unmap(Data, CapacityBytes, 0, &MapData);
  }
  T &operator[](uptr I) {
    DCHECK_LT(I, Size);
    return Data[I];
  }
  const T &operator[](uptr I) const {
    DCHECK_LT(I, Size);
    return Data[I];
  }
  void push_back(const T &Element) {
    DCHECK_LE(Size, capacity());
    if (Size == capacity()) {
      const uptr NewCapacity = roundUpToPowerOfTwo(Size + 1);
      reallocate(NewCapacity);
    }
    memcpy(&Data[Size++], &Element, sizeof(T));
  }
  T &back() {
    DCHECK_GT(Size, 0);
    return Data[Size - 1];
  }
  void pop_back() {
    DCHECK_GT(Size, 0);
    Size--;
  }
  uptr size() const { return Size; }
  const T *data() const { return Data; }
  T *data() { return Data; }
  constexpr uptr capacity() const { return CapacityBytes / sizeof(T); }
  void reserve(uptr NewSize) {
    // Never downsize internal buffer.
    if (NewSize > capacity())
      reallocate(NewSize);
  }
  void resize(uptr NewSize) {
    if (NewSize > Size) {
      reserve(NewSize);
      memset(&Data[Size], 0, sizeof(T) * (NewSize - Size));
    }
    Size = NewSize;
  }

  void clear() { Size = 0; }
  bool empty() const { return size() == 0; }

  const T *begin() const { return data(); }
  T *begin() { return data(); }
  const T *end() const { return data() + size(); }
  T *end() { return data() + size(); }

private:
  void reallocate(uptr NewCapacity) {
    DCHECK_GT(NewCapacity, 0);
    DCHECK_LE(Size, NewCapacity);
    NewCapacity = roundUpTo(NewCapacity * sizeof(T), getPageSizeCached());
    T *NewData = reinterpret_cast<T *>(
        map(nullptr, NewCapacity, "scudo:vector", 0, &MapData));
    memcpy(NewData, Data, Size * sizeof(T));
    destroy();
    Data = NewData;
    CapacityBytes = NewCapacity;
  }

  T *Data = nullptr;
  T LocalData[256 / sizeof(T)] = {};
  uptr CapacityBytes = 0;
  uptr Size = 0;
  MapPlatformData MapData = {};
};

template <typename T> class Vector : public VectorNoCtor<T> {
public:
  constexpr Vector() { VectorNoCtor<T>::init(); }
  explicit Vector(uptr Count) {
    VectorNoCtor<T>::init(Count);
    this->resize(Count);
  }
  ~Vector() { VectorNoCtor<T>::destroy(); }
  // Disallow copies and moves.
  Vector(const Vector &) = delete;
  Vector &operator=(const Vector &) = delete;
  Vector(Vector &&) = delete;
  Vector &operator=(Vector &&) = delete;
};

} // namespace scudo

#endif // SCUDO_VECTOR_H_
