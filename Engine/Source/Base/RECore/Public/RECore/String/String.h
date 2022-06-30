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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Core/Config.h"
#include "RECore/Memory/Memory.h"
#include "RECore/Memory/StdAllocator.h"
#include "RECore/Utility/CompressedPair.h"
#include "RECore/String/StringUtils.h"
#include "RECore/Utility/Iterator.h"
#include "RECore/Utility/Functional.h"
#include <memory>
#include <cstdarg>
#include <iterator>
#include <functional>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename T, typename TAllocator = DEFAULT_ALLOCATOR>
class BasicString {
public:
  typedef BasicString<T, TAllocator> ThisType;
  typedef T ValueType;
  typedef RECore::sizeT SizeType;
  typedef T *Pointer;
  typedef const T *ConstPointer;

  typedef T *Iterator;
  typedef const T *ConstIterator;
  typedef T &Reference;
  typedef const T &ConstReference;
  typedef RECore::ReverseIterator<Iterator> ReverseIterator;
  typedef RECore::ReverseIterator<ConstIterator> ConstReverseIterator;
  typedef ptrdiff_t DifferenceType;
  typedef TAllocator AllocatorType;

  /**
   * NPOS, as defined by the standard is a non-valid position.
   */
  static constexpr SizeType NPOS = (SizeType) -1;

  /**
   * @brief
   * CtorSprintf exists so that we can create a constructor that accepts printf-style
	 * arguments but also doesn't collide with any other constructor declaration.
   */
  struct CtorSprintf{};

#ifdef RE_SYSTEM_BIG_ENDIAN
  // Big Endian use LSB, unless we want to reorder struct layouts on endianness, Bit is set when we are in Heap
  static constexpr SizeType kHeapMask = 0x1;
  static constexpr SizeType kSSOMask  = 0x1;
#else
  // Little Endian use MSB
  static constexpr SizeType kHeapMask = ~(SizeType(~SizeType(0)) >> 1);
  static constexpr SizeType kSSOMask = 0x80;
#endif

#ifdef RE_SYSTEM_BIG_ENDIAN
  static constexpr SizeType kMaxSize = (~kHeapMask) >> 1;
#else
  static constexpr SizeType kMaxSize = ~kHeapMask;
#endif

protected:

  struct HeapLayout {
    ValueType *mpBegin;
    SizeType mnSize;
    SizeType mnCapacity;
  };

  template<typename TChar, size_t = sizeof(TChar)>
  struct SSOPadding {
    char padding[sizeof(TChar) - sizeof(char)];
  };

  template<typename TChar>
  struct SSOPadding<TChar, 1> {

  };

  struct SSOLayout {
    static constexpr SizeType SSO_CAPACITY = (sizeof(HeapLayout) - sizeof(char)) / sizeof(ValueType);

    struct SSOSize : SSOPadding<ValueType> {
      char mnRemainingSize;
    };

    ValueType mData[SSO_CAPACITY]; // Local buffer for string data.
    SSOSize mRemainingSizeField;
  };

  struct RawLayout {
    char mBuffer[sizeof(HeapLayout)];
  };

  struct Layout {
    union {
      HeapLayout heap;
      SSOLayout sso;
      RawLayout raw;
    };

    Layout();

    Layout(const Layout &cSource);

    Layout(Layout &&cSource);

    Layout &operator=(const Layout &cSource);

    Layout &operator=(Layout &&cSource);

    inline SizeType GetSize() const noexcept;

    inline SizeType GetSSOSize() const noexcept;

    inline SizeType GetHeapSize() const noexcept;

    inline void SetSize(SizeType nSize);

    inline void SetSSOSize(SizeType nSize);

    inline void SetHeapSize(SizeType nSize);


    inline SizeType GetRemainingCapacity() const noexcept;

    inline ValueType *HeapBeginPtr() noexcept;

    inline const ValueType *HeapBeginPtr() const noexcept;

    inline ValueType *SSOBeginPtr() noexcept;

    inline const ValueType *SSOBeginPtr() const noexcept;

    inline ValueType *BeginPtr() noexcept;

    inline const ValueType *BeginPtr() const noexcept;

    inline ValueType *HeapEndPtr() noexcept;

    inline const ValueType *HeapEndPtr() const noexcept;

    inline ValueType *SSOEndPtr() noexcept;

    inline const ValueType *SSOEndPtr() const noexcept;

    // Points to end of character stream, *ptr == '0'
    inline ValueType *EndPtr() noexcept;

    inline const ValueType *EndPtr() const noexcept;

    inline ValueType *HeapCapacityPtr() noexcept;

    inline const ValueType *HeapCapacityPtr() const noexcept;

    inline ValueType *SSOCapacityPtr() noexcept;

    inline const ValueType *SSOCapacityPtr() const noexcept;

    // Points to end of the buffer at the terminating '0', *ptr == '0' <- not true for SSO
    inline ValueType *CapacityPtr() noexcept;

    inline const ValueType *CapacityPtr() const noexcept;

    inline void SetHeapBeginPtr(ValueType *pBegin) noexcept;

    inline void SetHeapCapacity(SizeType cap) noexcept;

    inline SizeType GetHeapCapacity() const noexcept;

    inline void Copy(Layout &dst, const Layout &src) noexcept;

    inline void Move(Layout &dst, Layout &src) noexcept;

    inline void Swap(Layout &a, Layout &b) noexcept;


    inline void ResetToSSO() noexcept;


    inline bool IsHeap() const noexcept;

    inline bool IsSSO() const noexcept;
  };

  inline Layout &InternalLayout() noexcept;

  inline const Layout &InternalLayout() const noexcept;

public:

  /**
   * @brief
   * Default constructor.
   */
  BasicString() noexcept;

  BasicString(const ThisType &cSource, SizeType nPos, SizeType nNum = NPOS);

  BasicString(ValueType c, const AllocatorType &alloc = DEFAULT_ALLOCATOR());

  BasicString(const ValueType *p, const AllocatorType &alloc = DEFAULT_ALLOCATOR());

  BasicString(ValueType *p, const AllocatorType &alloc = DEFAULT_ALLOCATOR());

  BasicString(const ValueType *p, SizeType nSize, const AllocatorType &alloc = DEFAULT_ALLOCATOR());

  BasicString(SizeType n, ValueType c, const AllocatorType &alloc = DEFAULT_ALLOCATOR());

  BasicString(const ValueType *pBegin, const ValueType *pEnd, const AllocatorType &alloc = DEFAULT_ALLOCATOR());

  /**
   * @brief
   * Constructor.
   *
   * @param [in]cSource
   * The object to copy from.
   */
  BasicString(const ThisType &cSource);

  /**
   * @brief
   * Constructor.
   *
   * @param [in]cSource
   * The object to copy from.
   */
  BasicString(const ThisType &cSource, const AllocatorType &alloc);

  /**
   * @brief
   * Constructor.
   *
   * @param [in]cSource
   * The object to move.
   */
  BasicString(ThisType &&cSource);

  /**
   * @brief
   * Constructor.
   *
   * @param [in]cSource
   * The object to move.
   */
  BasicString(ThisType &&cSource, const AllocatorType &alloc);

  BasicString(CtorSprintf, const ValueType * pFormat, ...);

  /**
   * @brief
   * Destructor.
   */
  ~BasicString();


  ThisType &operator=(const ThisType &cSource);

  ThisType &operator=(ThisType &&cSource);

  ThisType &operator=(const ValueType *p);

  ThisType &operator=(ValueType c);

  ThisType &operator=(ValueType *p);

  template<typename TOtherType>
  ThisType &operator=(const TOtherType *p);

  template<typename TOtherType>
  ThisType &operator=(const TOtherType &cSource);

  ThisType &operator=(RECore::uint8 c);

  ThisType &operator=(uint16 c);

  ThisType &operator=(uint32 c);

  ThisType &operator=(uint64 c);

  ThisType &operator=(int16 c);

  ThisType &operator=(int32 c);

  ThisType &operator=(int64 c);

  ThisType &operator=(float c);

  ThisType &operator=(double c);

  Reference operator[](SizeType n);

  ConstReference operator[](SizeType n) const;

  ThisType &operator+=(const ThisType &cSource);

  ThisType &operator+=(const ValueType *p);

  ThisType &operator+=(ValueType c);

  ThisType &operator+=(RECore::uint8 c);

  ThisType &operator+=(uint16 c);

  ThisType &operator+=(uint32 c);

  ThisType &operator+=(uint64 c);

  ThisType &operator+=(int16 c);

  ThisType &operator+=(int32 c);

  ThisType &operator+=(int64 c);

  ThisType &operator+=(float c);

  ThisType &operator+=(double c);

  operator ValueType *() const { return this->cstr(); }

  operator const ValueType *() const { return this->cstr(); }


  void swap(ThisType &cSource);


  bool getBool() const;

  sizeT getInt() const;

  float getFloat() const;

  double getDouble() const;

  uint_ptr getUIntPtr() const;

  char getChar() const;

  //
  // Assign
  //
  ThisType &assign(const ThisType &cSource);

  ThisType &assign(const ThisType &cSource, SizeType nPos, SizeType nNum = NPOS);

  ThisType &assign(const ValueType *p, SizeType nNum);

  ThisType &assign(SizeType nNum, ValueType c);

  ThisType &assign(const ValueType *pB, const ValueType *pE);

  ThisType &assign(ThisType &&cSource);

  template<typename TOther>
  ThisType &assignConvert(const TOther *p);

  template<typename TOther>
  ThisType &assignConvert(const TOther *p, SizeType nSize);

  template<typename TOther>
  ThisType &assignConvert(const TOther &cSource);


  //
  // Iterators
  //
  Iterator begin() noexcept;

  ConstIterator begin() const noexcept;

  ConstIterator cbegin() const noexcept;

  Iterator end() noexcept;

  ConstIterator end() const noexcept;

  ConstIterator cend() const noexcept;

  ReverseIterator rbegin() noexcept;

  ConstReverseIterator rbegin() const noexcept;

  ConstReverseIterator crbegin() const noexcept;

  ReverseIterator rend() noexcept;

  ConstReverseIterator rend() const noexcept;

  ConstReverseIterator crend() const noexcept;

  Reference& front() noexcept;

  ConstReference& front() const noexcept;

  Reference& back() noexcept;

  ConstReference& back() const noexcept;


  //
  // Insert
  //
  ThisType &insert(SizeType nPos, const ThisType &cSource);

  ThisType &insert(SizeType nPos, const ThisType &cSource, SizeType nBegin, SizeType nNum);

  ThisType &insert(SizeType nPos, const ValueType *p, SizeType nNum);

  ThisType &insert(SizeType nPos, const ValueType *p);

  ThisType &insert(SizeType nPos, SizeType n, ValueType c);

  Iterator insert(ConstIterator p, ValueType c);

  Iterator insert(ConstIterator p, SizeType nNum, ValueType c);

  Iterator insert(ConstIterator p, const ValueType *pBegin, const ValueType *pEnd);


  //
  // Append
  //

  ThisType &append(const ThisType &cSource);

  ThisType &append(const ThisType &cSource, SizeType nPos, SizeType nNum = NPOS);

  ThisType &append(const ValueType *p, SizeType n);

  ThisType &append(const ValueType *p);

  ThisType &append(SizeType nNum, ValueType c);

  ThisType &append(const ValueType *pBegin, const ValueType *pEnd);

  ThisType &appendFormatVAList(const ValueType *pFormat, va_list cArguments);

  ThisType &appendFormat(const ValueType *pFormat, ...);

  template<typename TOtherType>
  ThisType &appendConvert(const TOtherType *p);

  template<typename TOtherType>
  ThisType &appendConvert(const TOtherType *p, SizeType nLen);


  template<typename TOtherString>
  ThisType &appendConvert(const TOtherString &p);

