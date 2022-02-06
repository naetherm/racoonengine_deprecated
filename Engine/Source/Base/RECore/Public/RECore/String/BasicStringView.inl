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
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include "RECore/Math/Math.h"
#include "RECore/Utility/Algorithm.h"
#include "RECore/Utility/Utility.h"
#include <limits>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView()
  : mpBegin(nullptr), mnCount(0) {

}

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView(const BasicStringView &cSource)
  : mpBegin(cSource.mpBegin), mnCount(cSource.mnCount) {

}

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView(const TCharType *pStr, BasicStringView::size_type nCount)
  : mpBegin(pStr), mnCount(nCount) {

}

template<typename TCharType>
BasicStringView<TCharType>::BasicStringView(const TCharType *pStr)
  : mpBegin(pStr), mnCount(pStr != nullptr ? CharStrlen(pStr) : 0) {

}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference
BasicStringView<TCharType>::operator[](BasicStringView::size_type nIndex) const {
  return this->mpBegin[nIndex];
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::begin() const {
  return this->mpBegin;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::cbegin() const {
  return this->mpBegin;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::end() const {
  return this->mpBegin + this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::cend() const {
  return this->mpBegin + this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_pointer BasicStringView<TCharType>::data() const {
  return this->mpBegin;
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference BasicStringView<TCharType>::front() const {
  return this->mpBegin[0];
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference BasicStringView<TCharType>::back() const {
  return this->mpBegin[this->mnCount - 1];
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_reference
BasicStringView<TCharType>::at(BasicStringView::size_type nIndex) const {
  return this->mpBegin[nIndex];
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type BasicStringView<TCharType>::size() const {
  return this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type BasicStringView<TCharType>::length() const {
  return this->mnCount;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type BasicStringView<TCharType>::maxSize() const {
  return std::numeric_limits<size_type>::max();
}

template<typename TCharType>
bool BasicStringView<TCharType>::empty() const {
  return this->mnCount == 0;
}

template<typename TCharType>
void BasicStringView<TCharType>::swap(BasicStringView &cSource) {
  RECore::Swap(this->mpBegin, cSource.mpBegin);
  RECore::Swap(this->mnCount, cSource.mnCount);
}

template<typename TCharType>
void BasicStringView<TCharType>::removePrefix(BasicStringView::size_type nNumElements) {
  this->mpBegin += nNumElements;
  this->mnCount -= nNumElements;
}

template<typename TCharType>
void BasicStringView<TCharType>::removeSuffix(BasicStringView::size_type nNumElements) {
  this->mnCount -= nNumElements;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::copy(TCharType *pDst, BasicStringView::size_type nCount,
                                 BasicStringView::size_type nPosition) const {
  nCount = RECore::MinAlt(nCount, this->mnCount - nPosition);
  auto *pResult = CharStringUninitializedCopy(this->mpBegin + nPosition, this->mpBegin + nPosition + nCount, pDst);
  return pResult - pDst;
}

template<typename TCharType>
BasicStringView<TCharType>
BasicStringView<TCharType>::substr(BasicStringView::size_type nPosition, BasicStringView::size_type nCount) {
  nCount = RECore::MinAlt(nCount, this->mnCount - nPosition);
  return this_type(this->mpBegin + nPosition, nCount);
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(const TCharType *pBegin1, const TCharType *pEnd1, const TCharType *pBegin2,
                                        const TCharType *pEnd2) {
  const ptrdiff_t n1 = pEnd1 - pBegin1;
  const ptrdiff_t n2 = pEnd2 - pBegin2;
  const ptrdiff_t nMin = RECore::MinAlt(n1, n2);
  const int cmp = compare(pBegin1, pBegin2, (size_type) nMin);

  return (cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0)));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView cSource) const {
  return compare(mpBegin, mpBegin + mnCount, cSource.mpBegin, cSource.mpBegin + cSource.mnCount);
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        BasicStringView cSource) const {
  return substr(nPos1, nCount1).compare(cSource);
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        BasicStringView cSource, BasicStringView::size_type nPos2,
                                        BasicStringView::size_type nCount2) const {
  return substr(nPos1, nCount1).compare(cSource.substr(nPos2, nCount2));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(const TCharType *pSrc) {
  return compare(BasicStringView(pSrc));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        const TCharType *pStr) {
  return substr(nPos1, nCount1).compare(BasicStringView(pStr));
}

template<typename TCharType>
int BasicStringView<TCharType>::compare(BasicStringView::size_type nPos1, BasicStringView::size_type nCount1,
                                        const TCharType *pStr, BasicStringView::size_type nCount2) const {
  return substr(nPos1, nCount1).compare(BasicStringView(pStr, nCount2));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::Find(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  auto *pEnd = mpBegin + mnCount;
  if (((NPOS - cSource.size()) >= nPosition) && (nPosition + cSource.size()) <= mnCount) {
    const value_type *const pTemp = RECore::Search(mpBegin + nPosition, pEnd, cSource.data(),
                                                   cSource.data() + cSource.size());

    if ((pTemp != pEnd) || (cSource.size() == 0))
      return (size_type)(pTemp - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::Find(TCharType cChar, BasicStringView::size_type nPosition) const {
  return Find(BasicStringView(&cChar, 1), nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::Find(const TCharType *pStr, BasicStringView::size_type nPosition,
                                 BasicStringView::size_type nCount) const {
  return Find(BasicStringView(pStr, nCount), nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::Find(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return Find(BasicStringView(pStr), nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rFind(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return rFind(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rFind(TCharType cChar, BasicStringView::size_type nPosition) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + RECore::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = CharTypeStringrFind(pEnd, mpBegin, cChar);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rFind(const TCharType *pStr, BasicStringView::size_type nPosition,
                                  BasicStringView::size_type nNum) const {
  if (nNum <= mnCount) {
    if (nNum) {
      const const_iterator pEnd = mpBegin + RECore::MinAlt(mnCount - nNum, nPosition) + nNum;
      const const_iterator pResult = CharTypeStringRSearch(mpBegin, pEnd, pStr, pStr + nNum);

      if (pResult != pEnd)
        return (size_type)(pResult - mpBegin);
    } else
      return RECore::MinAlt(mnCount, nPosition);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::rFind(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return rFind(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstOf(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return findFirstOf(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstOf(TCharType cChar, BasicStringView::size_type nPosition) const {
  return Find(cChar, nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstOf(const TCharType *pStr, BasicStringView::size_type nPosition,
                                        BasicStringView::size_type nNum) const {
  if ((nPosition < mnCount)) {
    const value_type *const pBegin = mpBegin + nPosition;
    const value_type *const pEnd = mpBegin + mnCount;
    const const_iterator pResult = CharTypeStringfindFirstOf(pBegin, pEnd, pStr, pStr + nNum);

    if (pResult != pEnd)
      return (size_type)(pResult - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstOf(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return findFirstOf(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastOf(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return findLastOf(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastOf(TCharType cChar, BasicStringView::size_type nPosition) const {
  return rFind(cChar, nPosition);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastOf(const TCharType *pStr, BasicStringView::size_type nPosition,
                                       BasicStringView::size_type nNum) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + RECore::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = CharTypeStringrfindFirstOf(pEnd, mpBegin, pStr, pStr + nNum);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastOf(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return findLastOf(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstNotOf(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return findFirstNotOf(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstNotOf(TCharType cChar, BasicStringView::size_type nPosition) const {
  if (nPosition <= mnCount) {
    const auto pEnd = mpBegin + mnCount;
    const const_iterator pResult = CharTypeStringfindFirstNotOf(mpBegin + nPosition, pEnd, &cChar, &cChar + 1);

    if (pResult != pEnd)
      return (size_type)(pResult - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstNotOf(const TCharType *pStr, BasicStringView::size_type nPosition,
                                           BasicStringView::size_type nNum) const {
  if (nPosition <= mnCount) {
    const auto pEnd = mpBegin + mnCount;
    const const_iterator pResult = CharTypeStringfindFirstNotOf(mpBegin + nPosition, pEnd, pStr, pStr + nNum);

    if (pResult != pEnd)
      return (size_type)(pResult - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findFirstNotOf(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return findFirstNotOf(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastNotOf(BasicStringView cSource, BasicStringView::size_type nPosition) const {
  return findLastNotOf(cSource.mpBegin, nPosition, cSource.mnCount);
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastNotOf(TCharType cChar, BasicStringView::size_type nPosition) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + RECore::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = CharTypeStringrfindFirstNotOf(pEnd, mpBegin, &cChar, &cChar + 1);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastNotOf(const TCharType *pStr, BasicStringView::size_type nPosition,
                                          BasicStringView::size_type nNum) const {
  if (mnCount) {
    const value_type *const pEnd = mpBegin + RECore::MinAlt(mnCount - 1, nPosition) + 1;
    const value_type *const pResult = CharTypeStringrfindFirstNotOf(pEnd, mpBegin, pStr, pStr + nNum);

    if (pResult != mpBegin)
      return (size_type)((pResult - 1) - mpBegin);
  }
  return NPOS;
}

template<typename TCharType>
typename BasicStringView<TCharType>::size_type
BasicStringView<TCharType>::findLastNotOf(const TCharType *pStr, BasicStringView::size_type nPosition) const {
  return findLastNotOf(pStr, nPosition, (size_type) CharStrlen(pStr));
}

template<typename TCharType>
bool BasicStringView<TCharType>::startsWith(BasicStringView cSource) const {
  return (Size() >= cSource.size()) && (compare(0, cSource.size(), cSource) == 0);
}

template<typename TCharType>
bool BasicStringView<TCharType>::startsWith(TCharType cChar) const {
  return startsWith(BasicStringView(&cChar, 1));
}

template<typename TCharType>
bool BasicStringView<TCharType>::startsWith(const TCharType *pStr) const {
  return startsWith(BasicStringView(pStr));
}

template<typename TCharType>
bool BasicStringView<TCharType>::endsWith(BasicStringView cSource) const {
  return (Size() >= cSource.size()) && (compare(Size() - cSource.size(), NPOS, cSource) == 0);
}

template<typename TCharType>
bool BasicStringView<TCharType>::endsWith(TCharType cChar) const {
  return endsWith(BasicStringView(&cChar, 1));
}

template<typename TCharType>
bool BasicStringView<TCharType>::endsWith(const TCharType *pStr) const {
  return endsWith(BasicStringView(pStr));
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::begin() const {
  return this->begin();
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::cbegin() const {
  return this->cbegin();
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::end() const {
  return this->end();
}

template<typename TCharType>
typename BasicStringView<TCharType>::const_iterator BasicStringView<TCharType>::cend() const {
  return this->cend();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]