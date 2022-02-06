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
#include "REInput/Input/Controller.h"
#include "REInput/Input/Connection.h"
#include "REInput/Input/Control/Axis.h"
#include "REInput/Input/Control/Button.h"
#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
void Controller::connect(const std::string& outputControlName, Control& inputControl, float fScale)
{
  // Get output control
  Control *pOutput = getControl(outputControlName);
  if (nullptr != pOutput && (&inputControl != pOutput) && (&inputControl.getController() != &pOutput->getController()))
  {
    // Create connection
    Connection *pConnection = new Connection(inputControl, *pOutput, fScale);
    if (pConnection->isValid())
    {
      // Add connection to both controllers
      inputControl.getController().addConnection(pConnection);
      addConnection(pConnection);
    }
    else
    {
      // Connection is invalid!
      // TODO(naetherm) Log entry
      delete pConnection;
    }
  }
}

void Controller::connectAll(Controller *pController, const std::string &sPrefixOut, const std::string &sPrefixIn)
{
  if (pController) {
    // Get all controls of input controller
    const Controls &lstControls = pController->getControls();
    for (uint32_t i=0; i<lstControls.size(); i++) {
      // Get control
      Control *pInput = lstControls[i];

      // Check if input control has the right prefix
      std::string sNameIn = pInput->getName();
      if (sPrefixIn.empty() || sNameIn.find(sPrefixIn) == std::string::npos) {
        // Get output name
        std::string sRoot = sNameIn.substr(0, sPrefixIn.size());
        std::string sNameOut = sPrefixOut + sRoot;

        // Try to create connection
        connect(sNameOut, *pInput);
      }
    }
  }
}

void Controller::disconnect(Connection *pConnection)
{
  // Check connection
  if (pConnection && std::find(m_lstConnections.cbegin(), m_lstConnections.cend(), pConnection) != m_lstConnections.cend()) {
    // Get other controller
    Controller* controller = &pConnection->getInputControl().getController();
    if (controller == this)
    {
      controller = &pConnection->getOutputControl().getController();
    }

    // Remove connection from both controllers
    controller->removeConnection(pConnection);
    removeConnection(pConnection);

    // Destroy connection
    delete pConnection;
  }
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
void Controller::addControl(Control *pControl)
{
  // Check if control is valid
  if (pControl) {
    // Add control to list
    m_lstControls.push_back(pControl);

    // Add control to hash map
    m_mapControls.emplace(pControl->getName(), pControl);
  }
}

void Controller::informControl(Control *pControl)
{
  // Check if controller is active and control is valid
  if (m_bActive && pControl) {
    // Set changed-flag
    m_bChanged = true;

    // Check if a button was hit
    if (pControl->getControlType() == ControlType::BUTTON && static_cast<Button*>(pControl)->isHit())
    {
      // Save key character
      m_nChar = static_cast<Button*>(pControl)->getCharacter();
    }

    // Check if an output value was changed
    if (pControl->getControlType() == ControlType::LED || pControl->getControlType() == ControlType::EFFECT)
    {
      // Update output control
      updateOutputControl(pControl);
    }

    // Check connections
    for (uint32_t i=0; i<m_lstConnections.size(); i++) {
      // Get connection
      Connection *pConnection = m_lstConnections[i];

      // Check 'direction' that we must take
      if (pControl->isInputControl() && (&pConnection->getInputControl() == pControl))
      {
        // Get the pointer to the controller that owns the output control
        // -> In case there's a controller, do only pass on the control event in case the controller is active
        if (pConnection->getOutputControl().getController().getActive())
        {
          // Input control, pass from connection-input to connection-output
          pConnection->passValue();
        }
      }
      else if (!pControl->isInputControl() && (&pConnection->getOutputControl() == pControl))
      {
        // Get the pointer to the controller that owns the input control
        // -> In case there's a controller, do only pass on the control event in case the controller is active
        if (pConnection->getInputControl().getController().getActive())
        {
          // Output control, pass backwards: from connection-output to connection-input
          pConnection->passValueBackwards();
        }
      }
    }
  }
}

void Controller::initControlList(ControlType controlType) const
{
  // Clear list
  if (ControlType::BUTTON == controlType)
  {
    m_lstButtons.clear();
  }
  else if (ControlType::AXIS == controlType)
  {
    m_lstAxes.clear();
  }

  // Loop over all controls
  for (uint32_t i=0; i<m_lstControls.size(); i++) {
    // Get control
    Control *pControl = m_lstControls[i];

    // Add control to appropriate list
    if (pControl->getControlType() == controlType)
    {
      if (ControlType::BUTTON == controlType)
      {
        m_lstButtons.push_back(static_cast<Button*>(pControl));
      }
      else if (ControlType::AXIS == controlType)
      {
        m_lstAxes.push_back(static_cast<Axis*>(pControl));
      }
    }
  }
}

void Controller::addConnection(Connection *pConnection)
{
  // Avoid duplicate connection entries
  if (pConnection && std::find(m_lstConnections.cbegin(), m_lstConnections.cend(), pConnection) == m_lstConnections.cend()) {
    // Add connection
    m_lstConnections.push_back(pConnection);
  }
}

void Controller::removeConnection(Connection *pConnection)
{
  // Check connection
  if (nullptr != pConnection)
  {
    Connections::const_iterator iterator = std::find(m_lstConnections.cbegin(), m_lstConnections.cend(), pConnection);
    if (m_lstConnections.cend() != iterator)
    {
      // Remove connection
      m_lstConnections.erase(iterator);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
