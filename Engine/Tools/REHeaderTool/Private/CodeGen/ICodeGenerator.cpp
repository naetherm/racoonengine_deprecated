#include "REHeaderTool/CodeGen/ICodeGenerator.h"

using namespace REHeaderTool;

int32_t ICodeGenerator::getGenerationOrder() const noexcept
{
	return 0;
}

uint8_t ICodeGenerator::getIterationCount() const noexcept
{
	return 1u;
}