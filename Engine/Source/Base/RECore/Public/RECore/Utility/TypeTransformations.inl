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
#include <climits>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

/**
 * @struct
 * AddConst
 *
 * @tparam T
 */
template<typename T, bool = IsConstV<T> || IsReferenceV<T> || IsFunctionV<T>>
struct AddConstInternal { typedef T Type; };
template<typename T>
struct AddConstInternal<T, false> { typedef const T Type; };

template<typename T>
struct AddConst {
  typedef typename AddConstInternal<T>::Type Type;
};
template<class T>
using AddConstT = typename AddConst<T>::Type;


/**
 * @struct
 * AddVolatile
 *
 * @tparam T
 */
template<typename T, bool = IsVolatileV<T> || IsReferenceV<T> || IsFunctionV<T>>
struct AddVolatileInternal { typedef T Type; };
template<typename T>
struct AddVolatileInternal<T, false> { typedef const T Type; };

template<typename T>
struct AddVolatile {
  typedef typename AddVolatileInternal<T>::Type Type;
};
template<class T>
using AddVolatileT = typename AddVolatile<T>::Type;


/**
 * @struct
 * AddCV
 *
 * @tparam T
 */
template<typename T>
struct AddCV {
  typedef typename AddConst<typename AddVolatile<T>::Type>::Type Type;
};
template<typename T>
using AddCVT = typename AddCV<T>::Type;


/**
 * @struct
 * MakeSigned
 *
 * @tparam T
 */
template<typename T>
struct MakeSigned { typedef T Type; };
template<>
struct MakeSigned<unsigned char> { typedef signed char Type; };
template<>
struct MakeSigned<unsigned short> { typedef signed short Type; };
template<>
struct MakeSigned<unsigned int> { typedef signed int Type; };
template<>
struct MakeSigned<unsigned long> { typedef signed long Type; };
template<>
struct MakeSigned<unsigned long long> { typedef signed long long Type; };
template<>
struct MakeSigned<const unsigned char> { typedef const signed char Type; };
template<>
struct MakeSigned<const unsigned short> { typedef const signed short Type; };
template<>
struct MakeSigned<const unsigned int> { typedef const signed int Type; };
template<>
struct MakeSigned<const unsigned long> { typedef const signed long Type; };
template<>
struct MakeSigned<const unsigned long long> { typedef const signed long long Type; };

template<typename T>
using MakeSignedT = typename MakeSigned<T>::Type;


/**
 * @struct
 * MakeSigned
 *
 * @tparam T
 */
template<typename T>
struct MakeUnsigned { typedef T Type; };
template<>
struct MakeUnsigned<signed char> { typedef unsigned char Type; };
template<>
struct MakeUnsigned<signed short> { typedef unsigned short Type; };
template<>
struct MakeUnsigned<signed int> { typedef unsigned int Type; };
template<>
struct MakeUnsigned<signed long> { typedef unsigned long Type; };
template<>
struct MakeUnsigned<signed long long> { typedef unsigned long long Type; };
template<>
struct MakeUnsigned<const signed char> { typedef const unsigned char Type; };
template<>
struct MakeUnsigned<const signed short> { typedef const unsigned short Type; };
template<>
struct MakeUnsigned<const signed int> { typedef const unsigned int Type; };
template<>
struct MakeUnsigned<const signed long> { typedef const unsigned long Type; };
template<>
struct MakeUnsigned<const signed long long> { typedef const unsigned long long Type; };

template<typename T>
using MakeUnsignedT = typename MakeUnsigned<T>::Type;


/**
 * @struct
 * RemovePointer
 *
 * @tparam T
 */
template<typename T>
struct RemovePointer { typedef T Type; };
template<typename T>
struct RemovePointer<T*> { typedef T Type; };
template<typename T>
struct RemovePointer<T* const> { typedef T Type; };
template<typename T>
struct RemovePointer<T* volatile> { typedef T Type; };
template<typename T>
struct RemovePointer<T* const volatile> { typedef T Type; };
template<class T>
using RemovePointerT = typename RemovePointer<T>::Type;

/**
 * @struct
 * AddPointer
 *
 * @tparam T
 */
template<typename T>
struct AddPointer {
  typedef typename RemoveReference<T>::Type* Type;
};
template<class T>
using AddPointerT = typename AddPointer<T>::Type;


/**
 * @struct
 * RemoveExtent
 *
 * @tparam T
 */
template<typename T>
struct RemoveExtent { typedef T Type; };
template<typename T>
struct RemoveExtent<T[]> { typedef T Type; };
template<typename T, sizeT N>
struct RemoveExtent<T[N]> { typedef T Type; };
template<class T>
using RemoveExtentT = typename RemoveExtent<T>::Type;


/**
 * @struct
 * RemoveAllExtents
 *
 * @tparam T
 */
template<typename T>
struct RemoveAllExtents { typedef T Type; };
template<typename T, sizeT N>
struct RemoveAllExtents<T[N]> { typedef typename RemoveAllExtents<T>::Type Type; };
template<typename T>
struct RemoveAllExtents<T[]> { typedef typename RemoveAllExtents<T>::Type Type; };
template<class T>
using RemoveAllExtentsT = typename RemoveAllExtents<T>::Type;


/**
 * @struct
 * AlignedStorage
 */
#if defined(RE_COMPILER_GNUC)
template<sizeT N, sizeT Align = RE_ALIGN_OF(double)>
struct AlignedStorage {
  struct Type { unsigned char mCharData[N]; } RE_ALIGN(Align);
};
#elif defined(_MSC_VER)
template<sizeT N, sizeT Align>
struct AlignedStorageHelper { struct Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 2> { struct RE_ALIGN(2) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 4> { struct RE_ALIGN(4) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 8> { struct RE_ALIGN(8) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 16> { struct RE_ALIGN(16) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 32> { struct RE_ALIGN(32) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 64> { struct RE_ALIGN(64) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 128> { struct RE_ALIGN(128) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 256> { struct RE_ALIGN(256) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 512> { struct RE_ALIGN(512) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 1024> { struct RE_ALIGN(1024) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 2048> { struct RE_ALIGN(2048) Type { unsigned char mCharData[N]; }; };
template<sizeT N> struct AlignedStorageHelper<N, 4096> { struct RE_ALIGN(4096) Type { unsigned char mCharData[N]; }; };

template<sizeT N, sizeT Align = RE_ALIGN_OF(double)>
struct AlignedStorage {
    typedef typename AlignedStorageHelper<N, Align>::Type Type;
};
#else
template<sizeT N, sizeT Align = RE_ALIGN_OF(double)>
struct AlignedStorage {
  union Type {
    unsigned char mCharData[N];
    struct RE_ALIGN(Align) mStruct{ };
  };
};
#endif


/**
 * @struct
 * AlignedUnion
 *
 * @tparam minSize
 * @tparam Type0
 * @tparam TypeN
 */
template <sizeT minSize, typename Type0, typename ...TypeN>
struct AlignedUnion {
  static const sizeT Size           = StaticMax<minSize, sizeof(Type0), sizeof(TypeN)...>::Value;
  static const sizeT AlignmentValue = StaticMax<RE_ALIGN_OF(Type0), RE_ALIGN_OF(TypeN)...>::Value;

  typedef typename AlignedStorage<Size, AlignmentValue>::Type Type;
};


// UnionCast
template <typename TDestType, typename TSourceType>
TDestType UnionCast(TSourceType sourceValue) {
  union {
    TSourceType sourceValue;
    TDestType   destValue;
  } u;
  u.sourceValue = sourceValue;

  return u.destValue;
}


template<class...>
using VoidT = void;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore