#include "REHeaderTool/Parsing/ParsingResults/ParsingResultBase.h"

using namespace REHeaderTool;

void ParsingResultBase::appendResultErrors(ParsingResultBase& result) noexcept
{
	if (!result.errors.empty())
	{
		errors.insert(errors.cend(), std::make_move_iterator(result.errors.cbegin()), std::make_move_iterator(result.errors.cend()));

		result.errors.clear();
	}
}