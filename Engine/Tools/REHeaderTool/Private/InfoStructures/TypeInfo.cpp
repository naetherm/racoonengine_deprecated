#include "REHeaderTool/InfoStructures/TypeInfo.h"

#include <iostream>
#include <cassert>
#include <algorithm>

#include "REHeaderTool/Misc/Helpers.h"

using namespace REHeaderTool;

TypeInfo::TypeInfo(CXType cursorType) noexcept:
	sizeInBytes{0}
{
	assert(cursorType.kind != CXTypeKind::CXType_Invalid);

	initialize(cursorType);
}

TypeInfo::TypeInfo(CXCursor cursor) noexcept
{
	initialize(cursor);
}

void TypeInfo::initialize(CXType cursorType) noexcept
{
	CXType	canonicalType = clang_getCanonicalType(cursorType);

	assert(canonicalType.kind != CXTypeKind::CXType_Invalid);

	_fullName			= Helpers::getString(clang_getTypeSpelling(cursorType));
	_canonicalFullName	= Helpers::getString(clang_getTypeSpelling(canonicalType));

	long long size		= clang_Type_getSizeOf(cursorType);

	if (size == CXTypeLayoutError::CXTypeLayoutError_Invalid ||
		size == CXTypeLayoutError::CXTypeLayoutError_Incomplete ||
		size == CXTypeLayoutError::CXTypeLayoutError_Dependent)
	{
		sizeInBytes = 0;
	}
	else
	{
		sizeInBytes = static_cast<size_t>(size);
	}

	//Remove class or struct keyword
	removeForwardDeclaredClassQualifier(_fullName);

	//Fill the descriptors vector
	TypePart*	currTypePart;
	CXType		prevType{ CXTypeKind::CXType_Invalid, { canonicalType.data } };
	CXType		currType		= canonicalType;
	bool		reachedValue	= false;

	while (!reachedValue)
	{
		if (currType.kind == CXTypeKind::CXType_Pointer)
		{
			currTypePart = &typeParts.emplace_back(TypePart{ 0u, ETypeDescriptor::Ptr, 0u });

			prevType = currType;
			currType = clang_getPointeeType(prevType);
		}
		else if (currType.kind == CXTypeKind::CXType_LValueReference)
		{
			currTypePart = &typeParts.emplace_back(TypePart{ 0u, ETypeDescriptor::LRef, 0u });

			prevType = currType;
			currType = clang_getPointeeType(prevType);
		}
		else if (currType.kind == CXTypeKind::CXType_RValueReference)
		{
			currTypePart = &typeParts.emplace_back(TypePart{ 0u, ETypeDescriptor::RRef, 0u });

			prevType = currType;
			currType = clang_getPointeeType(prevType);
		}
		else if (currType.kind == CXTypeKind::CXType_ConstantArray)
		{
			currTypePart = &typeParts.emplace_back(TypePart{ 0u, ETypeDescriptor::CArray, static_cast<uint32_t>(clang_getArraySize(currType)) });

			prevType = currType;
			currType = clang_getArrayElementType(prevType);
		}
		else	//Should be fundamental / record / enum
		{
			currTypePart	= &typeParts.emplace_back(TypePart{ 0u, ETypeDescriptor::Value, 0u });
			reachedValue	= true;
		}

		TypePart& curr = *currTypePart;

		if (clang_isConstQualifiedType(currType))
			curr.descriptor = curr.descriptor | ETypeDescriptor::Const;
		if (clang_isVolatileQualifiedType(currType))
			curr.descriptor = curr.descriptor | ETypeDescriptor::Volatile;
		if (clang_isRestrictQualifiedType(currType))
			curr.descriptor = curr.descriptor | ETypeDescriptor::Restrict;
	}
}

std::string	TypeInfo::computeClassTemplateFullName(CXCursor cursor) noexcept
{
	std::string result = Helpers::getString(clang_getCursorSpelling(cursor));

	CXCursor last = cursor;
	CXCursor current = clang_getCursorSemanticParent(last);
	while (!clang_equalCursors(last, current) && current.kind != CXCursorKind::CXCursor_TranslationUnit)
	{
		result = Helpers::getString(clang_getCursorSpelling(current)) + "::" + result;

		last = current;
		current = clang_getCursorSemanticParent(cursor);
	}

	return result;
}

void TypeInfo::fillTemplateParameters(CXCursor cursor) noexcept
{
	if (cursor.kind == CXCursorKind::CXCursor_ClassTemplate || cursor.kind == CXCursorKind::CXCursor_TemplateTemplateParameter)
	{
		clang_visitChildren(cursor, [](CXCursor cursor, CXCursor /* parent */, CXClientData client_data)
		{
			std::vector<TemplateParamInfo>* parameters = reinterpret_cast<std::vector<TemplateParamInfo>*>(client_data);

			switch (cursor.kind)
			{
				case CXCursorKind::CXCursor_TemplateTypeParameter:
					[[fallthrough]];
				case CXCursorKind::CXCursor_NonTypeTemplateParameter:
					[[fallthrough]];
				case CXCursorKind::CXCursor_TemplateTemplateParameter:
					parameters->emplace_back(cursor);
					break;

				default:
					return CXChildVisitResult::CXChildVisit_Break;
			}

			return CXChildVisitResult::CXChildVisit_Continue;
		}, &_templateParameters);
	}
	else
	{
		clang_visitChildren(cursor, [](CXCursor cursor, CXCursor /* parent */, CXClientData client_data)
		{
			std::vector<TemplateParamInfo>* typenames = reinterpret_cast<std::vector<TemplateParamInfo>*>(client_data);

			switch (cursor.kind)
			{
				case CXCursorKind::CXCursor_TypeRef:
					typenames->emplace_back(cursor);
					break;

				default:
					break;
			}

			return CXChildVisitResult::CXChildVisit_Recurse;
		}, &_templateParameters);
	}
}

void TypeInfo::initialize(CXCursor cursor) noexcept
{
	switch (cursor.kind)
	{
		case CXCursorKind::CXCursor_ClassTemplate:
			_fullName = computeClassTemplateFullName(cursor);
			_canonicalFullName = _fullName;	//TODO: Doesn't support canonical result computation for templates for now

			fillTemplateParameters(cursor);
			break;

		case CXCursorKind::CXCursor_TemplateTemplateParameter:
			_fullName = Helpers::getString(clang_getCursorSpelling(cursor));
			_canonicalFullName = _fullName;

			fillTemplateParameters(cursor);
			break;

		case CXCursorKind::CXCursor_TemplateTypeParameter:
			initialize(clang_getCursorType(cursor));
			break;

		default:
			CXType cursorType = clang_getCursorType(cursor);

			assert(cursorType.kind != CXTypeKind::CXType_Invalid);

			//Template type dependant on some type
			if (clang_Type_getSizeOf(cursorType) == CXTypeLayoutError::CXTypeLayoutError_Dependent &&
				isTemplateTypename(Helpers::getString(clang_getTypeSpelling(cursorType))))
			{
				fillTemplateParameters(cursor);
			}

			initialize(cursorType);
			break;
	}
}

void TypeInfo::removeForwardDeclaredClassQualifier(std::string& parsingStr) noexcept
{
	std::string expectedKeyword = parsingStr.substr(0, 7);

	//Check for the struct keyword
	if (expectedKeyword == _structQualifier)
	{
		parsingStr.erase(0, 7);
	}
	else
	{
		//Check for the class keyword
		expectedKeyword.pop_back();

		if (expectedKeyword == _classQualifier)
		{
			parsingStr.erase(0, 6);
		}
	}
}

void TypeInfo::removeNamespacesAndNestedClasses(std::string& typeString) noexcept
{
	size_t	stringStart		= 0;
	uint8_t	templateLevel;

	//Here comes the actual type result, remove namespace / nested class result
	size_t charIndex;
	while ((charIndex = typeString.find_first_of("<:", stringStart, 2)) != typeString.npos)
	{
		if (typeString.at(charIndex) == ':')	// : implies ::
		{
			typeString.erase(0, charIndex + 2);		// +2 to remove the ::
			stringStart = 0;
		}
		else	//parsingCanonicalStr.at(closingTemplateChar) == '<'
		{
			templateLevel	= 1;
			stringStart		= charIndex + 1;

			while (templateLevel != 0)
			{
				charIndex = typeString.find_first_of("<>", stringStart, 2);

				if (charIndex == typeString.npos)
				{
					assert(false);	//If assert here, means a template type is never closed
				}
				else if (typeString.at(charIndex) == '<')
				{
					templateLevel++;
					stringStart = charIndex + 1;
				}
				else	//parsingCanonicalStr.at(closingTemplateChar) == '>'
				{
					templateLevel--;
					stringStart = charIndex + 1;
				}
			}
		}
	}
}

