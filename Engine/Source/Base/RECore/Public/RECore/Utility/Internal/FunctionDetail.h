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

#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Utility/Move.h"
#include "RECore/Utility/TypeTraits.h"
#include "RECore/Memory/StdAllocator.h"
#include "RECore/Utility/Memory.h"
#include "RECore/Utility/Internal/FunctionalBase.h"
#include "RECore/Utility/Utility.h"
#include <new>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Internal {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class UnusedClass {};

union FunctorStorageAlignment {
  void (*UnusedFuncPtr)(void);
  void (UnusedClass::*UnusedFuncMemPtr)(void);
  void* UnusedPtr;
};

template<int TSizeInBytes>
struct FunctorStorage {
  template<typename TResult>
  TResult& GetStorageTypeRef() const {
    return *reinterpret_cast<TResult*>(const_cast<char*>(&storage[0]));
  }
  union {
    FunctorStorageAlignment align;
    char storage[TSizeInBytes];
  };
};
template<>
struct FunctorStorage<0> {
  template<typename TResult>
  TResult& GetStorageTypeRef() const {
    return *reinterpret_cast<TResult*>(const_cast<char*>(&storage[0]));
  }
  union {
    FunctorStorageAlignment align;
    char storage[sizeof(FunctorStorageAlignment)];
  };
};



template<typename TFunctor, int TSizeInBytes>
struct IsFunctorInplaceAllocatable {
  static constexpr bool Value =
    sizeof(TFunctor) <= sizeof(FunctorStorage<TSizeInBytes>) &&
      (RECore::AlignmentOfV<FunctorStorage<TSizeInBytes>> % AlignmentOfV<TFunctor>) == 0;
};



template<int TSizeInBytes>
class FunctionBaseDetail {
public:

  using FunctorStorageType = FunctorStorage<TSizeInBytes>;
  FunctorStorageType mStorage;

  enum ManagerOperations : int {
    MGROPS_DESTRUCT_FUNCTOR = 0,
    MGROPS_COPY_FUNCTOR = 1,
    MGROPS_MOVE_FUNCTOR = 2,
#if RE_RTTI_ENABLED
    MGROPS_GET_TYPE_INFO = 3,
    MGROPS_GET_FUNC_PTR = 4,
#endif
  };

  // Functor can be allocated inplace
  template <typename Functor, typename = void>
  class FunctionManagerBase {
    public:

    static Functor* GetFunctorPtr(const FunctorStorageType& storage) noexcept {
      return &(storage.template GetStorageTypeRef<Functor>());
    }

    template <typename T>
    static void CreateFunctor(FunctorStorageType& storage, T&& functor) {
      ::new (GetFunctorPtr(storage)) Functor(Forward<T>(functor));
    }

    static void DestructFunctor(FunctorStorageType& storage) {
      GetFunctorPtr(storage)->~Functor();
    }

    static void CopyFunctor(FunctorStorageType& to, const FunctorStorageType& from) {
      ::new (GetFunctorPtr(to)) Functor(*GetFunctorPtr(from));
    }

    static void MoveFunctor(FunctorStorageType& to, FunctorStorageType& from) noexcept {
      ::new (GetFunctorPtr(to)) Functor(Move(*GetFunctorPtr(from)));
    }