  ThisType &append(RECore::uint8 c);

  ThisType &append(uint16 c);

  ThisType &append(uint32 c);

  ThisType &append(uint64 c);

  ThisType &append(int16 c);

  ThisType &append(int32 c);

  ThisType &append(int64 c);

  ThisType &append(float c);

  ThisType &append(double c);

  void pushBack(ValueType c);

  void popBack();


  //
  // Erase
  //

  ThisType &erase(SizeType nPos = 0, SizeType nNum = NPOS);

  Iterator erase(ConstIterator p);

  Iterator erase(ConstIterator pBegin, ConstIterator pEnd);

  ReverseIterator erase(ReverseIterator p);

  ReverseIterator erase(ReverseIterator first, ReverseIterator last);

  void clear() noexcept;


  //
  // Replace
  //
  ThisType &replace(SizeType position, SizeType n, const ThisType &x);

  ThisType &replace(SizeType pos1, SizeType n1, const ThisType &x, SizeType pos2, SizeType n2 = NPOS);

  ThisType &replace(SizeType position, SizeType n1, const ValueType *p, SizeType n2);

  ThisType &replace(SizeType position, SizeType n1, const ValueType *p);

  ThisType &replace(SizeType position, SizeType n1, SizeType n2, ValueType c);

  ThisType &replace(ConstIterator first, ConstIterator last, const ThisType &x);

  ThisType &replace(ConstIterator first, ConstIterator last, const ValueType *p, SizeType n);

  ThisType &replace(ConstIterator first, ConstIterator last, const ValueType *p);

  ThisType &replace(ConstIterator first, ConstIterator last, SizeType n, ValueType c);

  ThisType &replace(ConstIterator first, ConstIterator last, const ValueType *pBegin, const ValueType *pEnd);

  ThisType &replaceAll(const ThisType &from, const ThisType &to);

  SizeType copy(ValueType *p, SizeType n, SizeType position = 0);

  SizeType copy(const ValueType *p, SizeType n, SizeType position = 0);


  //
  // Contains: C++23 feature
  //
  bool contains(const ThisType &x) const noexcept;

  bool contains(ValueType c) const noexcept;

  bool contains(const ValueType *p) const;


  //
  // Find
  //
  SizeType Find(const ThisType &x, SizeType position = 0) const noexcept;

  SizeType Find(const ValueType *p, SizeType position = 0) const;

  SizeType Find(const ValueType *p, SizeType position, SizeType n) const;

  SizeType Find(ValueType c, SizeType position = 0) const noexcept;


  //
  // Reverse Find
  //
  SizeType rFind(const ThisType &x, SizeType position = NPOS) const noexcept;

  SizeType rFind(const ValueType *p, SizeType position = NPOS) const;

  SizeType rFind(const ValueType *p, SizeType position, SizeType n) const;

  SizeType rFind(ValueType c, SizeType position = NPOS) const noexcept;


  //
  // First First Of
  //
  SizeType findFirstOf(const ThisType &x, SizeType position = 0) const noexcept;

  SizeType findFirstOf(const ValueType *p, SizeType position = 0) const;

  SizeType findFirstOf(const ValueType *p, SizeType position, SizeType n) const;

  SizeType findFirstOf(ValueType c, SizeType position = 0) const noexcept;


  //
  // Find Last Of
  //
  SizeType findLastOf(const ThisType &x, SizeType position = NPOS) const noexcept;

  SizeType findLastOf(const ValueType *p, SizeType position = NPOS) const;

  SizeType findLastOf(const ValueType *p, SizeType position, SizeType n) const;

  SizeType findLastOf(ValueType c, SizeType position = NPOS) const noexcept;


  //
  // Find First Not Of
  //
  SizeType findFirstNotOf(const ThisType &x, SizeType position = 0) const noexcept;

  SizeType findFirstNotOf(const ValueType *p, SizeType position = 0) const;

  SizeType findFirstNotOf(const ValueType *p, SizeType position, SizeType n) const;