void TypeInfo::removeTemplateParameters(std::string& typeString) noexcept
{
	size_t closingTemplateChar = typeString.find_last_of(":>");

	if (closingTemplateChar != std::string::npos)
	{
		//If we find a : before a >, means there is no template params on the type
		if (typeString.at(closingTemplateChar) == ':')
		{
			return;
		}

		//Search for the opening <
		size_t openingTemplateChar = typeString.find_last_of('<');
		
		assert(openingTemplateChar < closingTemplateChar); //Assert here means there is no opening < eventhough there is a >

		typeString.erase(openingTemplateChar, closingTemplateChar - openingTemplateChar + 1);
	}
}

bool TypeInfo::removeConstQualifier(std::string& typeString) const noexcept
{
	if (!typeParts.empty() && (typeParts.back().descriptor & ETypeDescriptor::Const))
	{
		size_t charIndex = typeString.rfind(_constQualifier);

		if (charIndex != typeString.npos)
		{
			typeString.erase(charIndex, 5);

			return true;
		}
	}
	
	return false;
}

bool TypeInfo::removeVolatileQualifier(std::string& typeString) const noexcept
{
	if (!typeParts.empty() && (typeParts.back().descriptor & ETypeDescriptor::Volatile))
	{
		size_t charIndex = typeString.rfind(_volatileQualifier);

		if (charIndex != typeString.npos)
		{
			typeString.erase(charIndex, 8);

			return true;
		}
	}

	return false;
}

bool TypeInfo::removeRestrictQualifier(std::string& typeString) const noexcept
{
	if (!typeParts.empty() && (typeParts.back().descriptor & ETypeDescriptor::Restrict))
	{
		size_t charIndex = typeString.rfind(_restrictQualifier);

		if (charIndex != typeString.npos)
		{
			typeString.erase(charIndex, 8);

			return true;
		}
	}

	return false;
}

bool TypeInfo::isTemplateTypename(std::string const& typename_) noexcept
{
	return typename_.find('<') != std::string::npos;
}

std::string TypeInfo::getName(bool removeQualifiers, bool shouldRemoveNamespacesAndNestedClasses, bool shouldRemoveTemplateParameters) const noexcept
{
	std::string result = _fullName;

	if (removeQualifiers)
	{
		removeConstQualifier(result);
		removeVolatileQualifier(result);
		removeRestrictQualifier(result);
	}

	if (shouldRemoveNamespacesAndNestedClasses)
	{
		removeNamespacesAndNestedClasses(result);
	}

	if (shouldRemoveTemplateParameters)
	{
		removeTemplateParameters(result);
	}

	return result;
}

std::string TypeInfo::getCanonicalName(bool removeQualifiers, bool shouldRemoveNamespacesAndNestedClasses) const noexcept
{
	std::string result = _canonicalFullName;

	if (removeQualifiers)
	{
		removeConstQualifier(result);
		removeVolatileQualifier(result);
		removeRestrictQualifier(result);
	}

	if (shouldRemoveNamespacesAndNestedClasses)
	{
		removeNamespacesAndNestedClasses(result);
	}

	return result;
}

std::vector<TemplateParamInfo> const& TypeInfo::getTemplateParameters() const noexcept
{
	return _templateParameters;
}

bool TypeInfo::isTemplateType() const noexcept
{
	return !_templateParameters.empty();
}

std::ostream& REHeaderTool::operator<<(std::ostream& out_stream, TypeInfo const& typeInfo) noexcept
{
	out_stream << typeInfo.getName(false, false);

	return out_stream;
}