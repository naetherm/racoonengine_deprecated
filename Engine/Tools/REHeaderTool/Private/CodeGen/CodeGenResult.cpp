#include "REHeaderTool/CodeGen/CodeGenResult.h"

using namespace REHeaderTool;

void CodeGenResult::mergeResult(CodeGenResult&& otherResult) noexcept
{
	parsedFiles.insert(parsedFiles.cend(), std::make_move_iterator(otherResult.parsedFiles.cbegin()), std::make_move_iterator(otherResult.parsedFiles.cend()));
	upToDateFiles.insert(upToDateFiles.cend(), std::make_move_iterator(otherResult.upToDateFiles.cbegin()), std::make_move_iterator(otherResult.upToDateFiles.cend()));

	completed &= otherResult.completed;
}