  SizeType findFirstNotOf(ValueType c, SizeType position = 0) const noexcept;


  //
  // Find Last Not Of
  //
  SizeType findLastNotOf(const ThisType &x, SizeType position = NPOS) const noexcept;

  SizeType findLastNotOf(const ValueType *p, SizeType position = NPOS) const;

  SizeType findLastNotOf(const ValueType *p, SizeType position, SizeType n) const;

  SizeType findLastNotOf(ValueType c, SizeType position = NPOS) const noexcept;


  //
  // startsWith
  //
  bool startsWith(ValueType c) const;

  bool startsWith(const ValueType *p) const;

  bool startsWith(const ThisType &c) const;


  //
  // endsWith
  //
  bool endsWith(ValueType c) const;

  bool endsWith(const ValueType *p) const;

  bool endsWith(const ThisType &c) const;


  //
  // Substr
  //
  ThisType substr(SizeType position = 0, SizeType n = NPOS) const;

  //
  // Compare
  //
  int compare(const ThisType &cSource) const;

  int compare(SizeType nPos1, SizeType n1, const ThisType &cSource) const;

  int compare(SizeType nPos1, SizeType n1, const ThisType &cSource, SizeType nPos2, SizeType n2) const;

  int compare(const ValueType *p) const;

  int compare(SizeType nPos1, SizeType n1, const ValueType *p) const;

  int compare(SizeType nPos1, SizeType n1, const ValueType *p, SizeType n2) const;

  static int compare(const ValueType *pB1, const ValueType *pE1, const ValueType *pB2, const ValueType *pE2);


  //
  // Comparei
  //
  int comparei(const ThisType &cSource) const;

  int comparei(SizeType nPos1, SizeType n1, const ThisType &cSource) const;

  int comparei(SizeType nPos1, SizeType n1, const ThisType &cSource, SizeType nPos2, SizeType n2) const;

  int comparei(const ValueType *p) const;

  static int comparei(const ValueType *pB1, const ValueType *pE1, const ValueType *pB2, const ValueType *pE2);


  //
  // Size Related
  //
  bool empty() const noexcept;

  SizeType size() const noexcept;

  SizeType length() const noexcept;

  SizeType maxSize() const noexcept;

  SizeType capacity() const noexcept;

  void resize(SizeType n, ValueType c);

  void resize(SizeType n);

  void reserve(SizeType n = 0);

  void setCapacity(SizeType n = NPOS);

  void forceSize(SizeType n);

  void shrinkToFit();


  //
  // Raw data access
  //
  const ValueType *data() const noexcept;

  const ValueType *cstr() const noexcept;

  void makeLower();

  void makeUpper();

  ThisType ToLower() const;

  ThisType ToUpper() const;

  ThisType ltrim();

  ThisType rtrim();

  ThisType trim();


  bool validate() const noexcept;

protected:

  Iterator _insert(ConstIterator p, ValueType c);

  const AllocatorType &getAllocator() const noexcept;

  AllocatorType &getAllocator() noexcept;

  ValueType *doAllocate(SizeType n);

  void doFree(ValueType *p, SizeType n);

  SizeType getNewCapacity(SizeType nCurCap);

  void allocateSelf();

  void allocateSelf(SizeType n);

  void deallocateSelf();

  void rangeInitialize(const ValueType *pBegin, const ValueType *pEnd);

  void rangeInitialize(const ValueType *pBegin);

  void sizeInitialize(SizeType n, ValueType c);

protected:

  CompressedPair<Layout, AllocatorType> mPair;

private:
};

//
// Global operators
//
template<typename TType, typename TAllocator>
inline bool operator==(const BasicString<TType, TAllocator> &a, const BasicString<TType, TAllocator> &b) {
  return ((a.size() == b.size()) &&
          (memcmp(a.data(), b.data(), (size_t) a.size() * sizeof(typename BasicString<TType, TAllocator>::ValueType)) ==
           0));
}

