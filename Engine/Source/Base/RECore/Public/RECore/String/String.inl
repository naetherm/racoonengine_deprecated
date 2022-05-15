////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/Utility/Move.h"
#include "RECore/Memory/Memory.h"
#include "RECore/String/StringUtils.h"
#include "RECore/Utility/Algorithm.h"
#include "RECore/Utility/Utility.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <algorithm>
#include <utility>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::Layout::Layout() {
  this->ResetToSSO();
  this->SetSSOSize(0);
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::Layout::Layout(const BasicString::Layout &cSource) {
  this->copy(*this, cSource);
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::Layout::Layout(BasicString::Layout &&cSource) {
  this->Move(*this, cSource);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Layout &BasicString<T, TAllocator>::Layout::operator=(const BasicString::Layout &cSource) {
  this->copy(*this, cSource);

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Layout &BasicString<T, TAllocator>::Layout::operator=(BasicString::Layout &&cSource) {
  this->Move(*this, cSource);

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::Layout::GetSize() const noexcept {
  return this->IsHeap() ? this->GetHeapSize() : this->GetSSOSize();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::Layout::GetSSOSize() const noexcept {
#ifdef RE_SYSTEM_BIG_ENDIAN
  return ((SSOLayout::SSO_CAPACITY - this->sso.mRemainingSizeField.mnRemainingSize) >> 2);
#else
  return (SSOLayout::SSO_CAPACITY - this->sso.mRemainingSizeField.mnRemainingSize);
#endif
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::Layout::GetHeapSize() const noexcept {
  return this->heap.mnSize;
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::SetSSOSize(BasicString<T, TAllocator>::SizeType nSize) {
#ifdef RE_SYSTEM_BIG_ENDIAN
  sso.mRemainingSizeField.mnRemainingSize = (char)((SSOLayout::SSO_CAPACITY - nSize) << 2);
#else
  sso.mRemainingSizeField.mnRemainingSize = (char)(SSOLayout::SSO_CAPACITY - nSize);
#endif
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::SetHeapSize(BasicString<T, TAllocator>::SizeType nSize) {
  this->heap.mnSize = nSize;
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::SetSize(BasicString<T, TAllocator>::SizeType nSize) {
  this->IsHeap() ? this->SetHeapSize(nSize) : this->SetSSOSize(nSize);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::Layout::GetRemainingCapacity() const noexcept {
  return SizeType (CapacityPtr() - EndPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::HeapBeginPtr() noexcept {
  return heap.mpBegin;
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::HeapBeginPtr() const noexcept {
  return heap.mpBegin;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::SSOBeginPtr() noexcept {
  return sso.mData;
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::SSOBeginPtr() const noexcept {
  return sso.mData;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::BeginPtr() noexcept {
  return IsHeap() ? HeapBeginPtr() : SSOBeginPtr();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::BeginPtr() const noexcept {
  return IsHeap() ? HeapBeginPtr() : SSOBeginPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::HeapEndPtr() noexcept {
  return heap.mpBegin + heap.mnSize;
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::HeapEndPtr() const noexcept {
  return heap.mpBegin + heap.mnSize;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::SSOEndPtr() noexcept {
  return sso.mData + GetSSOSize();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::SSOEndPtr() const noexcept {
  return sso.mData + GetSSOSize();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::EndPtr() noexcept {
  return IsHeap() ? HeapEndPtr() : SSOEndPtr();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::EndPtr() const noexcept {
  return IsHeap() ? HeapEndPtr() : SSOEndPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::HeapCapacityPtr() noexcept {
  return heap.mpBegin + GetHeapCapacity();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::HeapCapacityPtr() const noexcept {
  return heap.mpBegin + GetHeapCapacity();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::SSOCapacityPtr() noexcept {
  return sso.mData + SSOLayout::SSO_CAPACITY;
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::SSOCapacityPtr() const noexcept {
  return sso.mData + SSOLayout::SSO_CAPACITY;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::CapacityPtr() noexcept {
  return IsHeap() ? HeapCapacityPtr() : SSOCapacityPtr();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::Layout::CapacityPtr() const noexcept {
  return IsHeap() ? HeapCapacityPtr() : SSOCapacityPtr();
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::SetHeapBeginPtr(ValueType *pBegin) noexcept {
  heap.mpBegin = pBegin;
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::SetHeapCapacity(SizeType cap) noexcept {
#ifdef RE_SYSTEM_BIG_ENDIAN
  heap.mnCapacity = (cap << 1) | kHeapMask;
#else
  heap.mnCapacity = (cap | kHeapMask);
#endif
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::Layout::GetHeapCapacity() const noexcept {
#ifdef RE_SYSTEM_BIG_ENDIAN
  return (heap.mnCapacity >> 1);
#else
  return (heap.mnCapacity & ~kHeapMask);
#endif
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::Copy(Layout &dst, const Layout &src) noexcept {
  dst.raw = src.raw;
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::Move(Layout &dst, Layout &src) noexcept {
  RECore::Swap(dst.raw, src.raw);
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::Swap(Layout &a, Layout &b) noexcept {
  RECore::Swap(a.raw, b.raw);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::Layout::IsSSO() const noexcept {
  return !(this->IsHeap());
}



template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::Layout::ResetToSSO() noexcept {
  RECore::Memory::Set(&raw, 0, sizeof(RawLayout));
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::Layout::IsHeap() const noexcept {
  return !(!(sso.mRemainingSizeField.mnRemainingSize & kSSOMask));
}



template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString() noexcept
  : mPair(ThisType::AllocatorType()) {
  this->allocateSelf();
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(
  const BasicString<T, TAllocator>::ThisType &cSource,
  BasicString<T, TAllocator>::SizeType nPos,
  BasicString<T, TAllocator>::SizeType nNum)
  : mPair(cSource.getAllocator()) {
  this->rangeInitialize(cSource.InternalLayout().BeginPtr() + nPos,
                        cSource.InternalLayout().BeginPtr() + nPos + std::min(nNum, cSource.InternalLayout().GetSize() - nPos));
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(ValueType c, const AllocatorType &alloc)
  : mPair(alloc) {
  // Range based initialization from the start to start + nSize
  this->sizeInitialize(1, c);
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(const ValueType *p, const AllocatorType &alloc)
  : mPair(alloc) {
  // Range based initialization from the start to start + nSize
  this->rangeInitialize(p);
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(ValueType *p, const AllocatorType &alloc)
  : mPair(alloc) {
  if (p == nullptr) {
    //this->assign("");
  } else {
    // Range based initialization from the start to start + nSize
    this->rangeInitialize(p);
  }
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(const ValueType *p, SizeType nSize, const AllocatorType &alloc)
  : mPair(alloc) {
  if (p == nullptr) {
    //this->assign("");
  } else {
    // Range based initialization from the start to start + nSize
    this->rangeInitialize(p, p + nSize);
  }
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(SizeType n, ValueType c, const AllocatorType &alloc)
  : mPair(alloc) {
  this->sizeInitialize(n, c);
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(const ValueType *pBegin, const ValueType *pEnd, const AllocatorType &alloc)
  : mPair(alloc) {
  this->rangeInitialize(pBegin, pEnd);
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(const ThisType &cSource)
  : mPair(cSource.getAllocator()) {
  if (cSource.length() > 0)
    this->rangeInitialize(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(const ThisType &cSource, const AllocatorType &alloc)
  : mPair(alloc) {
  this->rangeInitialize(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(ThisType &&cSource)
  : mPair(cSource.getAllocator()) {
  // Just move the layout
  this->InternalLayout() = ::std::move(cSource.InternalLayout());

  // 'Clean' the other string
  cSource.allocateSelf();
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::BasicString(ThisType &&cSource, const AllocatorType &alloc)
  : mPair(alloc) {
  //: mAllocator(alloc) {
  if (this->getAllocator() == cSource.getAllocator()) {
    this->InternalLayout() = ::std::move(cSource.InternalLayout());

    cSource.allocateSelf();
  } else {
    this->rangeInitialize(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
  }
}

template<typename T, typename TAllocator>
BasicString<T, TAllocator>::~BasicString() {
  this->deallocateSelf();
}




template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(const ThisType &cSource) {
  if (&cSource != this) {
    //this->getAllocator() = cSource.getAllocator();
    this->assign(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
  }
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(ThisType &&cSource) {
  if (&cSource != this) {
    //this->getAllocator() = cSource.getAllocator();
    this->assign(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
  }
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(const ValueType *p) {
  return this->assign(p, p + CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(ValueType c) {
  return this->assign((SizeType)1, c);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(ValueType *p) {
  return this->assign(p, p + CharStrlen(p));
}

template<typename T, typename TAllocator>
template<typename TOtherType>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(const TOtherType *p) {
  return this->assignConvert(p, CharStrlen(p));
}

template<typename T, typename TAllocator>
template<typename TOtherType>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(const TOtherType &cSource) {
  return this->assignConvert(cSource);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(uint8 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(uint16 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(uint32 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(uint64 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(int16 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(int32 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(int64 c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(float c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator=(double c) {
  this->append(c);
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Reference BasicString<T, TAllocator>::operator[](SizeType n) {
  return this->InternalLayout().BeginPtr()[n];
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReference BasicString<T, TAllocator>::operator[](SizeType n) const {
  return this->InternalLayout().BeginPtr()[n];
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(const ThisType &cSource) {
  return this->append(cSource);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(const ValueType *p) {
  return this->append(p);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(ValueType c) {
  return this->append((SizeType)1, c);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(uint8 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(uint16 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(uint32 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(uint64 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(int16 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(int32 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(int64 c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(float c) {
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::operator+=(double c) {
  return *this;
}


template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::swap(BasicString::ThisType &cSource) {
  // Only if both are inequal
  if (this->getAllocator() == cSource.getAllocator() || (this->InternalLayout().IsSSO() && cSource.InternalLayout().IsSSO())) {
    RECore::Swap(this->InternalLayout(), cSource.InternalLayout());
  } else {
    const ThisType temp(*this);
    *this = cSource;
    cSource = temp;
  }
}



template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::getBool() const {
  return this->getInt();
}

template<typename T, typename TAllocator>
sizeT BasicString<T, TAllocator>::getInt() const {
  return atoi(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
float BasicString<T, TAllocator>::getFloat() const {
  return atof(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
double BasicString<T, TAllocator>::getDouble() const {
  return atof(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
uint_ptr BasicString<T, TAllocator>::getUIntPtr() const {
  return 0;
}

template<typename T, typename TAllocator>
char BasicString<T, TAllocator>::getChar() const {
  return this->InternalLayout().BeginPtr()[0];
}



template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstIterator BasicString<T, TAllocator>::cbegin() const noexcept {
  return this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstIterator BasicString<T, TAllocator>::cend() const noexcept {
  return this->InternalLayout().EndPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ReverseIterator BasicString<T, TAllocator>::rbegin() noexcept {
  return RECore::BasicString<T, TAllocator>::ReverseIterator(this->InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReverseIterator BasicString<T, TAllocator>::rbegin() const noexcept {
  return RECore::BasicString<T, TAllocator>::ConstReverseIterator(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReverseIterator BasicString<T, TAllocator>::crbegin() const noexcept {
  return RECore::BasicString<T, TAllocator>::ConstReverseIterator(this->InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ReverseIterator BasicString<T, TAllocator>::rend() noexcept {
  return RECore::BasicString<T, TAllocator>::ReverseIterator(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReverseIterator BasicString<T, TAllocator>::rend() const noexcept {
  return RECore::BasicString<T, TAllocator>::ConstReverseIterator(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReverseIterator BasicString<T, TAllocator>::crend() const noexcept {
  return RECore::BasicString<T, TAllocator>::ConstReverseIterator(this->InternalLayout().BeginPtr());
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::clear() noexcept {
  this->InternalLayout().SetSize(0);
  *this->InternalLayout().BeginPtr() = ValueType (0);
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::forceSize(BasicString::SizeType n) {
  this->InternalLayout().SetSize(n);
}


template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assign(const ThisType &cSource) {
  return this->assign(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::assign(const ThisType &cSource, SizeType nPos, SizeType nNum) {
  return this->assign(
    cSource.InternalLayout().BeginPtr() + nPos,
    cSource.InternalLayout().BeginPtr() + nPos + std::min(nNum, cSource.InternalLayout().GetSize() - nPos)
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assign(const ValueType *p, SizeType nNum) {
  return this->assign(p, p + nNum);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assign(SizeType nNum, ValueType c) {
  if(nNum <= this->InternalLayout().GetSize())
  {
    CharTypeAssignN(this->InternalLayout().BeginPtr(), nNum, c);
    this->erase(this->InternalLayout().BeginPtr() + nNum, this->InternalLayout().EndPtr());
  }
  else
  {
    CharTypeAssignN(this->InternalLayout().BeginPtr(), this->InternalLayout().GetSize(), c);
    this->append(nNum - this->InternalLayout().GetSize(), c);
  }
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assign(const ValueType *pB, const ValueType *pE) {
  const SizeType n = (SizeType)(pE - pB);

  if (n < this->InternalLayout().GetSize()) {
    RECore::Memory::Move(this->InternalLayout().BeginPtr(), pB, (sizeT)n * sizeof(ValueType));
    this->erase(this->InternalLayout().BeginPtr() + n, this->InternalLayout().EndPtr());
  } else {
    RECore::Memory::Move(this->InternalLayout().BeginPtr(), pB, (sizeT)(this->InternalLayout().GetSize()) * sizeof(ValueType));
    this->append(pB + this->InternalLayout().GetSize(), pE);
  }

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assign(ThisType &&cSource) {
  // First check if the allocator are equal -> if that the cae we can simply do a swap
  if (this->getAllocator() == cSource.getAllocator()) {

  } else {
    this->assign(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
  }

  return *this;
}

template<typename T, typename TAllocator>
template<typename TOther>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assignConvert(const TOther *p) {
  this->clear();
  this->appendConvert(p);
  return *this;
}

template<typename T, typename TAllocator>
template<typename TOther>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assignConvert(const TOther *p, SizeType nSize) {
  this->clear();
  this->appendConvert(p, nSize);
  return *this;
}

template<typename T, typename TAllocator>
template<typename TOther>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::assignConvert(const TOther &cSource) {
  this->clear();
  this->appendConvert(cSource.cstr(), cSource.length());
  return *this;
}



template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::insert(SizeType nPos, const ThisType &cSource) {
  this->insert(this->InternalLayout().BeginPtr() + nPos,
               cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr()
  );
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::insert(SizeType nPos, const ThisType &cSource, SizeType nBegin, SizeType nNum) {
  SizeType nLength = std::min(nNum, cSource.InternalLayout().GetSize() + nBegin);

  this->insert(this->InternalLayout().BeginPtr() + nPos,
               cSource.InternalLayout().BeginPtr() + nBegin, cSource.InternalLayout().BeginPtr() + nBegin + nLength
  );
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::insert(SizeType nPos, const ValueType *p, SizeType nNum) {
  this->insert(this->InternalLayout().BeginPtr() + nPos, p, p + nNum);

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::insert(SizeType nPos, const ValueType *p) {
  SizeType nLength = (SizeType)CharStrlen(p);

  this->insert(this->InternalLayout().BeginPtr() + nPos, p, p + nLength);

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::insert(SizeType nPos, SizeType n, ValueType c) {
  this->insert(this->InternalLayout().BeginPtr() + nPos, n, c);

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator BasicString<T, TAllocator>::insert(ConstIterator p, ValueType c) {
  if (p == this->InternalLayout().EndPtr()) {
    this->pushBack(c);

    return this->InternalLayout().EndPtr() - 1;
  }

  return this->_insert(p, c);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator
BasicString<T, TAllocator>::insert(ConstIterator p, SizeType nNum, ValueType c) {
  const DifferenceType nPos = (p - this->InternalLayout().BeginPtr());

  // Is there anything to set at all?
  if (nNum) {
    if (this->InternalLayout().GetRemainingCapacity() >= nNum) {
      const SizeType nElementsAfter = (SizeType)(this->InternalLayout().EndPtr() - p);

      if (nElementsAfter >= nNum) {
        const SizeType nSavedSize = this->InternalLayout().GetSize();
        CharStringUninitializedCopy((this->InternalLayout().EndPtr() - nNum) + 1, this->InternalLayout().EndPtr() + 1,
                                    this->InternalLayout().EndPtr() + 1);
        this->InternalLayout().SetSize(nSavedSize + nNum);
        RECore::Memory::Move(const_cast<ValueType *>(p) + nNum, p, (size_t) ((nElementsAfter - nNum) + 1) * sizeof(ValueType));
        CharTypeAssignN(const_cast<ValueType *>(p), nNum, c);
      } else {
        Pointer pOldEnd = this->InternalLayout().EndPtr();

        CharStringUninitializedFillN(this->InternalLayout().EndPtr() + 1, nNum - nElementsAfter - 1, c);
        this->InternalLayout().SetSize(this->InternalLayout().GetSize() + (nNum - nElementsAfter));

        const SizeType nSavedSize = this->InternalLayout().GetSize();
        CharStringUninitializedCopy(p, pOldEnd + 1, this->InternalLayout().EndPtr());
        this->InternalLayout().SetSize(nSavedSize + nElementsAfter);

        CharTypeAssignN(const_cast<ValueType *>(p), nElementsAfter + 1, c);
      }
    } else {
      const SizeType nOldSize = this->InternalLayout().GetSize();
      const SizeType nOldCap = this->capacity();
      const SizeType nLength = std::max(getNewCapacity(nOldCap), nOldSize + nNum);

      Iterator pNewBegin = doAllocate(nLength + 1);

      Iterator pNewEnd = CharStringUninitializedCopy(this->InternalLayout().BeginPtr(), p, pNewBegin);
      pNewEnd = CharStringUninitializedFillN(pNewEnd, nNum, c);
      pNewEnd = CharStringUninitializedCopy(p, this->InternalLayout().EndPtr(), pNewEnd);
      *pNewEnd = 0;

      deallocateSelf();
      this->InternalLayout().SetHeapBeginPtr(pNewBegin);
      this->InternalLayout().SetHeapCapacity(nLength);
      this->InternalLayout().SetHeapSize(nOldSize + nNum);
    }
  }

  return this->InternalLayout().BeginPtr() + nPos;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator
BasicString<T, TAllocator>::insert(BasicString::ConstIterator p, const ValueType *pBegin, const ValueType *pEnd) {
  const DifferenceType nPos = (p - this->InternalLayout().BeginPtr());

  const SizeType n = (SizeType)(pEnd - pBegin);

  if(n) {
    const bool bCapacityIsSufficient = (this->InternalLayout().GetRemainingCapacity() >= n);
    const bool bSourceIsFromSelf     = ((pEnd >= this->InternalLayout().BeginPtr()) && (pBegin <= this->InternalLayout().EndPtr()));

    if(bSourceIsFromSelf && this->InternalLayout().IsSSO()) {
      const ThisType stackTemp(pBegin, pEnd, AllocatorType ());
      return this->insert(p, stackTemp.data(), stackTemp.data() + stackTemp.size());
    }

    if(bCapacityIsSufficient && !bSourceIsFromSelf) {
      const SizeType nElementsAfter = (SizeType)(this->InternalLayout().EndPtr() - p);

      if(nElementsAfter >= n) {
        const SizeType nSavedSize = this->InternalLayout().GetSize();
        CharStringUninitializedCopy((this->InternalLayout().EndPtr() - n) + 1, this->InternalLayout().EndPtr() + 1, this->InternalLayout().EndPtr() + 1);
        this->InternalLayout().SetSize(nSavedSize + n);
        RECore::Memory::Move(const_cast<ValueType *>(p) + n, p, (size_t)((nElementsAfter - n) + 1) * sizeof(ValueType));
        RECore::Memory::Move(const_cast<ValueType*>(p), pBegin, (size_t)(n) * sizeof(ValueType));
      } else {
        Pointer pOldEnd = this->InternalLayout().EndPtr();
        const ValueType* const pMid = pBegin + (nElementsAfter + 1);

        CharStringUninitializedCopy(pMid, pEnd, this->InternalLayout().EndPtr() + 1);
        this->InternalLayout().SetSize(this->InternalLayout().GetSize() + (n - nElementsAfter));

        // See comment in if block above
        const SizeType nSavedSize = this->InternalLayout().GetSize();
        CharStringUninitializedCopy(p, pOldEnd + 1, this->InternalLayout().EndPtr());
        this->InternalLayout().SetSize(nSavedSize + nElementsAfter);

        CharStringUninitializedCopy(pBegin, pMid, const_cast<ValueType*>(p));
      }
    } else {
      const SizeType nOldSize = this->InternalLayout().GetSize();
      const SizeType nOldCap  = this->capacity();
      SizeType nLength;

      if(bCapacityIsSufficient)
        nLength = nOldSize + n;
      else
        nLength = std::max(getNewCapacity(nOldCap), (nOldSize + n));

      Pointer pNewBegin = doAllocate(nLength + 1);

      Pointer pNewEnd = CharStringUninitializedCopy(this->InternalLayout().BeginPtr(), p, pNewBegin);
      pNewEnd         = CharStringUninitializedCopy(pBegin, pEnd, pNewEnd);
      pNewEnd         = CharStringUninitializedCopy(p, this->InternalLayout().EndPtr(), pNewEnd);
      *pNewEnd         = 0;

      deallocateSelf();
      this->InternalLayout().SetHeapBeginPtr(pNewBegin);
      this->InternalLayout().SetHeapCapacity(nLength);
      this->InternalLayout().SetHeapSize(nOldSize + n);
    }
  }

  return this->InternalLayout().BeginPtr() + nPos;
}



template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(const ThisType &cSource) {
  return this->append(cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::append(const ThisType &cSource, SizeType nPos, SizeType nNum) {
  return this->append(
    cSource.InternalLayout().BeginPtr() + nPos,
    cSource.InternalLayout().BeginPtr() + nPos + std::min(nNum, cSource.InternalLayout().GetSize() - nPos)
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(const ValueType *p, SizeType n) {
  return this->append(p, p + n);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(const ValueType *p) {
  return this->append(p, p + CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(SizeType nNum, ValueType c) {
  // Current size
  const SizeType n = this->InternalLayout().GetSize();

  const SizeType nCap = this->capacity();

  if ((n + nNum) > nCap) {
    this->reserve(std::max(this->getNewCapacity(nCap), (n + nNum)));
  }

  if (nNum > 0) {
    Pointer pNewEnd = CharStringUninitializedFillN(this->InternalLayout().EndPtr(), nNum, c);
    *pNewEnd = 0;
    this->InternalLayout().SetSize(nNum + n);
  }

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(const ValueType *pBegin, const ValueType *pEnd) {
  // Are begin and end equal?
  if (pBegin != pEnd) {
    const SizeType nOldSize = this->InternalLayout().GetSize();
    const SizeType n = (SizeType)(pEnd - pBegin);

    const SizeType nCap = this->capacity();

    if ((nOldSize + n) > nCap) {
      const SizeType nLength = std::max(this->getNewCapacity(nCap), (nOldSize+n));

      Pointer pNewBegin = this->doAllocate(nLength + 1);

      Pointer pNewEnd = CharStringUninitializedCopy(this->InternalLayout().BeginPtr(), this->InternalLayout().EndPtr(), pNewBegin);
      pNewEnd = CharStringUninitializedCopy(pBegin, pEnd, pNewEnd);
      *pNewEnd = 0;

      this->deallocateSelf();
      this->InternalLayout().SetHeapBeginPtr(pNewBegin);
      this->InternalLayout().SetHeapCapacity(nLength);
      this->InternalLayout().SetHeapSize(nOldSize + n);
    } else {
      Pointer pNewEnd = CharStringUninitializedCopy(pBegin, pEnd, this->InternalLayout().EndPtr());
      *pNewEnd = 0;
      this->InternalLayout().SetSize(nOldSize + n);
    }
  } // Yes -> do nothing

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::appendFormatVAList(const ValueType *pFormat, va_list cArguments) {

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::appendFormat(const ValueType *pFormat, ...) {
  va_list lstArgs;
  va_start(lstArgs, pFormat);
  SizeType len_ = vsnprintf(nullptr, 0, pFormat, lstArgs);
  va_end(lstArgs);

  if (len_ > 0) {
    //char * pNewString = PLNewArray<char>(len_);
    char * pNewString = reinterpret_cast<char*>(this->getAllocator().allocate(sizeof(T)*(len_ + 1)));
    //char * pNewString = new char[len_ + 1];

    va_start(lstArgs, pFormat);
    vsprintf(pNewString, pFormat, lstArgs);
    va_end(lstArgs);

    pNewString[len_] = 0;

    this->append(pNewString);

    //PLDelete(pNewString);
    //delete[] pNewString;
    this->getAllocator().deallocate(pNewString, 1);
  }

  return *this;
}

template<typename T, typename TAllocator>
template<typename TOtherType>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::appendConvert(const TOtherType *p) {
  return this->appendConvert(p, (SizeType)(CharStrlen(p)));;
}

template<typename T, typename TAllocator>
template<typename TOtherType>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::appendConvert(const TOtherType *p, BasicString::SizeType nLen) {
  const SizeType kBufferSize = 512;
  ValueType selfBuffer[kBufferSize];
  ValueType *const selfBufferEnd = selfBuffer + kBufferSize;
  const TOtherType * pOtherEnd = p + nLen;

  while (p != pOtherEnd) {
    ValueType * pSelfBufferCurrent = selfBuffer;

    this->append(selfBuffer, pSelfBufferCurrent);
  }

  return *this;
}

template<typename T, typename TAllocator>
template<typename TOtherString>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::appendConvert(const TOtherString &p) {
  return this->appendConvert(p.data(), p.length());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(uint8 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%d", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(uint16 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%d", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(uint32 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%d", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(uint64 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%ld", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(int16 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%d", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(int32 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%d", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(int64 c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%ld", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(float c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%g", c));
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::append(double c) {
  this->append(BasicString<T, TAllocator>().appendFormat("%g", c));
  return *this;
}


template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::erase(SizeType nPos, SizeType nNum) {
  this->erase(
    this->InternalLayout().BeginPtr() + nPos,
    this->InternalLayout().BeginPtr() + nPos + std::min(nNum, this->InternalLayout().GetSize() - nPos)
  );

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator BasicString<T, TAllocator>::erase(ConstIterator p) {
  RECore::Memory::Move(const_cast<ValueType*>(p), p+1, (sizeT)(this->InternalLayout().EndPtr() - p) * sizeof(ValueType));

  this->InternalLayout().SetSize(this->InternalLayout().GetSize() - 1);

  return const_cast<ValueType*>(p);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator
BasicString<T, TAllocator>::erase(ConstIterator pBegin, ConstIterator pEnd) {
  // Only proceed if begin and end are !=
  if (pBegin != pEnd) {
    RECore::Memory::Move(
      const_cast<ValueType*>(pBegin), pEnd, (sizeT)((this->InternalLayout().EndPtr() - pEnd) + 1) *sizeof(ValueType)
    );
    const SizeType n = (SizeType)(pEnd-pBegin);
    this->InternalLayout().SetSize(this->InternalLayout().GetSize() - n);
  }

  return const_cast<ValueType*>(pBegin);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ReverseIterator BasicString<T, TAllocator>::erase(ReverseIterator p) {
  return BasicString<T, TAllocator>::ReverseIterator(this->erase((++p).base()));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ReverseIterator BasicString<T, TAllocator>::erase(ReverseIterator first, ReverseIterator last) {
  return BasicString<T, TAllocator>::ReverseIterator(this->erase((++last).base(), (++first).base()));
}



template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::replace(SizeType position, SizeType n,
                                                                                   const ThisType &x) {
  return this->replace(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + n,
    x.InternalLayout().BeginPtr(),
    x.InternalLayout().EndPtr()
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::replace(SizeType pos1, SizeType n1,
                                                                                   const ThisType &x, SizeType pos2,
                                                                                   SizeType n2) {
  const SizeType nL1 = std::min(n1, this->InternalLayout().GetSize() - pos1);
  const SizeType nL2 = std::min(n2, this->InternalLayout().GetSize() - pos2);

  return this->replace(
    this->InternalLayout().BeginPtr() + pos1,
    this->InternalLayout().BeginPtr() + pos1 + nL1,
    x.InternalLayout().BeginPtr() + pos2,
    x.InternalLayout().BeginPtr() + pos2 + nL2
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(SizeType position, SizeType n1, const ValueType *p,
                                    SizeType n2) {
  const SizeType nL = std::min(n1, this->InternalLayout().GetSize() - position);

  return this->replace(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + nL,
    p,
    p + n2
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(SizeType position, SizeType n1, const ValueType *p) {
  const SizeType nL = std::min(n1, this->InternalLayout().GetSize() - position);

  return this->replace(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + nL,
    p,
    p + CharStrlen(p)
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &BasicString<T, TAllocator>::replace(SizeType position, SizeType n1,
                                                                                   SizeType n2, ValueType c) {
  const SizeType nL = std::min(n1, this->InternalLayout().GetSize() - position);

  return this->replace(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + nL,
    n2,
    c
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(ConstIterator first, ConstIterator last,
                                    const ThisType &x) {
  return this->replace(first, last, x.InternalLayout().BeginPtr(), x.InternalLayout().EndPtr());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(ConstIterator first, ConstIterator last,
                                    const ValueType *p, SizeType n) {
  return this->replace(first, last, p, p + n);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(ConstIterator first, ConstIterator last,
                                    const ValueType *p) {
  return this->replace(first, last, p, p + CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(ConstIterator first, ConstIterator last,
                                    SizeType n, ValueType c) {
  const SizeType nLength = static_cast<SizeType >(last - first);

  if(nLength >= n)
  {
    CharTypeAssignN(const_cast<ValueType *>(first), n, c);
    this->erase(first + n, last);
  }
  else
  {
    CharTypeAssignN(const_cast<ValueType *>(first), nLength, c);
    this->insert(last, n - nLength, c);
  }

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType &
BasicString<T, TAllocator>::replace(ConstIterator first, ConstIterator last,
                                    const ValueType *pBegin, const ValueType *pEnd) {
  const SizeType nLength1 = SizeType(last - first);
  const SizeType nLength2 = SizeType(pEnd - pBegin);

  if(nLength1 >= nLength2) // If we have a non-expanding operation...
  {
    if((pBegin > last) || (pEnd <= first))  // If we have a non-overlapping operation...
      RECore::Memory::Copy(const_cast<ValueType*>(first), pBegin, (sizeT)(pEnd - pBegin) * sizeof(ValueType));
    else
      RECore::Memory::Move(const_cast<ValueType*>(first), pBegin, (sizeT)(pEnd - pBegin) * sizeof(ValueType));
    this->erase(first + nLength2, last);
  }
  else // Else we are expanding.
  {
    if((pBegin > last) || (pEnd <= first)) // If we have a non-overlapping operation...
    {
      const ValueType* const pMid2 = pBegin + nLength1;

      if((pEnd <= first) || (pBegin > last))
        RECore::Memory::Copy(const_cast<ValueType*>(first), pBegin, (sizeT)(pMid2 - pBegin) * sizeof(ValueType));
      else
        RECore::Memory::Move(const_cast<ValueType*>(first), pBegin, (sizeT)(pMid2 - pBegin) * sizeof(ValueType));
      this->insert(last, pMid2, pEnd);
    }
    else // else we have an overlapping operation.
    {
      // I can't think of any easy way of doing this without allocating temporary memory.
      const SizeType nOldSize     = this->InternalLayout().GetSize();
      const SizeType nOldCap      = this->capacity();
      const SizeType nNewCapacity = std::max(getNewCapacity(nOldCap), (nOldSize + (nLength2 - nLength1)));

      Pointer pNewBegin = doAllocate(nNewCapacity + 1);

      Pointer pNewEnd = CharStringUninitializedCopy(this->InternalLayout().BeginPtr(), first, pNewBegin);
      pNewEnd         = CharStringUninitializedCopy(pBegin, pEnd,   pNewEnd);
      pNewEnd         = CharStringUninitializedCopy(last,   this->InternalLayout().EndPtr(),   pNewEnd);
      *pNewEnd         = 0;

      deallocateSelf();
      this->InternalLayout().SetHeapBeginPtr(pNewBegin);
      this->InternalLayout().SetHeapCapacity(nNewCapacity);
      this->InternalLayout().SetHeapSize(nOldSize + (nLength2 - nLength1));
    }
  }

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType& BasicString<T, TAllocator>::replaceAll(const typename BasicString<T, TAllocator>::ThisType& from, const typename BasicString<T, TAllocator>::ThisType& to) {
  SizeType nStartPos = this->Find(from);

  while (nStartPos != BasicString<T, TAllocator>::NPOS) {
    this->replace(nStartPos, from.length(), to);
    nStartPos += to.length();
    nStartPos = this->Find(from, nStartPos);
  }
  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::copy(ValueType *p, SizeType n, SizeType position) {
  const SizeType nLen = std::min(n, this->InternalLayout().GetSize() - position);

  //CharStringUninitializedCopy(
  //  p, p + nLen,
  //  this->InternalLayout().BeginPtr() + position
  //);

  CharStringUninitializedCopy(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + nLen,
    p
  );

  return nLen;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::copy(const ValueType *p, SizeType n, SizeType position) {
  const SizeType nLen = std::min(n, this->InternalLayout().GetSize() - position);

  //CharStringUninitializedCopy(
  //  p, p + nLen,
  //  this->InternalLayout().BeginPtr() + position
  //);

  CharStringUninitializedCopy(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + nLen,
    const_cast<T*>(p)
  );

  return nLen;
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::contains(const BasicString::ThisType &x) const noexcept {
  return (this->Find(x) != NPOS);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::contains(ValueType c) const noexcept {
  return (this->Find(c) != NPOS);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::contains(const ValueType *p) const {
  return (this->Find(p, 0, CharStrlen(p)) != NPOS);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::Find(const ThisType &x, SizeType position) const noexcept {
  return this->Find(
    x.InternalLayout().BeginPtr(),
    position,
    x.InternalLayout().GetSize()
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::Find(const ValueType *p, SizeType position) const {
  return this->Find(
    p,
    position,
    (SizeType)CharStrlen(p)
  );
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::Find(const ValueType *p, SizeType position, SizeType n) const {

  if (((NPOS - n) >= position) && (position + n) <= this->InternalLayout().GetSize()) {
    const ValueType * const pT = Search(this->InternalLayout().BeginPtr() + position, this->InternalLayout().EndPtr(), p, p + n);

    if ((pT != this->InternalLayout().EndPtr()) || (n == 0)) {
      return SizeType (pT - this->InternalLayout().BeginPtr());
    }
  }

  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::Find(ValueType c, SizeType position) const noexcept {
  if (position < this->InternalLayout().GetSize()) {
    const ConstIterator pIter = std::find(this->InternalLayout().BeginPtr() + position, this->InternalLayout().EndPtr(), c);

    if (pIter != this->InternalLayout().EndPtr()) {
      return SizeType(pIter - this->InternalLayout().BeginPtr());
    }

    if (pIter != this->InternalLayout().EndPtr()) {
      return SizeType (pIter - this->InternalLayout().BeginPtr());
    }
  }

  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::rFind(const ThisType &x, SizeType position) const noexcept {
  return this->rFind(x.InternalLayout().BeginPtr(), position, x.InternalLayout().GetSize());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::rFind(const ValueType *p, SizeType position) const {
  return this->rFind(p, position, SizeType(CharStrlen(p)));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::rFind(const ValueType *p, SizeType position, SizeType n) const {
  const SizeType nLength = this->InternalLayout().GetSize();

  if(n <= nLength) {
    if(n) {
      const ConstIterator pEnd    = this->InternalLayout().BeginPtr() + ::std::min(nLength - n, position) + n;
      const ConstIterator pResult = CharTypeStringRSearch(this->InternalLayout().BeginPtr(), pEnd, p, p + n);

      if(pResult != pEnd)
        return (SizeType)(pResult - this->InternalLayout().BeginPtr());
    }
    else
      return ::std::min(nLength, position);
  }
  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::rFind(ValueType c, SizeType position) const noexcept {
  const SizeType nLength = this->InternalLayout().GetSize();

  if(nLength) {
    const ValueType * const pEnd    = this->InternalLayout().BeginPtr() + ::std::min(nLength - 1, position) + 1;
    const ValueType * const pResult = CharTypeStringrFind(pEnd, this->InternalLayout().BeginPtr(), c);

    if(pResult != this->InternalLayout().BeginPtr())
      return (SizeType)((pResult - 1) - this->InternalLayout().BeginPtr());
  }
  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findFirstOf(const ThisType &x,
                                                                                      SizeType position) const noexcept {
  return this->findFirstOf(x.InternalLayout().BeginPtr(), position, x.InternalLayout().GetSize());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findFirstOf(const ValueType *p, SizeType position) const {
  return this->findFirstOf(p, position, (SizeType)CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findFirstOf(const ValueType *p, SizeType position,
                                                                                      SizeType n) const {
  if (position < this->InternalLayout().GetSize()) {
    const ValueType * pBegin = this->InternalLayout().BeginPtr() + position;
    const ConstIterator pIter = CharTypeStringfindFirstOf(pBegin, this->InternalLayout().EndPtr(), p, p + n);

    if (pIter != this->InternalLayout().EndPtr()) {
      return (SizeType)(pIter - this->InternalLayout().BeginPtr());
    }
  }

  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findFirstOf(ValueType c, SizeType position) const noexcept {
  return this->Find(c, position);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findLastOf(const ThisType &x,
                                                                                     SizeType position) const noexcept {
  return this->findLastOf(x.InternalLayout().BeginPtr(), position, x.InternalLayout().GetSize());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findLastOf(const ValueType *p, SizeType position) const {
  return this->findLastOf(p, position, (SizeType)CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findLastOf(const ValueType *p, SizeType position,
                                                                                     SizeType n) const {
  const SizeType nLength = this->InternalLayout().GetSize();

  if (nLength) {
    const ValueType * pEnd = this->InternalLayout().BeginPtr() + ::std::min(nLength - 1, position) + 1;
    const ValueType* const pResult = CharTypeStringrfindFirstOf(pEnd, this->InternalLayout().BeginPtr(), p, p + n);

    if(pResult != this->InternalLayout().BeginPtr())
      return (SizeType)((pResult - 1) - this->InternalLayout().BeginPtr());
  }

  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findLastOf(ValueType c, SizeType position) const noexcept {
  return this->rFind(c, position);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findFirstNotOf(const ThisType &x,
                                                                                         SizeType position) const noexcept {
  return this->findFirstNotOf(x.InternalLayout().BeginPtr(), position, x.InternalLayout().GetSize());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findFirstNotOf(const ValueType *p, SizeType position) const {
  return this->findFirstNotOf(p, position, (SizeType)CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findFirstNotOf(const ValueType *p, SizeType position,
                                                                                         SizeType n) const {
  if (position <= this->InternalLayout().GetSize()) {
    const ConstIterator pIter = CharTypeStringfindFirstNotOf(
      this->InternalLayout().BeginPtr() + position, this->InternalLayout().EndPtr(), p, p + n);

    if (pIter != this->InternalLayout().EndPtr()) {
      return (SizeType)(pIter - this->InternalLayout().BeginPtr());
    }
  }
  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findFirstNotOf(ValueType c, SizeType position) const noexcept {
  if (position <= this->InternalLayout().GetSize()) {
    const ConstIterator pIter = CharTypeStringfindFirstNotOf(
      this->InternalLayout().BeginPtr() + position, this->InternalLayout().EndPtr(), &c, &c + 1);

    if (pIter != this->InternalLayout().EndPtr()) {
      return (SizeType)(pIter - this->InternalLayout().BeginPtr());
    }
  }
  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findLastNotOf(const ThisType &x,
                                                                                        SizeType position) const noexcept {
  return this->findLastNotOf(x.InternalLayout().BeginPtr(), position, x.InternalLayout().GetSize());
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findLastNotOf(const ValueType *p, SizeType position) const {
  return this->findLastNotOf(p, position, (SizeType)CharStrlen(p));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::findLastNotOf(const ValueType *p, SizeType position,
                                                                                        SizeType n) const {
  const SizeType nLength = this->InternalLayout().GetSize();

  if (nLength) {
    const ValueType * pEnd = this->InternalLayout().BeginPtr() + std::min(nLength - 1, position) + 1;
    const ValueType * pIter = CharTypeStringrfindFirstNotOf(pEnd, this->InternalLayout().BeginPtr(), p, p + n);

    if (pIter != this->InternalLayout().BeginPtr()) {
      return (SizeType)((pIter - 1) - this->InternalLayout().BeginPtr());
    }
  }

  return NPOS;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType
BasicString<T, TAllocator>::findLastNotOf(ValueType c, SizeType position) const noexcept {
  const SizeType nLength = this->InternalLayout().GetSize();

  if (nLength) {
    const ValueType * pEnd = this->InternalLayout().BeginPtr() + std::min(nLength - 1, position) + 1;
    const ValueType * pIter = CharTypeStringrfindFirstNotOf(pEnd, this->InternalLayout().BeginPtr(), &c, &c + 1);

    if (pIter != this->InternalLayout().BeginPtr()) {
      return (SizeType)((pIter - 1) - this->InternalLayout().BeginPtr());
    }
  }

  return NPOS;
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::startsWith(ValueType c) const {
  return (this->substr(0, 1) == c);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::startsWith(const ValueType *p) const {
  sizeT nLen = CharStrlen(p);
  return (this->substr(0, nLen) == p);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::startsWith(const BasicString::ThisType &c) const {
  return (this->substr(0, c.length()) == c);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::endsWith(ValueType c) const {
  return (this->substr(this->Length()-1, 1) == c);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::endsWith(const ValueType *p) const {
  sizeT nLen = CharStrlen(p);
  return (this->substr(this->Length()-nLen, nLen) == p);
}

template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::endsWith(const BasicString::ThisType &c) const {
  return (this->substr(this->Length()-c.length()) == c);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType
BasicString<T, TAllocator>::substr(SizeType position, SizeType n) const {
  return BasicString(
    this->InternalLayout().BeginPtr() + position,
    this->InternalLayout().BeginPtr() + position + std::min(n, this->InternalLayout().GetSize() - position), AllocatorType()
  );
}


template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::resize(SizeType n, ValueType c) {
  const SizeType nCurrSize = this->InternalLayout().GetSize();

  if (n < nCurrSize) {
    this->erase(this->InternalLayout().BeginPtr() + n, this->InternalLayout().EndPtr());
  } else {
    this->append(n - nCurrSize, c);
  }
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::resize(SizeType n) {
  const SizeType nCurrSize = this->InternalLayout().GetSize();

  if (n < nCurrSize) {
    this->erase(this->InternalLayout().BeginPtr() + n, this->InternalLayout().EndPtr());
  } else if (n > nCurrSize) {
    this->append(n - nCurrSize, ValueType());
  }
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::reserve(SizeType n) {
  n = std::max(n, this->InternalLayout().GetSize());

  if (n > this->capacity()) {
    this->setCapacity(n);
  }
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::setCapacity(SizeType n) {
  if (n == NPOS) {
    n = this->InternalLayout().GetSize();
  }

  else if ( n < this->InternalLayout().GetSize()) {
    this->InternalLayout().SetSize(n);
    *this->InternalLayout().EndPtr() = 0;
  }

  if ((n < this->capacity() && this->InternalLayout().IsHeap()) || (n > this->capacity())) {
    if(n <= SSOLayout::SSO_CAPACITY)
    {
      // heap->sso
      // A heap based layout wants to reduce its size to within sso capacity
      // An sso layout wanting to reduce its capacity will not get in here
      Pointer pOldBegin = this->InternalLayout().BeginPtr();
      const SizeType  nOldCap = this->InternalLayout().GetHeapCapacity();

      this->InternalLayout().ResetToSSO(); // reset layout to sso
      CharStringUninitializedCopy(pOldBegin, pOldBegin + n, this->InternalLayout().BeginPtr());
      // *EndPtr() = 0 is already done by the ResetToSSO
      this->InternalLayout().SetSSOSize(n);

      this->doFree(pOldBegin, nOldCap + 1);

      return;
    }

    Pointer pNewBegin = this->doAllocate(n + 1); // We need the + 1 to accomodate the trailing 0.
    SizeType nSavedSize = this->InternalLayout().GetSize(); // save the size in case we transition from sso->heap

    Pointer pNewEnd = CharStringUninitializedCopy(this->InternalLayout().BeginPtr(), this->InternalLayout().EndPtr(), pNewBegin);
    *pNewEnd = 0;

    this->deallocateSelf();

    this->InternalLayout().SetHeapBeginPtr(pNewBegin);
    this->InternalLayout().SetHeapCapacity(n);
    this->InternalLayout().SetHeapSize(nSavedSize);
  }
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator BasicString<T, TAllocator>::begin() noexcept {
  return this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstIterator BasicString<T, TAllocator>::begin() const noexcept {
  return this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator BasicString<T, TAllocator>::end() noexcept {
  return this->InternalLayout().EndPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstIterator BasicString<T, TAllocator>::end() const noexcept {
  return this->InternalLayout().EndPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Reference& BasicString<T, TAllocator>::front() noexcept {
  return *this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReference& BasicString<T, TAllocator>::front() const noexcept {
  return *this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Reference& BasicString<T, TAllocator>::back() noexcept {
  return *(this->InternalLayout().EndPtr() - 1);
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ConstReference& BasicString<T, TAllocator>::back() const noexcept {
  return *(this->InternalLayout().EndPtr() - 1);
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::pushBack(ValueType c) {
  this->append((SizeType)1, c);
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::popBack() {
  this->InternalLayout().EndPtr()[-1] = ValueType(0);
  this->InternalLayout().SetSize(this->InternalLayout().GetSize() - 1);
}


template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::empty() const noexcept {
  return this->InternalLayout().GetSize() == 0;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::size() const noexcept {
  return this->InternalLayout().GetSize();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::length() const noexcept {
  return this->InternalLayout().GetSize();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::maxSize() const noexcept {
  return kMaxSize;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::capacity() const noexcept {
  if (this->InternalLayout().IsHeap()) {
    return this->InternalLayout().GetHeapCapacity();
  }
  return SSOLayout::SSO_CAPACITY;
}


template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::data() const noexcept {
  return this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::cstr() const noexcept {
  return this->InternalLayout().BeginPtr();
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::makeLower() {
  for (Pointer p = this->InternalLayout().BeginPtr(); p < this->InternalLayout().EndPtr(); ++p) {
    *p = (ValueType)CharToLower(*p);
  }
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::makeUpper() {
  for (Pointer p = this->InternalLayout().BeginPtr(); p < this->InternalLayout().EndPtr(); ++p) {
    *p = (ValueType)CharToUpper(*p);
  }
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType BasicString<T, TAllocator>::ToLower() const {
  BasicString<T, TAllocator>::ThisType result(*this);
  for (Pointer p = result.InternalLayout().BeginPtr(); p < result.InternalLayout().EndPtr(); ++p) {
    *p = (ValueType)CharToLower(*p);
  }

  return result;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType BasicString<T, TAllocator>::ToUpper() const {
  BasicString<T, TAllocator>::ThisType result(*this);
  for (Pointer p = result.InternalLayout().BeginPtr(); p < result.InternalLayout().EndPtr(); ++p) {
    *p = (ValueType)CharToUpper(*p);
  }

  return result;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType BasicString<T, TAllocator>::ltrim() {
  const ValueType values[] = { ' ', '\t', 0 };
  this->erase(0, this->findFirstNotOf(values));

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType BasicString<T, TAllocator>::rtrim() {
  const ValueType values[] = { ' ', '\t', 0 };
  this->erase(this->findLastNotOf(values) + 1);

  return *this;
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ThisType BasicString<T, TAllocator>::trim() {
  this->ltrim();
  return this->rtrim();
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::shrinkToFit() {
  this->setCapacity(this->InternalLayout().GetSize());
}


template<typename T, typename TAllocator>
bool BasicString<T, TAllocator>::validate() const noexcept {
  if((this->InternalLayout().BeginPtr() == nullptr) || (this->InternalLayout().EndPtr() == nullptr))
    return false;
  if(this->InternalLayout().EndPtr() < this->InternalLayout().BeginPtr())
    return false;
  if(this->InternalLayout().CapacityPtr() < this->InternalLayout().EndPtr())
    return false;
  if(*this->InternalLayout().EndPtr() != 0)
    return false;
  return true;
}



template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::compare(const BasicString::ThisType &cSource) const {
  return compare(
    this->InternalLayout().BeginPtr(),
    this->InternalLayout().EndPtr(),
    cSource.InternalLayout().BeginPtr(),
    cSource.InternalLayout().EndPtr()
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::compare(BasicString::SizeType nPos1, BasicString::SizeType n1,
                                        const BasicString::ThisType &cSource) const {
  return compare(
    this->InternalLayout().BeginPtr() + nPos1,
    this->InternalLayout().EndPtr() + nPos1 + std::min(n1, this->InternalLayout().GetSize() - nPos1),
    cSource.InternalLayout().BeginPtr(),
    cSource.InternalLayout().EndPtr()
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::compare(BasicString::SizeType nPos1, BasicString::SizeType n1,
                                        const BasicString::ThisType &cSource, BasicString::SizeType nPos2,
                                        BasicString::SizeType n2) const {
  return compare(
    this->InternalLayout().BeginPtr() + nPos1,
    this->InternalLayout().BeginPtr() + nPos1 + std::min(n1, this->InternalLayout().GetSize() - nPos1),
    cSource.InternalLayout().BeginPtr() + nPos2,
    cSource.InternalLayout().BeginPtr() + nPos2 + std::min(n2, cSource.InternalLayout().GetSize() - nPos2)
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::compare(const ValueType *p) const {
  return compare(
    this->InternalLayout().BeginPtr(),
    this->InternalLayout().EndPtr(),
    p,
    p + CharStrlen(p)
  );
}

template<typename T, typename TAllocator>
int
BasicString<T, TAllocator>::compare(BasicString::SizeType nPos1, BasicString::SizeType n1, const ValueType *p) const {
  return compare(
    this->InternalLayout().BeginPtr() + nPos1 + std::min(n1, this->InternalLayout().GetSize() - nPos1),
    this->InternalLayout().EndPtr(),
    p,
    p + CharStrlen(p)
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::compare(BasicString::SizeType nPos1, BasicString::SizeType n1, const ValueType *p,
                                        BasicString::SizeType n2) const {
  return compare(
    this->InternalLayout().BeginPtr() + nPos1,
    this->InternalLayout().BeginPtr() + nPos1 + std::min(n1, this->InternalLayout().GetSize() - nPos1),
    p,
    p + n2
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::compare(const ValueType *pB1, const ValueType *pE1, const ValueType *pB2,
                                        const ValueType *pE2) {
  const DifferenceType d1 = pE1 - pB1;
  const DifferenceType d2 = pE2 - pB2;
  const DifferenceType dMin = std::min(d1, d2);
  const int nCmp = CharStringcompare(pB1, pB2, (sizeT)dMin);

  return (nCmp != 0 ? nCmp : (d1 < d2) ? -1 : (d1 > d2) ? 1 : 0);
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::comparei(const BasicString::ThisType &cSource) const {
  return comparei(
    this->InternalLayout().BeginPtr(), this->InternalLayout().EndPtr(),
    cSource.InternalLayout().BeginPtr(), cSource.InternalLayout().EndPtr()
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::comparei(BasicString::SizeType nPos1, BasicString::SizeType n1,
                                         const BasicString::ThisType &cSource) const {
  return comparei(
    this->InternalLayout().BeginPtr() + nPos1,
    this->InternalLayout().EndPtr() + nPos1 + std::min(n1, this->InternalLayout().GetSize() - nPos1),
    cSource.InternalLayout().BeginPtr(),
    cSource.InternalLayout().EndPtr()
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::comparei(BasicString::SizeType nPos1, BasicString::SizeType n1,
                                         const BasicString::ThisType &cSource, BasicString::SizeType nPos2,
                                         BasicString::SizeType n2) const {
  return comparei(
    this->InternalLayout().BeginPtr() + nPos1,
    this->InternalLayout().BeginPtr() + nPos1 + std::min(n1, this->InternalLayout().GetSize() - nPos1),
    cSource.InternalLayout().BeginPtr() + nPos2,
    cSource.InternalLayout().BeginPtr() + nPos2 + std::min(n2, cSource.InternalLayout().GetSize() - nPos2)
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::comparei(const ValueType *p) const {
  return comparei(
    this->InternalLayout().BeginPtr(), this->InternalLayout().EndPtr(),
    p, p + CharStrlen(p)
  );
}

template<typename T, typename TAllocator>
int BasicString<T, TAllocator>::comparei(const ValueType *pB1, const ValueType *pE1, const ValueType *pB2,
                                         const ValueType *pE2) {
  const DifferenceType d1 = pE1 - pB1;
  const DifferenceType d2 = pE2 - pB2;
  const DifferenceType dMin = std::min(d1, d2);
  const int nCmp = CharStringCompareI(pB1, pB2, (sizeT)dMin);

  return (nCmp != 0 ? nCmp : (d1 < d2) ? -1 : (d1 > d2) ? 1 : 0);
}



template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::rangeInitialize(const ValueType *pBegin, const ValueType *pEnd) {
  // Get the size of the range
  const SizeType nSize = (SizeType)(pEnd - pBegin);

  // Allocate enough space
  this->allocateSelf(nSize);

  // Now copy everything over
  CharStringUninitializedCopy(pBegin, pEnd, this->InternalLayout().BeginPtr());

  this->InternalLayout().SetSize(nSize);

  *this->InternalLayout().EndPtr() = 0;
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::rangeInitialize(const ValueType *pBegin) {
  this->rangeInitialize(pBegin, pBegin + CharStrlen(pBegin));
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::sizeInitialize(SizeType n, ValueType c) {
  // First allocate enough space
  this->allocateSelf(n);

  // Call the filling
  CharStringUninitializedFillN(this->InternalLayout().BeginPtr(), n, c);
  this->InternalLayout().SetSize(n);
  *this->InternalLayout().EndPtr() = 0;
}


template<typename T, typename TAllocator>
inline typename BasicString<T, TAllocator>::Layout & BasicString<T, TAllocator>::InternalLayout() noexcept {
  return this->mPair.First();
}

template<typename T, typename TAllocator>
inline const typename BasicString<T, TAllocator>::Layout & BasicString<T, TAllocator>::InternalLayout() const noexcept {
  return this->mPair.First();
}

template<typename T, typename TAllocator>
const typename BasicString<T, TAllocator>::AllocatorType &BasicString<T, TAllocator>::getAllocator() const noexcept {
  return this->mPair.Second();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::AllocatorType &BasicString<T, TAllocator>::getAllocator() noexcept {
  return this->mPair.Second();
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::ValueType *BasicString<T, TAllocator>::doAllocate(BasicString::SizeType n) {
  auto pAlloc = this->getAllocator();
  return (BasicString<T, TAllocator>::ValueType*)(pAlloc.allocate(n*sizeof(ValueType)));
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::Iterator BasicString<T, TAllocator>::_insert(ConstIterator p, ValueType c) {
  Iterator pNewPosition = const_cast<ValueType *>(p);

  if((this->InternalLayout().EndPtr() + 1) <= this->InternalLayout().CapacityPtr())
  {
    const SizeType nSavedSize = this->InternalLayout().GetSize();
    RECore::Memory::Move(const_cast<ValueType *>(p) + 1, p, (sizeT)(this->InternalLayout().EndPtr() - p) * sizeof(ValueType));
    *(this->InternalLayout().EndPtr() + 1) = 0;
    *pNewPosition = c;
    this->InternalLayout().SetSize(nSavedSize + 1);
  }
  else
  {
    const SizeType nOldSize = this->InternalLayout().GetSize();
    const SizeType nOldCap  = this->capacity();
    const SizeType nLength  = std::max(this->getNewCapacity(nOldCap), (nOldSize + 1));

    Iterator pNewBegin = this->doAllocate(nLength + 1);

    pNewPosition = CharStringUninitializedCopy(this->InternalLayout().BeginPtr(), p, pNewBegin);
    *pNewPosition = c;

    Iterator pNewEnd = pNewPosition + 1;
    pNewEnd          = CharStringUninitializedCopy(p, this->InternalLayout().EndPtr(), pNewEnd);
    *pNewEnd          = 0;

    this->deallocateSelf();
    this->InternalLayout().SetHeapBeginPtr(pNewBegin);
    this->InternalLayout().SetHeapCapacity(nLength);
    this->InternalLayout().SetHeapSize(nOldSize + 1);
  }
  return pNewPosition;
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::allocateSelf() {
  this->InternalLayout().ResetToSSO();
  this->InternalLayout().SetSSOSize(0);
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::allocateSelf(SizeType nSize) {
  if (nSize > SSOLayout::SSO_CAPACITY) {
    Pointer pBegin = this->doAllocate(nSize + 1); // + 1 because of the \0
    this->InternalLayout().SetHeapBeginPtr(pBegin);
    this->InternalLayout().SetHeapCapacity(nSize);
    this->InternalLayout().SetHeapSize(0);
  } else {
    this->allocateSelf();
  }
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::deallocateSelf() {
  if (this->InternalLayout().IsHeap()) {
    this->doFree(this->InternalLayout().BeginPtr(), this->InternalLayout().GetHeapCapacity());// + 1);
  }
}

template<typename T, typename TAllocator>
void BasicString<T, TAllocator>::doFree(ValueType *p, SizeType n) {
  if (p) {
    auto pAlloc = this->getAllocator();
    pAlloc.deallocate(p, n*sizeof(ValueType));
    //delete[] p;
  }
}

template<typename T, typename TAllocator>
typename BasicString<T, TAllocator>::SizeType BasicString<T, TAllocator>::getNewCapacity(SizeType nCurCap) {
  return (nCurCap <= SSOLayout::SSO_CAPACITY) ? SSOLayout::SSO_CAPACITY : (2 * nCurCap);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore