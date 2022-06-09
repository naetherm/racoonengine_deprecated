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
#include "RERenderer/Resource/ShaderBlueprint/Cache/Preprocessor.h"
#define MOJOSHADER_NO_VERSION_INCLUDE
#include "RERenderer/IRenderer.h"
#include "RERenderer/Context.h"
#include <RECore/Log/Log.h>

#include <src/mojoshader.h>

#include <cstring>	// For "strlen()"

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void Preprocessor::preprocess(const IRenderer& renderer, std::string& source, std::string& result)
	{
		// TODO(naetherm) The usage of MojoShader just as preprocessor is overkill. Find a simpler but still efficient solution. Switch to "mcpp -- a portable C preprocessor" ( http://mcpp.sourceforge.net/ ) ?

		// Disable warnings in external headers, we can't fix them
		PRAGMA_WARNING_PUSH
			PRAGMA_WARNING_DISABLE_MSVC(5039)	// warning C5039: 'MOJOSHADER_preprocess': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.

			// Preprocess
			const MOJOSHADER_preprocessData* preprocessData = MOJOSHADER_preprocess(nullptr, source.c_str(), static_cast<unsigned int>(source.length()), 0, 0, 0, 0, 0, 0, 0);
		PRAGMA_WARNING_POP

		// Evaluate the result
		if (preprocessData->error_count > 0)
		{
			for (int i = 0; i < preprocessData->error_count; ++i)
			{
        RE_LOG(Critical, std::string("Renderer shader preprocessor ") + (preprocessData->errors[i].filename ? preprocessData->errors[i].filename : "???") + ":"+std::to_string(preprocessData->errors[i].error_position)+": Error: " + std::string(preprocessData->errors[i].error))
			}
		}
		else
		{
			result.assign(preprocessData->output, static_cast<size_t>(preprocessData->output_len));
			replace_all(result, "# version 460 core", "#version 460 core\n");
		}
		MOJOSHADER_freePreprocessData(preprocessData);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
