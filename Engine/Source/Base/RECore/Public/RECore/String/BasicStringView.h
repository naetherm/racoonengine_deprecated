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
#include "RECore/Utility/Iterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TCharType>
class BasicStringView {
public:
  // std::compatibility
  typedef BasicStringView<TCharType>						this_type;
  typedef TCharType 											value_type;
  typedef TCharType* 											pointer;
  typedef const TCharType* 									const_pointer;
  typedef TCharType& 											reference;
  typedef const TCharType& 									const_reference;
  typedef TCharType* 											iterator;
  typedef const TCharType* 									const_iterator;
  typedef RECore::ReverseIterator<iterator> 			reverse_iterator;
  typedef RECore::ReverseIterator<const_iterator> 	const_reverse_iterator;
  typedef size_t 										size_type;
  typedef ptrdiff_t 									difference_type;

  static const constexpr size_type NPOS = size_type(-1);
public:

  BasicStringView();

  BasicStringView(const BasicStringView& cSource);

  BasicStringView(const TCharType* pStr, size_type nCount);

  BasicStringView(const TCharType* pStr);


  BasicStringView& operator=(const BasicStringView& cSource) = default;

  const_reference operator[](size_type nIndex) const;


  const_iterator begin() const;
  const_iterator cbegin() const;
  const_iterator end() const;
  const_iterator cend() const;


  const_pointer data() const noexcept;

  const_reference front() const;

  const_reference back() const;

  const_reference at(size_type nIndex) const;

  size_type size() const;

  size_type length() const;

  size_type maxSize() const;

  bool empty() const;

  void swap(BasicStringView& cSource);

  void removePrefix(size_type nNumElements);

  void removeSuffix(size_type nNumElements);

  size_type copy(TCharType* pDst, size_type nCount, size_type nPosition = 0) const;

  BasicStringView substr(size_type nPosition = 0, size_type nCount = NPOS);

  int compare(BasicStringView cSource) const;

  int compare(size_type nPos1, size_type nCount1, BasicStringView cSource) const;

  int compare(size_type nPos1, size_type nCount1, BasicStringView cSource, size_type nPos2, size_type nCount2) const;

  int compare(const TCharType* pSrc);

  int compare(size_type nPos1, size_type nCount1, const TCharType* pStr);

  int compare(size_type nPos1, size_type nCount1, const TCharType* pStr, size_type nCount2) const;

  size_type Find(BasicStringView cSource, size_type nPosition = 0) const;

  size_type Find(TCharType cChar, size_type nPosition = 0) const;

  size_type Find(const TCharType* pStr, size_type nPosition, size_type nCount) const;

  size_type Find(const TCharType* pStr, size_type nPosition = 0) const;

  size_type rFind(BasicStringView cSource, size_type nPosition = NPOS) const;

  size_type rFind(TCharType cChar, size_type nPosition = NPOS) const;

  size_type rFind(const TCharType* pStr, size_type nPosition, size_type nNum) const;

  size_type rFind(const TCharType* pStr, size_type nPosition = NPOS) const;

  size_type findFirstOf(BasicStringView cSource, size_type nPosition = 0) const;

  size_type findFirstOf(TCharType cChar, size_type nPosition = 0) const;

  size_type findFirstOf(const TCharType* pStr, size_type nPosition, size_type nNum) const;

  size_type findFirstOf(const TCharType* pStr, size_type nPosition = 0) const;

  size_type findLastOf(BasicStringView cSource, size_type nPosition = NPOS) const;

  size_type findLastOf(TCharType cChar, size_type nPosition = NPOS) const;

  size_type findLastOf(const TCharType* pStr, size_type nPosition, size_type nNum) const;

  size_type findLastOf(const TCharType* pStr, size_type nPosition = NPOS) const;

  size_type findFirstNotOf(BasicStringView cSource, size_type nPosition = 0) const;

  size_type findFirstNotOf(TCharType cChar, size_type nPosition = 0) const;

  size_type findFirstNotOf(const TCharType* pStr, size_type nPosition, size_type nNum) const;

  size_type findFirstNotOf(const TCharType* pStr, size_type nPosition = 0) const;

  size_type findLastNotOf(BasicStringView cSource, size_type nPosition = NPOS) const;

  size_type findLastNotOf(TCharType cChar, size_type nPosition = NPOS) const;

  size_type findLastNotOf(const TCharType* pStr, size_type nPosition, size_type nNum) const;

  size_type findLastNotOf(const TCharType* pStr, size_type nPosition = NPOS) const;

  bool startsWith(BasicStringView cSource) const;

  bool startsWith(TCharType cChar) const;

  bool startsWith(const TCharType* pStr) const;

  bool endsWith(BasicStringView cSource) const;

  bool endsWith(TCharType cChar) const;

  bool endsWith(const TCharType* pStr) const;

public:

  static int compare(const TCharType* pBegin1, const TCharType* pEnd1, const TCharType* pBegin2, const TCharType* pEnd2);

  // std::compatibility
public:
  const_iterator begin() const;
  const_iterator cbegin() const;
  const_iterator end() const;
  const_iterator cend() const;
protected:

  const_pointer mpBegin = nullptr;
  size_type mnCount = 0;

private:
};

typedef BasicStringView<char> StringView;
typedef BasicStringView<wchar_t> WStringView;
typedef BasicStringView<char16_t> U16StringView;
typedef BasicStringView<char32_t> U32StringView;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/String/BasicStringView.inl"