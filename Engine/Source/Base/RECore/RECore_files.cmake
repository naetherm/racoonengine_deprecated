#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2021 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////


set(FILES
  Private/RECore.cpp
  Private/Runtime.cpp

  # Application
  Private/Application/ApplicationContext.cpp
  Private/Application/CoreApplication.cpp
  Private/Application/CoreContext.cpp

  # Asset
  Private/Asset/AssetManager.cpp
  Private/Asset/AssetPackage.cpp
  Private/Asset/Loader/AssetPackageLoader.cpp

  # Color
  Private/Color/Color4.cpp

  # Config
  Private/Config/Config.cpp
  Private/Config/ConfigSection.cpp

  # File
  Private/File/FileSystemHelper.cpp
  Private/File/MemoryFile.cpp
  Private/File/FileWatcher.cpp

  # Frontend
  Private/Frontend/AbstractFrontend.cpp
  Private/Frontend/Frontend.cpp
  Private/Frontend/FrontendApplication.cpp
  Private/Frontend/FrontendContext.cpp
  Private/Frontend/FrontendImpl.cpp
  Private/Frontend/FrontendOS.cpp
  Private/Frontend/FrontendRacoonEngine.cpp
  Private/Frontend/WindowDescription.cpp
  Private/Frontend/WindowOS.cpp

  # Log
  Private/Log/Log.cpp
  Private/Log/LogFormatter.cpp
  Private/Log/LogFormatterText.cpp

  # Math
  Private/Math/EulerAngles.cpp
  Private/Math/Frustum.cpp
  Private/Math/Math.cpp
  Private/Math/Transform.cpp
  Private/Math/Vec2i.cpp

  # Memory
  Private/Memory/Memory.cpp
  Private/Memory/StdAllocator.cpp

  # Null
  Private/Null/FrontendNull.cpp

  # Reflect
  Private/Reflect/Object.cpp
  Private/Reflect/Module.cpp
  Private/Reflect/MemberDesc.cpp
  Private/Reflect/ClassReal.cpp
  Private/Reflect/ClassManager.cpp
  Private/Reflect/ClassImpl.cpp
  Private/Reflect/ClassDummy.cpp
  Private/Reflect/Class.cpp
  Private/Reflect/TypeInfo/DynTypeInfo.cpp
  Private/Reflect/Event/EventHandlerDesc.cpp
  Private/Reflect/Event/EventDesc.cpp
  Private/Reflect/Event/DynEventHandler.cpp
  Private/Reflect/Event/DynEvent.cpp
  Private/Reflect/Tools/ParamsParser.cpp
  Private/Reflect/Func/FuncDesc.cpp
  Private/Reflect/Func/DynSignature.cpp
  Private/Reflect/Func/DynParams.cpp
  Private/Reflect/Func/DynFunc.cpp
  Private/Reflect/Func/ConstructorDesc.cpp
  Private/Reflect/Var/VarDesc.cpp
  Private/Reflect/Var/DynVar.cpp

  # Resource
  Private/Resource/ResourceStreamer.cpp
  Private/Resource/IResource.cpp
  Private/Resource/IResourceListener.cpp

  # Scripting
  Private/Scripting/Script.cpp
  Private/Scripting/ScriptBinding.cpp
  Private/Scripting/ScriptManager.cpp

  # Time
  Private/Time/TimeManager.cpp
  Private/Time/Stopwatch.cpp

  # Tools
  Private/Tools/CommandLine.cpp
  Private/Tools/CommandLineOption.cpp
  Private/Tools/CommandLineArguments.cpp
  Private/Tools/Version.cpp

  # Platform
  Private/Platform/Console.cpp
  Private/Platform/Platform.cpp

  # System
  Private/System/DynLib.cpp
)