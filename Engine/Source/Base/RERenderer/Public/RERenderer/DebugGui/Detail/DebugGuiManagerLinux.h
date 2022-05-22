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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERenderer/DebugGui/DebugGuiManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class DebugGuiManagerLinux final : public DebugGuiManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void onWindowResize(RECore::uint32 width, RECore::uint32 heigth);
		void onKeyInput(RECore::uint32 keySym, char character, bool pressed);
		void onMouseMoveInput(int x, int y);
		void onMouseButtonInput(RECore::uint32 button, bool pressed);
		void onMouseWheelInput(bool scrollUp);


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::DebugGuiManager methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual void initializeImGuiKeyMap() override;
		virtual void onNewFrame(RERHI::RHIRenderTarget& renderTarget) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit DebugGuiManagerLinux(IRenderer& renderer) :
			DebugGuiManager(renderer),
			mWindowWidth(0),
			mWindowHeigth(0),
			mTime(0)
		{
			// Nothing here
		}

		inline virtual ~DebugGuiManagerLinux() override
		{
			// Nothing here
		}

		explicit DebugGuiManagerLinux(const DebugGuiManagerLinux&) = delete;
		DebugGuiManagerLinux& operator=(const DebugGuiManagerLinux&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::uint32 mWindowWidth;
		RECore::uint32 mWindowHeigth;
		RECore::uint64 mTime;	//< Holds the time in microseconds


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
