#include "REHeaderTool/Misc/EAccessSpecifier.h"

using namespace REHeaderTool;

std::string REHeaderTool::toString(EAccessSpecifier accessSpecifier) noexcept
{
	std::string result;

	switch (accessSpecifier)
	{
		case EAccessSpecifier::Public:
			result = "Public";
			break;

		case EAccessSpecifier::Protected:
			result = "Protected";
			break;

		case EAccessSpecifier::Private:
			result = "Private";
			break;

		case EAccessSpecifier::Invalid:
			result = "Invalid";
			break;
	}

	return result;
}