    static void* Manager(void* to, void* from, typename FunctionBaseDetail::ManagerOperations ops) noexcept {
      switch (ops) {
        case MGROPS_DESTRUCT_FUNCTOR: {
          DestructFunctor(*static_cast<FunctorStorageType*>(to));
        }
          break;
        case MGROPS_COPY_FUNCTOR: {
          CopyFunctor(*static_cast<FunctorStorageType*>(to),
                      *static_cast<const FunctorStorageType*>(from));
        }
          break;
        case MGROPS_MOVE_FUNCTOR: {
          MoveFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<FunctorStorageType*>(from));
          DestructFunctor(*static_cast<FunctorStorageType*>(from));
        }
          break;
        default:
          break;
      }
      return nullptr;
    }
  };

  // Functor is allocated on the heap
  template <typename Functor>
  class FunctionManagerBase<Functor, typename EnableIf<!IsFunctorInplaceAllocatable<Functor, TSizeInBytes>::Value>::Type> {
    public:
    static Functor* GetFunctorPtr(const FunctorStorageType& storage) noexcept {
      return storage.template GetStorageTypeRef<Functor*>();
    }

    static Functor*& GetFunctorPtrRef(const FunctorStorageType& storage) noexcept {
      return storage.template GetStorageTypeRef<Functor*>();
    }

    template <typename T>
    static void CreateFunctor(FunctorStorageType& storage, T&& functor) {
      auto& allocator = *(new DEFAULT_ALLOCATOR());
      Functor* func = static_cast<Functor*>(allocator.allocate(sizeof(Functor), alignof(Functor), 0));

      ::new (static_cast<void*>(func)) Functor(Forward<T>(functor));
      GetFunctorPtrRef(storage) = func;
    }

    static void DestructFunctor(FunctorStorageType& storage) {
      Functor* func = GetFunctorPtr(storage);
      if (func) {
        auto& allocator = *(new DEFAULT_ALLOCATOR());
        func->~Functor();
        allocator.deallocate(static_cast<void*>(func), sizeof(Functor));
      }
    }

    static void CopyFunctor(FunctorStorageType& to, const FunctorStorageType& from) {
      auto& allocator = *(new DEFAULT_ALLOCATOR());
      Functor* func = static_cast<Functor*>(allocator.allocate(sizeof(Functor), alignof(Functor), 0));

      ::new (static_cast<void*>(func)) Functor(*GetFunctorPtr(from));
      GetFunctorPtrRef(to) = func;
    }

    static void MoveFunctor(FunctorStorageType& to, FunctorStorageType& from) noexcept {
      Functor* func = GetFunctorPtr(from);
      GetFunctorPtrRef(to) = func;
      GetFunctorPtrRef(from) = nullptr;
    }

    static void* Manager(void* to, void* from, typename FunctionBaseDetail::ManagerOperations ops) noexcept {
      switch (ops) {
        case MGROPS_DESTRUCT_FUNCTOR: {
          DestructFunctor(*static_cast<FunctorStorageType*>(to));
        }
          break;
        case MGROPS_COPY_FUNCTOR: {
          CopyFunctor(*static_cast<FunctorStorageType*>(to),
                      *static_cast<const FunctorStorageType*>(from));
        }
          break;
        case MGROPS_MOVE_FUNCTOR: {
          MoveFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<FunctorStorageType*>(from));
          // Moved ptr, no need to destruct ourselves
        }
          break;
        default:
          break;
      }
      return nullptr;
    }
  };

  template <typename Functor, typename R, typename... Args>
  class FunctionManager final : public FunctionManagerBase<Functor> {
    public:
    using Base = FunctionManagerBase<Functor>;

#if RE_RTTI_ENABLED
    static void* GetTypeInfo() noexcept {
      return reinterpret_cast<void*>(const_cast<std::type_info*>(&typeid(Functor)));
    }

    static void* Manager(void* to, void* from, typename function_base_detail::ManagerOperations ops) noexcept {
      switch (ops) {
        case MGROPS_GET_TYPE_INFO: {
          return GetTypeInfo();
        }
        break;
        case MGROPS_GET_FUNC_PTR: {
          return static_cast<void*>(Base::GetFunctorPtr(*static_cast<FunctorStorageType*>(to)));
        }
        break;
        default: {
          return Base::Manager(to, from, ops);
        }
        break;
      }
    }
#endif // RE_RTTI_ENABLED

    static R Invoker(const FunctorStorageType& functor, Args... args) {
      return RECore::Invoke(*Base::GetFunctorPtr(functor), Forward<Args>(args)...);
    }
  };

  FunctionBaseDetail() noexcept = default;
  ~FunctionBaseDetail() noexcept = default;
};


template<int, typename>
class FunctionDetail;

template<int TSizeInBytes, typename R, typename... Args>
class FunctionDetail<TSizeInBytes, R(Args...)> : public FunctionBaseDetail<TSizeInBytes> {
public:

  using ResultType = R;

protected:

  using Base = FunctionBaseDetail<TSizeInBytes>;
  using FunctorStorageType = typename FunctionBaseDetail<TSizeInBytes>::FunctorStorageType;
  using Base::mStorage;

public:

  FunctionDetail() noexcept = default;

  FunctionDetail(std::nullptr_t) noexcept {

  }

  FunctionDetail(const FunctionDetail& cSource) {
    if (this != &cSource) {
      this->Copy(cSource);
    }
  }

  FunctionDetail(FunctionDetail&& cSource) {
    if (this != &cSource) {
      this->Move(RECore::Move(cSource));
    }
  }

  template<
    typename TFunctor,
    typename = typename RECore::EnableIfT<RECore::IsInvocableRV<R, TFunctor, Args...> && !RECore::IsBaseOfV<FunctionDetail, RECore::DecayT<TFunctor>> & !RECore::IsSameV<RECore::DecayT<TFunctor>, FunctionDetail>>
    >
  FunctionDetail(TFunctor cFunctor) {
    this->CreateForwardFunctor(RECore::Move(cFunctor));
  }

  virtual ~FunctionDetail() {
    this->Destroy();
  }


  FunctionDetail& operator=(const FunctionDetail& cSource) {
    if (this != &cSource) {
      this->Destroy();
      this->Copy(cSource);
    }
    return *this;
  }

  FunctionDetail& operator=(FunctionDetail&& cSource) {
    if (this != &cSource) {
      this->Destroy();
      this->Move(RECore::Move(cSource));
    }
    return *this;
  }