template<typename TType, typename TAllocator>
inline bool
operator==(const typename BasicString<TType, TAllocator>::ValueType *a, const BasicString<TType, TAllocator> &b) {
  typedef typename BasicString<TType, TAllocator>::SizeType SizeType;
  const SizeType n = (SizeType) CharStrlen(a);
  return ((n == b.size()) &&
          (memcmp(a, b.data(), (size_t) n * sizeof(typename BasicString<TType, TAllocator>::ValueType)) == 0));
}

template<typename TType, typename TAllocator>
inline bool
operator==(const BasicString<TType, TAllocator> &a, const typename BasicString<TType, TAllocator>::ValueType *b) {
  typedef typename BasicString<TType, TAllocator>::SizeType SizeType;
  const SizeType n = (SizeType) CharStrlen(b);
  return ((a.size() == n) &&
          (memcmp(a.data(), b, (size_t) n * sizeof(typename BasicString<TType, TAllocator>::ValueType)) == 0));
}

template<typename TType, typename TAllocator>
inline bool operator==(typename BasicString<TType, TAllocator>::ValueType a, const BasicString<TType, TAllocator> &b) {
  typedef typename BasicString<TType, TAllocator>::SizeType SizeType;
  const SizeType n = (SizeType) CharStrlen(&a);
  return ((n == b.size()) &&
          (memcmp(&a, b.data(), (size_t) n * sizeof(typename BasicString<TType, TAllocator>::ValueType)) == 0));
}

template<typename TType, typename TAllocator>
inline bool operator==(const BasicString<TType, TAllocator> &a, typename BasicString<TType, TAllocator>::ValueType b) {
  typedef typename BasicString<TType, TAllocator>::SizeType SizeType;
  const SizeType n = (SizeType) CharStrlen(&b);
  return ((a.size() == n) &&
          (memcmp(a.data(), &b, (size_t) n * sizeof(typename BasicString<TType, TAllocator>::ValueType)) == 0));
}

template<typename TType, typename TAllocator>
inline bool operator!=(const BasicString<TType, TAllocator> &a, const BasicString<TType, TAllocator> &b) {
  return !(a == b);
}

template<typename TType, typename TAllocator>
inline bool
operator!=(const typename BasicString<TType, TAllocator>::ValueType *a, const BasicString<TType, TAllocator> &b) {
  return !(a == b);
}

template<typename TType, typename TAllocator>
inline bool
operator!=(const BasicString<TType, TAllocator> &a, const typename BasicString<TType, TAllocator>::ValueType *b) {
  return !(a == b);
}

template<typename TType, typename TAllocator>
inline bool operator<(const BasicString<TType, TAllocator> &a, const BasicString<TType, TAllocator> &b) {
  return (BasicString<TType, TAllocator>::compare(a.begin(), a.end(), b.begin(), b.end()) < 0);
}

template<typename TType, typename TAllocator>
inline bool
operator<(const typename BasicString<TType, TAllocator>::ValueType *a, const BasicString<TType, TAllocator> &b) {
  return (BasicString<TType, TAllocator>::compare(a, a + CharStrlen(a), b.begin(), b.end()) < 0);
}

template<typename TType, typename TAllocator>
inline bool
operator<(const BasicString<TType, TAllocator> &a, const typename BasicString<TType, TAllocator>::ValueType *b) {
  return (BasicString<TType, TAllocator>::compare(a.begin(), a.end(), b, b + CharStrlen(b)) < 0);
}

template<typename TType, typename TAllocator>
inline bool operator>(const BasicString<TType, TAllocator> &a, const BasicString<TType, TAllocator> &b) {
  return (BasicString<TType, TAllocator>::compare(a.begin(), a.end(), b.begin(), b.end()) > 0);
}

template<typename TType, typename TAllocator>
inline bool
operator>(const typename BasicString<TType, TAllocator>::ValueType *a, const BasicString<TType, TAllocator> &b) {
  return (BasicString<TType, TAllocator>::compare(a, a + CharStrlen(a), b.begin(), b.end()) > 0);
}

template<typename TType, typename TAllocator>
inline bool
operator>(const BasicString<TType, TAllocator> &a, const typename BasicString<TType, TAllocator>::ValueType *b) {
  return (BasicString<TType, TAllocator>::compare(a.begin(), a.end(), b, b + CharStrlen(b)) > 0);
}


template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator>
operator+(const BasicString<TType, TAllocator> &a, const BasicString<TType, TAllocator> &b) {
  if (a.empty()) {
    return b;
  } else if (b.empty()) {
    return a;
  }
  BasicString<TType, TAllocator> result(a);

  result.append(b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator>
operator+(const typename BasicString<TType, TAllocator>::ValueType *a, const BasicString<TType, TAllocator> &b) {
  BasicString<TType, TAllocator> result(a);

  result.append(b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator>
operator+(const BasicString<TType, TAllocator> &a, const typename BasicString<TType, TAllocator>::ValueType *b) {
  BasicString<TType, TAllocator> result(a);

  result.append(b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator>
operator+(typename BasicString<TType, TAllocator>::ValueType a, const BasicString<TType, TAllocator> &b) {
  BasicString<TType, TAllocator> result(a);

  result.append(b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator>
operator+(const BasicString<TType, TAllocator> &a, typename BasicString<TType, TAllocator>::ValueType b) {
  BasicString<TType, TAllocator> result(a);

  result.append(1, b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, float b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%g", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, double b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%g", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, uint8 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, uint16 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, uint32 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, uint64 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, int16 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, int32 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

template<typename TType, typename TAllocator>
inline BasicString<TType, TAllocator> operator+(const BasicString<TType, TAllocator> &a, int64 b) {
  BasicString<TType, TAllocator> result(a);

  result.appendFormat("%d", b);

  return result;
}

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;
typedef BasicString<char16_t> U16String;
typedef BasicString<char32_t> U32String;


template<>
struct Hash<RECore::String> {
  size_t operator()(const RECore::String &p) const {
    return (size_t) Hash<char *>()(p.cstr());
  }
};

template<>
struct Hash<RECore::WString> {
  size_t operator()(const RECore::WString &p) const {
    return (size_t) Hash<wchar_t *>()(p.cstr());
  }
};

template<>
struct Hash<RECore::U16String> {
  size_t operator()(const RECore::U16String &p) const {
    return (size_t) Hash<char16_t *>()(p.cstr());
  }
};

template<>
struct Hash<RECore::U32String> {
  size_t operator()(const RECore::U32String &p) const {
    return (size_t) Hash<char32_t *>()(p.cstr());
  }
};



inline String to_string(int value)
{ return String().appendFormat("%d", value); }
inline String to_string(long value)
{ return String().appendFormat("%ld", value); }
inline String to_string(long long value)
{ return String().appendFormat("%lld", value); }
inline String to_string(unsigned value)
{ return String().appendFormat("%u", value); }
inline String to_string(unsigned long value)
{ return String().appendFormat("%lu", value); }
inline String to_string(unsigned long long value)
{ return String().appendFormat("%llu", value); }
inline String to_string(float value)
{ return String().appendFormat("%f", value); }
inline String to_string(double value)
{ return String().appendFormat("%f", value); }
inline String to_string(long double value)
{ return String().appendFormat("%Lf", value); }

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

namespace std {


template<>
struct hash<RECore::String> {
  size_t operator()(const RECore::String &p) const {
    return (size_t) RECore::Hash<char *>()(p.cstr());
  }
};

template<>
struct hash<RECore::WString> {
  size_t operator()(const RECore::WString &p) const {
    return (size_t) RECore::Hash<wchar_t *>()(p.cstr());
  }
};

template<>
struct hash<RECore::U16String> {
  size_t operator()(const RECore::U16String &p) const {
    return (size_t) RECore::Hash<char16_t *>()(p.cstr());
  }
};

template<>
struct hash<RECore::U32String> {
  size_t operator()(const RECore::U32String &p) const {
    return (size_t) RECore::Hash<char32_t *>()(p.cstr());
  }
};


}

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/String/String.inl"