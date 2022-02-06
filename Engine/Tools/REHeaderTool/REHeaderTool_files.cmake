#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2019 - 2022 RacoonStudios
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
  Private/main.cpp

  # CodeGen
  Private/CodeGen/CodeGenHelpers.cpp
  Private/CodeGen/CodeGenManager.cpp
  Private/CodeGen/CodeGenManagerSettings.cpp
  Private/CodeGen/CodeGenModule.cpp
  Private/CodeGen/CodeGenResult.cpp
  Private/CodeGen/CodeGenUnit.cpp
  Private/CodeGen/CodeGenUnitSettings.cpp
  Private/CodeGen/GeneratedFile.cpp
  Private/CodeGen/ICodeGenerator.cpp
  Private/CodeGen/PropertyCodeGen.cpp
  Private/CodeGen/Macro/MacroCodeGenUnit.cpp
  Private/CodeGen/Macro/MacroCodeGenerator.cpp
  Private/CodeGen/Macro/MacroCodeGenModule.cpp
  Private/CodeGen/Macro/MacroCodeGenUnitSettings.cpp
  Private/CodeGen/Macro/MacroPropertyCodeGen.cpp

  # InfoStructures
  Private/InfoStructures/EntityInfo.cpp
  Private/InfoStructures/EnumInfo.cpp
  Private/InfoStructures/EnumValueInfo.cpp
  Private/InfoStructures/FieldInfo.cpp
  Private/InfoStructures/FunctionInfo.cpp
  Private/InfoStructures/MethodInfo.cpp
  Private/InfoStructures/NamespaceInfo.cpp
  Private/InfoStructures/NestedEnumInfo.cpp
  Private/InfoStructures/NestedStructClassInfo.cpp
  Private/InfoStructures/StructClassInfo.cpp
  Private/InfoStructures/StructClassTree.cpp
  Private/InfoStructures/TemplateParamInfo.cpp
  Private/InfoStructures/TypeInfo.cpp
  Private/InfoStructures/VariableInfo.cpp

  # Misc
  Private/Misc/CompilerHelpers.cpp
  Private/Misc/DefaultLogger.cpp
  Private/Misc/EAccessSpecifier.cpp
  Private/Misc/Filesystem.cpp
  Private/Misc/Helpers.cpp
  Private/Misc/Settings.cpp
  Private/Misc/System.cpp
  Private/Misc/TomlUtility.cpp

  # Parsing
  Private/Parsing/ClassParser.cpp
  Private/Parsing/EntityParser.cpp
  Private/Parsing/EnumParser.cpp
  Private/Parsing/EnumValueParser.cpp
  Private/Parsing/FieldParser.cpp
  Private/Parsing/FileParser.cpp
  Private/Parsing/FunctionParser.cpp
  Private/Parsing/MethodParser.cpp
  Private/Parsing/NamespaceParser.cpp
  Private/Parsing/ParsingError.cpp
  Private/Parsing/ParsingSettings.cpp
  Private/Parsing/PropertyParser.cpp
  Private/Parsing/VariableParser.cpp
  Private/Parsing/ParsingResults/ParsingResultBase.cpp

  # Properties
  Private/Properties/PropertyParsingSettings.cpp

  # Threading
  Private/Threading/TaskBase.cpp
  Private/Threading/ThreadPool.cpp
)