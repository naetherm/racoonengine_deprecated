#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2021 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////


set(FILES
  Private/REInput.cpp

  # Backend
  Private/Backend/UpdateDevice.cpp
  Private/Backend/Provider.cpp
  Private/Backend/DeviceImpl.cpp
  Private/Backend/ConnectionDevice.cpp

  # Backend/HID
  Private/Backend/HID/HIDProvider.cpp
  Private/Backend/HID/HIDImpl.cpp
  Private/Backend/HID/HIDDevice.cpp
  Private/Backend/HID/HIDCapability.cpp
  Private/Backend/HID/HID.cpp

  # Backend/Bluetooth
  Private/Backend/Bluetooth/BTImpl.cpp
  Private/Backend/Bluetooth/BTDevice.cpp
  Private/Backend/Bluetooth/BluetoothProvider.cpp
  Private/Backend/Bluetooth/Bluetooth.cpp

  # Input
  Private/Input/Connection.cpp
  Private/Input/Controller.cpp
  Private/Input/InputManager.cpp

  # Input/Control
  Private/Input/Control/LED.cpp
  Private/Input/Control/Effect.cpp
  Private/Input/Control/Control.cpp
  Private/Input/Control/Button.cpp
  Private/Input/Control/Axis.cpp

  # Input/Devices
  Private/Input/Devices/WiiMote.cpp
  Private/Input/Devices/SplitTouchPad.cpp
  Private/Input/Devices/SpaceMouse.cpp
  Private/Input/Devices/SensorManager.cpp
  Private/Input/Devices/Mouse.cpp
  Private/Input/Devices/Keyboard.cpp
  Private/Input/Devices/Joystick.cpp
  Private/Input/Devices/Device.cpp

  # Input/Virtual
  Private/Input/Virtual/VirtualController.cpp
  Private/Input/Virtual/VirtualStandardController.cpp
)