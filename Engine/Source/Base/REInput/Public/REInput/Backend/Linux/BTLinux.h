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
#include "REInput/Backend/Bluetooth/BTImpl.h"
#include "REInput/Backend/Linux/BTDeviceLinux.h"
#include <dbus-1.0/dbus/dbus.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux implementation of 'BTImpl'
*/
class BTLinux : public BTImpl
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class Bluetooth;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Default constructor
  */
  inline BTLinux()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  virtual ~BTLinux() override
  {
    // Clear devices
    for (uint32_t i=0; i<m_lstDevices.size(); i++)
      delete m_lstDevices[i];
    m_lstDevices.clear();
  }

  /**
  *  @brief
  *    Enumerate Bluetooth devices
  *
  *  @param[out] lstDevices
  *    List that will receive the devices
  *  @param[in]  cConnection
  *    Used DBus connection
  */
  void enumerateBluetoothDevices(std::vector<BTDevice*> &lstDevices, DBusConnection &cConnection)
  {
    // Initialize error value
    DBusError sError;
    dbus_error_init(&sError);

    // Listen to signals from org.bluez.Adapter
    dbus_bus_add_match(&cConnection, "type='signal',interface='org.bluez.Adapter'", &sError);
    dbus_connection_flush(&cConnection);
    if (dbus_error_is_set(&sError)) {
      // TODO(naetherm) Log
      // RE_LOG(Error, "BTLinux: DBUS match error (" + std::string(sError.message) + ')')
    } else {
      // Listen for signals
      bool bAbort = false;
      while (!bAbort) {
        // Read next message
        dbus_connection_read_write(&cConnection, 0);
        DBusMessage *pMessage = dbus_connection_pop_message(&cConnection);
        if (pMessage) {
          // Check signal
          if (dbus_message_is_signal(pMessage, "org.bluez.Adapter", "DeviceFound")) {
            // org.bluez.Adapter.DeviceFound
            std::string sDeviceAddress;
            std::string sDeviceName;
            uint32_t nDeviceClass = 0;

            // Get arguments
            DBusMessageIter sIter;
            dbus_message_iter_init(pMessage, &sIter);
            int nType = 0;
            while ((nType = dbus_message_iter_get_arg_type(&sIter)) != DBUS_TYPE_INVALID) {
              // Check argument type
              if (nType == DBUS_TYPE_STRING) {
                // Device address
                char *pszAddress = nullptr;
                dbus_message_iter_get_basic(&sIter, &pszAddress);
                if (pszAddress)
                  sDeviceAddress = pszAddress;
              } else if (nType == DBUS_TYPE_ARRAY) {
                // Get device infos
                DBusMessageIter sArrayIter;
                for (dbus_message_iter_recurse(&sIter, &sArrayIter);
                     dbus_message_iter_get_arg_type(&sArrayIter) != DBUS_TYPE_INVALID;
                     dbus_message_iter_next(&sArrayIter))
                {
                  // Dictionary entry
                  if (dbus_message_iter_get_arg_type(&sArrayIter) == DBUS_TYPE_DICT_ENTRY) {
                    // Get values
                    DBusMessageIter sDictIter;
                    dbus_message_iter_recurse (&sArrayIter, &sDictIter);
                    if (dbus_message_iter_get_arg_type(&sDictIter) == DBUS_TYPE_STRING) {
                      // Get name
                      char *pszName;
                      dbus_message_iter_get_basic(&sDictIter, &pszName);
                      std::string sName = pszName;

                      // Next
                      dbus_message_iter_next(&sDictIter);
                      if (dbus_message_iter_get_arg_type(&sDictIter) == DBUS_TYPE_VARIANT) {
                        DBusMessageIter sVariantIter;
                        dbus_message_iter_recurse(&sDictIter, &sVariantIter);
                        if (dbus_message_iter_get_arg_type(&sVariantIter) == DBUS_TYPE_STRING) {
                          // Get value
                          char *pszValue = nullptr;
                          dbus_message_iter_get_basic(&sVariantIter, &pszValue);

                          // Save value
                          if (sName == "Name" && pszValue) {
                            // Device name
                            sDeviceName = pszValue;
                          }
                        } else if (dbus_message_iter_get_arg_type(&sVariantIter) == DBUS_TYPE_UINT32) {
                          // Get value
                          uint32_t nValue = 0;
                          dbus_message_iter_get_basic(&sVariantIter, &nValue);

                          // Save value
                          if (sName == "Class")
                            nDeviceClass = nValue;
                        }
                      }
                    }
                  }
                }
              }
              dbus_message_iter_next(&sIter);
            }

            // Device info
            // TODO(naetherm) Log
            //RE_LOG(Info, "BTLinux: Found device '" + sDeviceName + "', Address = " + sDeviceAddress)

            // Convert address from string to bytes
            const int nAddress0 = std::atoi(sDeviceAddress.substr( 0, 2).c_str());
            const int nAddress1 = std::atoi(sDeviceAddress.substr( 3, 2).c_str());
            const int nAddress2 = std::atoi(sDeviceAddress.substr( 6, 2).c_str());
            const int nAddress3 = std::atoi(sDeviceAddress.substr( 9, 2).c_str());
            const int nAddress4 = std::atoi(sDeviceAddress.substr(12, 2).c_str());
            const int nAddress5 = std::atoi(sDeviceAddress.substr(15, 2).c_str());

            // Set device info
            BTDeviceLinux *pDevice = new BTDeviceLinux();
            pDevice->m_sName = sDeviceName;
            pDevice->m_nAddress[0] = nAddress5;
            pDevice->m_nAddress[1] = nAddress4;
            pDevice->m_nAddress[2] = nAddress3;
            pDevice->m_nAddress[3] = nAddress2;
            pDevice->m_nAddress[4] = nAddress1;
            pDevice->m_nAddress[5] = nAddress0;
            pDevice->m_nAddress[6] = 0;
            pDevice->m_nAddress[7] = 0;
            pDevice->m_nClass[0] = (nDeviceClass >>  0) & 255;
            pDevice->m_nClass[1] = (nDeviceClass >>  8) & 255;
            pDevice->m_nClass[2] = (nDeviceClass >> 16) & 255;

            // Add device
            m_lstDevices.push_back(pDevice);
            lstDevices.push_back(pDevice);

            // Device found, not stop
            bAbort = true;
          } else if (dbus_message_is_signal(pMessage, "org.bluez.Adapter", "PropertyChanged")) {
            // org.bluez.Adapter.PropertyChanged
            DBusMessageIter sIter;
            dbus_message_iter_init(pMessage, &sIter);
            if (dbus_message_iter_get_arg_type(&sIter) == DBUS_TYPE_STRING) {
              // Get name
              char *pszName;
              dbus_message_iter_get_basic(&sIter, &pszName);
              std::string sName = pszName;

              // 'Discovering'
              if (sName == "Discovering") {
                // Get value
                dbus_message_iter_next(&sIter);
                if (dbus_message_iter_get_arg_type(&sIter) == DBUS_TYPE_VARIANT) {
                  // Get variant
                  DBusMessageIter sVariantIter;
                  dbus_message_iter_recurse(&sIter, &sVariantIter);
                  if (dbus_message_iter_get_arg_type(&sVariantIter) == DBUS_TYPE_BOOLEAN) {
                    // Get device discovery state
                    bool bState = false;
                    dbus_message_iter_get_basic(&sVariantIter, &bState);

                    // Stop loop when Discovering=false
                    if (!bState)
                      bAbort = true;
                  }
                }
              }
            }
          }

          // Release message
          dbus_message_unref(pMessage);
        }
      }
    }

    // De-initialize error value
    dbus_error_free(&sError);
  }


  //[-------------------------------------------------------]
  //[ Private virtual BTImpl functions                      ]
  //[-------------------------------------------------------]
private:
  virtual void enumerateDevices(std::vector<BTDevice*> &lstDevices) override
  {
    // Clear devices
    for (uint32_t i=0; i<m_lstDevices.size(); i++)
      delete m_lstDevices[i];
    m_lstDevices.clear();

    // Initialize DBUS
    //	dbus_threads_init_default();

    // Initialize error value
    DBusError sError;
    dbus_error_init(&sError);

    // Get DBUS connection
    // TODO(naetherm) Log
    // RE_LOG(Info, "BTLinux: Discovering Bluetooth devices")
    DBusConnection *pConnection = dbus_bus_get(DBUS_BUS_SYSTEM, &sError);
    if (pConnection) {
      // Get default Bluetooth adapter
      DBusMessage *pMessage = dbus_message_new_method_call("org.bluez", "/", "org.bluez.Manager", "DefaultAdapter");
      DBusMessage *pReply = dbus_connection_send_with_reply_and_block(pConnection, pMessage, -1, &sError);
      dbus_message_unref(pMessage);
      if (pReply) {
        // Get adapter name
        const char *pszAdapter = nullptr;
        dbus_message_get_args(pReply, &sError, DBUS_TYPE_OBJECT_PATH, &pszAdapter, DBUS_TYPE_INVALID);
        std::string sAdapter = pszAdapter;
        dbus_message_unref(pReply);

        // Set timeout for device discovery
        const char *pszTimeout = "DiscoverableTimeout";
        int nTimeout = TIMEOUT;
        pMessage = dbus_message_new_method_call("org.bluez", sAdapter.c_str(), "org.bluez.Adapter", "SetProperty");
        DBusMessageIter sIter;
        DBusMessageIter sVariantIter;
        dbus_message_iter_init_append(pMessage, &sIter);
        dbus_message_iter_append_basic(&sIter, DBUS_TYPE_STRING, &pszTimeout);
        dbus_message_iter_open_container(&sIter, DBUS_TYPE_VARIANT, DBUS_TYPE_UINT32_AS_STRING, &sVariantIter);
        dbus_message_iter_append_basic(&sVariantIter, DBUS_TYPE_UINT32, &nTimeout);
        dbus_message_iter_close_container(&sIter, &sVariantIter);
        pReply = dbus_connection_send_with_reply_and_block(pConnection, pMessage, -1, &sError);
        if (pReply)
          dbus_message_unref(pReply);
        if (pMessage)
          dbus_message_unref(pMessage);

        if (dbus_error_is_set(&sError)) {
          // TODO(naetherm) Log
          //RE_LOG(Error, "BTLinux (Set timeout for device discovery): DBUS error (" + std::string(sError.message) + ')')
        } else {
          // Start device discovery
          pMessage = dbus_message_new_method_call("org.bluez", sAdapter.c_str(), "org.bluez.Adapter", "StartDiscovery");
          pReply = dbus_connection_send_with_reply_and_block(pConnection, pMessage, -1, &sError);
          if (pReply)
            dbus_message_unref(pReply);
          if (pMessage)
            dbus_message_unref(pMessage);

          if (dbus_error_is_set(&sError)) {}
            // TODO(naetherm) Log
            // RE_LOG(Error, "BTLinux (Start device discovery): DBUS error (" + std::string(sError.message) + ')')
          else {
            enumerateBluetoothDevices(lstDevices, *pConnection);
          }
        }
      }

      // Close connection
      dbus_error_free(&sError);
      dbus_connection_unref(pConnection);
    } else {
      // TODO(naetherm) Log
      // RE_LOG(Error, "BTLinux: Could not create DBUS connection")
    }
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  std::vector<BTDeviceLinux*> m_lstDevices;	///< List of devices


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