  template<
    typename TFunctor,
    typename = typename RECore::EnableIfT<RECore::IsInvocableRV<R, TFunctor, Args...> && !RECore::IsBaseOfV<FunctionDetail, RECore::DecayT<TFunctor>> & !RECore::IsSameV<RECore::DecayT<TFunctor>, FunctionDetail>>
    >
  FunctionDetail& operator=(TFunctor&& cFunctor) {
    this->Destroy();
    this->CreateForwardFunctor(RECore::Forward<TFunctor>(cFunctor));

    return *this;
  }

  template<typename TFunctor>
  FunctionDetail& operator=(RECore::ReferenceWrapper<TFunctor> cFunctor) noexcept {
    this->Destroy();
    this->CreateForwardFunctor(cFunctor);

    return *this;
  }

  FunctionDetail& operator=(std::nullptr_t) noexcept {
    this->Destroy();
    this->mMgrFuncPtr = nullptr;
    this->mInvokeFuncPtr = nullptr;

    return *this;
  }

  explicit operator bool() const noexcept {
    return this->HaveManager();
  }


  R operator()(Args... args) const {
    return (*this->mInvokeFuncPtr)(this->mStorage, RECore::Forward<Args>(args)...);
  }


  void Swap(FunctionDetail& cSource) noexcept {
    if (this == &cSource) {
      return;
    }
    FunctorStorageType tempStorage;
    if (cSource.HaveManager())
    {
      (void)(*cSource.mMgrFuncPtr)(static_cast<void*>(&tempStorage), static_cast<void*>(&cSource.mStorage),
                                 Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
    }

    if (HaveManager())
    {
      (void)(*mMgrFuncPtr)(static_cast<void*>(&cSource.mStorage), static_cast<void*>(&mStorage),
                           Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
    }

    if (cSource.HaveManager())
    {
      (void)(*cSource.mMgrFuncPtr)(static_cast<void*>(&mStorage), static_cast<void*>(&tempStorage),
                                 Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
    }

    RECore::Swap(mMgrFuncPtr, cSource.mMgrFuncPtr);
    RECore::Swap(mInvokeFuncPtr, cSource.mInvokeFuncPtr);
  }

#if RE_RTTI_ENABLED

  const std::type_info& TargetType() const noexcept;

  template<typename TFunctor>
  TFunctor* Target() noexcept;

  template<typename TFunctor>
  const TFunctor* Target() const noexcept;

#endif

private:

  bool HaveManager() const noexcept {
    return (this->mMgrFuncPtr != nullptr);
  }

  void Destroy() noexcept {
    if (this->HaveManager()) {
      (void)(*this->mMgrFuncPtr)(
        static_cast<void*>(&this->mStorage),
        nullptr,
        Base::ManagerOperations::MGROPS_DESTRUCT_FUNCTOR
      );
    }
  }

  void Copy(const FunctionDetail& cSource) {
    if (this->HaveManager()) {
      (void)(*this->mMgrFuncPtr)(
        static_cast<void*>(&this->mStorage),
        const_cast<void*>(static_cast<const void*>(&cSource.mStorage)),
        Base::ManagerOperations::MGROPS_COPY_FUNCTOR
      );
    }

    this->mMgrFuncPtr = cSource.mMgrFuncPtr;
    this->mInvokeFuncPtr = cSource.mInvokeFuncPtr;
  }

  void Move(FunctionDetail&& cSource) {
    if (this->HaveManager()) {
      (void)(*this->mMgrFuncPtr)(
        static_cast<void*>(&this->mStorage),
        const_cast<void*>(static_cast<const void*>(&cSource.mStorage)),
        Base::ManagerOperations::MGROPS_MOVE_FUNCTOR
      );
    }

    this->mMgrFuncPtr = cSource.mMgrFuncPtr;
    this->mInvokeFuncPtr = cSource.mInvokeFuncPtr;
    cSource.mMgrFuncPtr = nullptr;
    cSource.mInvokeFuncPtr = nullptr;
  }

  template<typename Functor>
  void CreateForwardFunctor(Functor&& cFunctor) {
    using DecayedFunctorType = typename Decay<Functor>::Type;
    using FunctionManagerType = typename Base::template FunctionManager<DecayedFunctorType, R, Args...>;

    if (IsNull(cFunctor)) {
      this->mMgrFuncPtr = nullptr;
      this->mInvokeFuncPtr = nullptr;
    } else {
      this->mMgrFuncPtr = &FunctionManagerType::Manager;
      this->mInvokeFuncPtr = &FunctionManagerType::Invoker;

      FunctionManagerType::CreateFunctor(this->mStorage, Forward<Functor>(cFunctor));
    }
  }

private:

  typedef void* (*ManagerFuncPtr)(void*, void*, typename Base::ManagerOperations);
  typedef R (*InvokeFuncPtr)(const FunctorStorageType&, Args...);

  ManagerFuncPtr mMgrFuncPtr = nullptr;
  InvokeFuncPtr mInvokeFuncPtr = nullptr;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace Internal


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore
