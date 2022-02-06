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
#include "REInput/REInput.h"
#include "REInput/Backend/Provider.h"
#include "REInput/Backend/Linux/LinuxEventDevice.h"
#include "REInput/Backend/Linux/LinuxKeyboardDevice.h"
#include "REInput/Backend/Linux/LinuxMouseDevice.h"
#include "REInput/Input/Devices/Joystick.h"
#include <dirent.h>
#include <fcntl.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Standard input provider for Linux
*/
class LinuxProvider : public Provider
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  LinuxProvider() = delete;

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  */
  explicit LinuxProvider(InputManager& inputManager) :
    Provider(inputManager)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  virtual ~LinuxProvider() override
  {
    // Nothing here
  }

  LinuxProvider& operator= (const LinuxProvider& source) = delete;


  //[-------------------------------------------------------]
  //[ Private virtual Provider functions                    ]
  //[-------------------------------------------------------]
private:
  virtual void queryDevices() override
  {
    // Create a keyboard device
    if (!checkDevice("Keyboard")) {
      // Add device
      LinuxKeyboardDevice *pImpl = new LinuxKeyboardDevice();
      addDevice("Keyboard", new Keyboard(this->mInputManager, std::string("Keyboard"), pImpl));
    }

    // Create a mouse device
    if (!checkDevice("Mouse")) {
      LinuxMouseDevice *pImpl = new LinuxMouseDevice();
      addDevice("Mouse", new Mouse(this->mInputManager, std::string("Mouse"),
                                   pImpl));
    }

    // List devices in "/dev/input/event*"
    DIR *pDir = opendir("/dev/input");
    if (pDir) {
      int nDevice = 0;

      // Read first entry
      dirent *pEntry = readdir(pDir);
      while (pEntry) {
        // Check if filename is "eventX"
        std::string sFilename = pEntry->d_name;
        if (sFilename.substr(0, 5) == "event") {
          // Try to open the device
          int f = open(("/dev/input/" + sFilename).c_str(), O_RDWR | O_NONBLOCK);
          if (f > 0) {
            // Create device
            LinuxEventDevice *pImpl = new LinuxEventDevice(f);
            std::string sName = std::string("Joystick") + std::to_string(nDevice);
            addDevice(sName, new Joystick(this->mInputManager, sName, pImpl));
            nDevice++;
          }
        }

        // Read next entry
        pEntry = readdir(pDir);
      }

      // Be polite and close the directory after we're done...
      closedir(pDir);
    }
  }


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
