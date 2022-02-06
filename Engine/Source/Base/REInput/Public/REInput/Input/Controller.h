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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


class Control;
class Axis;
class Button;
class Connection;
class InputManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input controller
*
*  @remarks
*    A controller represents an input device, which can either be a real device like e.g. a mouse or joystick,
*    or a virtual device that is used to map real input devices to virtual axes and keys. A controller consists
*    of a list of controls, e.g. buttons or axes and provides methods to obtain the status.
*/
class Controller
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class InputManager;
  friend class Provider;
  friend class Control;


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  typedef std::vector<Control*> Controls;
  typedef std::vector<Connection*> Connections;


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  *  @param[in] controllerType
  *    Controller type
  *  @param[in] sName
  *    Controller name
  *  @param[in] sDescription
  *    Controller description
  */
  inline Controller(InputManager& inputManager, ControllerType controllerType, const std::string &sName, const std::string &sDescription) :
    mInputManager(inputManager),
    mControllerType(controllerType),
    m_sName(sName),
    m_sDescription(sDescription),
    m_bConfirmed(false),
    m_bActive(true),
    m_bChanged(false),
    m_nChar(0)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~Controller()
  {
    // Destroy all connections
    while (!m_lstConnections.empty())
    {
      disconnect(m_lstConnections[0]);
    }
  }

  /**
  *  @brief
  *    Get owner input manager
  *
  *  @return
  *    Owner input manager
  */
  [[nodiscard]] inline InputManager& getInputManager() const
  {
    return mInputManager;
  }

  /**
  *  @brief
  *    Get controller type
  *
  *  @return
  *    Controller type
  */
  [[nodiscard]] inline ControllerType getControllerType() const
  {
    return mControllerType;
  }

  /**
  *  @brief
  *    Get controller name
  *
  *  @return
  *    Name
  */
  [[nodiscard]] inline const std::string& getName() const
  {
    return m_sName;
  }

  /**
  *  @brief
  *    Get controller description
  *
  *  @return
  *    Description
  */
  [[nodiscard]] inline const std::string& getDescription() const
  {
    return m_sDescription;
  }

  /**
  *  @brief
  *    Check if controller is active
  *
  *  @return
  *    'true' if controller is active, else 'false'
  *
  *  @remarks
  *    If a controller is active, it sends out signals when the state of it's controls has changed.
  *    If a controller is not active, no state changes will occur and all input events from connected
  *    devices will be discarded.
  */
  [[nodiscard]] inline bool getActive() const
  {
    return m_bActive;
  }

  /**
  *  @brief
  *    Activate or deactivate controller
  *
  *  @param[in] bActive
  *    'true' if controller is active, else 'false'
  *
  *  @remarks
  *    Virtual controllers can be activated or deactivated, real input devices
  *    are always active and can not be deactivated.
  */
  inline void setActive(bool bActive)
  {
    // Only virtual controller can be activated/deactivated
    if (ControllerType::VIRTUAL == mControllerType)
    {
      // Set active-state
      m_bActive = bActive;
    }
  }

  /**
  *  @brief
  *    Check if the controller's state has changed (for polling)
  *
  *  @return
  *    'true', if the state has changed, else 'false'
  */
  [[nodiscard]] inline bool hasChanged() const
  {
    // Get state
    const bool bChanged = m_bChanged;

    // Reset state
    m_bChanged = false;

    // Return state
    return bChanged;
  }

  /**
  *  @brief
  *    Get all controls of the controller
  *
  *  @return
  *    List of controls
  */
  [[nodiscard]] inline const Controls &getControls() const
  {
    return m_lstControls;
  }

  /**
  *  @brief
  *    Get all buttons
  *
  *  @return
  *    List of buttons
  */
  [[nodiscard]] inline const std::vector<Button*> &getButtons() const
  {
    // Initialize button list
    if (m_lstButtons.empty())
      initControlList(ControlType::BUTTON);

    // Return button list
    return m_lstButtons;
  }

  /**
  *  @brief
  *    Get all axes
  *
  *  @return
  *    List of axes
  */
  [[nodiscard]] inline const std::vector<Axis*> &getAxes() const
  {
    // Initialize axes list
    if (m_lstAxes.empty())
    {
      initControlList(ControlType::AXIS);
    }

    // Return axes list
    return m_lstAxes;
  }

  /**
  *  @brief
  *    Get control with a specific name
  *
  *  @param[in] sName
  *    Name of control
  *
  *  @return
  *    Control, or a null pointer if no control with that name could be found
  */
  [[nodiscard]] inline Control *getControl(const std::string &sName) const
  {
    ControlMap::const_iterator iterator = m_mapControls.find(sName);
    return (m_mapControls.cend() != iterator) ? iterator->second : nullptr;
  }

  /**
  *  @brief
  *    Get character of last button that was hit
  *
  *  @return
  *    Button character (ASCII), can be '\0'
  *
  *  @remarks
  *    This function returns the character code of the last button that was hit (not pressed!).
  *    The character will then be reset to '\0', so the next call will return '\0', until
  *    a new button is first pressed and then released.
  */
  [[nodiscard]] inline char getChar()
  {
    // Get character
    const char nChar = m_nChar;
    m_nChar = 0;

    // Return character
    return nChar;
  }

  /**
  *  @brief
  *    Get connections
  *
  *  @return
  *    List of connections (both incoming and outgoing), do not destroy the returned connection instances!
  *
  *  @remarks
  *    To determine whether a connection is incoming or outgoing, you can check e.g.
  *    "GetOutputControl()->getController()" == this or something similar.
  */
  [[nodiscard]] inline const Connections &getConnections()
  {
    return m_lstConnections;
  }

  /**
  *  @brief
  *    Connect to another controller
  *
  *  @param[in] outputControlName
  *    Name of control of this controller (output control)
  *  @param[in] inputControl
  *    Pointer to control (input control)
  *  @param[in] fScale
  *    Scale factor
  */
  REINPUT_API void connect(const std::string& outputControlName, Control& inputControl, float fScale = 1.0f);

  /**
  *  @brief
  *    Connect to another controller
  *
  *  @param[in] pController
  *    Pointer to controller containing the input controls, shouldn't be a null pointer (but a null pointer is caught internally)
  *  @param[in] sPrefixOut
  *    Prefix for controls of this controller
  *  @param[in] sPrefixIn
  *    Prefix for controls of the other controller
  *
  *  @remarks
  *    This connects all controls of the input controller (pController) to the controls of the output
  *    controller (this), if their names are equal, e.g. pController->"Left" will be connected to this->"Left".
  *    You can also provide a prefix for either or both sides, e.g.: connectAll(pOtherController, "", "Camera")
  *    will connect pController->"CameraLeft" to this->"Left".
  */
  REINPUT_API void connectAll(Controller *pController, const std::string &sPrefixOut, const std::string &sPrefixIn);

  /**
  *  @brief
  *    Disconnect connection
  *
  *  @param[in] pConnection
  *    Connection (must be valid!), on successful disconnect, the given "pConnection" instance becomes invalid
  */
  REINPUT_API void disconnect(Connection *pConnection);


  //[-------------------------------------------------------]
  //[ Public virtual Controller functions                   ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Update device once per frame
  *
  *  @remarks
  *    This function can be used e.g. to reset any data of a device once per frame. Usually this is
  *    not needed, but some devices (e.g. RawInput mice etc.) need to reset their data once per frame.
  *
  *  @note
  *    - The default implementation is empty
  */
  inline virtual void update()
  {
    // Do nothing by default
  }

  /**
  *  @brief
  *    Update output controls (LEDs, effects etc.)
  *
  *  @param[in] pControl
  *    Output control that has been changed, must be valid!
  *
  *  @remarks
  *    This function is called whenever an output control such as LED or Effect has been changed.
  *    A device should use this function to update the specific control state on the device (or update
  *    all output controls at the same time)
  *
  *  @note
  *    - The default implementation is empty
  */
  inline virtual void updateOutputControl(Control*)
  {
    // Do nothing by default
  }


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  Controller &operator =(const Controller &cSource) = delete;

  /**
  *  @brief
  *    Add control
  *
  *  @param[in] pControl
  *    Pointer to control, must be valid!
  */
  REINPUT_API void addControl(Control *pControl);

  /**
  *  @brief
  *    Inform controller that a control has changed it's state
  *
  *  @param[in] pControl
  *    Pointer to control, must be valid!
  */
  REINPUT_API void informControl(Control *pControl);

  /**
  *  @brief
  *    Initialize control list
  *
  *  @param[in] controlType
  *    Control type of list that is to be filled
  */
  REINPUT_API void initControlList(ControlType controlType) const;

  /**
  *  @brief
  *    Add connection
  *
  *  @param[in] pConnection
  *    Connection (must be valid!)
  */
  REINPUT_API void addConnection(Connection *pConnection);

  /**
  *  @brief
  *    Remove connection
  *
  *  @param[in] pConnection
  *    Connection (must be valid!)
  */
  REINPUT_API void removeConnection(Connection *pConnection);


  //[-------------------------------------------------------]
  //[ Protected definitions                                 ]
  //[-------------------------------------------------------]
protected:
  typedef std::unordered_map<std::string, Control*> ControlMap;


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  // Controller information and state
  InputManager&   mInputManager;		///< Owner input manager
  ControllerType	mControllerType;	///< Controller type
  std::string		m_sName;			///< Controller name
  std::string		m_sDescription;		///< Controller description
  bool			m_bConfirmed;		///< Confirmation flag for "detectDevices()"
  bool			m_bActive;			///< Is the controller active?
  mutable bool	m_bChanged;			///< Has the controller's state changed?

  // Controls
  Controls					 m_lstControls;	///< List of all controls
  ControlMap					 m_mapControls;	///< Hash map of name -> control
  mutable std::vector<Button*> m_lstButtons;	///< List of buttons (filled on use)
  mutable std::vector<Axis*>	 m_lstAxes;		///< List of absolute axes (filled on use)
  char						 m_nChar;		///< Last hit key character

  // Connections
  Connections m_lstConnections;	///< List of connections


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
