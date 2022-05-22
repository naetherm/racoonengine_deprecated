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
#include "REInput/Backend/ConnectionDevice.h"
#include "REInput/Backend/HID/HIDCapability.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Information about a HID device
*/
class HIDDevice : public ConnectionDevice
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class HID;


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  HIDDevice() :
  // Device data
    m_nVendor(0),
    m_nProduct(0),
    // Device capabilities
    m_nUsagePage(0),
    m_nUsage(0),
    m_nFeatureReportByteLength(0),
    m_nNumberLinkCollectionNodes(0),
    m_nNumberInputButtonCaps(0),
    m_nNumberInputValueCaps(0),
    m_nNumberInputDataIndices(0),
    m_nNumberOutputButtonCaps(0),
    m_nNumberOutputValueCaps(0),
    m_nNumberOutputDataIndices(0),
    m_nNumberFeatureButtonCaps(0),
    m_nNumberFeatureValueCaps(0),
    m_nNumberFeatureDataIndices(0)
  {
    // Set device backend type
    mDeviceBackendType = DeviceBackendType::HID;

    // Set device type
    m_nDeviceType = ConnectionDevice::DeviceTypeHID;
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    HIDDevice to copy from
  */
  explicit HIDDevice(const HIDDevice &cSource) :
  // Device data
    m_sName(cSource.m_sName),
    m_nVendor(cSource.m_nVendor),
    m_nProduct(cSource.m_nProduct),
    // Device capabilities
    m_nUsagePage(cSource.m_nUsagePage),
    m_nUsage(cSource.m_nUsage),
    m_nFeatureReportByteLength(cSource.m_nFeatureReportByteLength),
    m_nNumberLinkCollectionNodes(cSource.m_nNumberLinkCollectionNodes),
    m_nNumberInputButtonCaps(cSource.m_nNumberInputButtonCaps),
    m_nNumberInputValueCaps(cSource.m_nNumberInputValueCaps),
    m_nNumberInputDataIndices(cSource.m_nNumberInputDataIndices),
    m_nNumberOutputButtonCaps(cSource.m_nNumberOutputButtonCaps),
    m_nNumberOutputValueCaps(cSource.m_nNumberOutputValueCaps),
    m_nNumberOutputDataIndices(cSource.m_nNumberOutputDataIndices),
    m_nNumberFeatureButtonCaps(cSource.m_nNumberFeatureButtonCaps),
    m_nNumberFeatureValueCaps(cSource.m_nNumberFeatureValueCaps),
    m_nNumberFeatureDataIndices(cSource.m_nNumberFeatureDataIndices),
    // Controls
    m_lstInputButtons(cSource.m_lstInputButtons),
    m_lstInputValues(cSource.m_lstInputValues),
    m_lstOutputValues(cSource.m_lstOutputValues)
  {
    // Set device type
    m_nDeviceType = ConnectionDevice::DeviceTypeHID;

    // Set input and output report sizes
    m_nInputReportSize  = cSource.m_nInputReportSize;
    m_nOutputReportSize = cSource.m_nOutputReportSize;
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~HIDDevice() override
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Assignment operator
  *
  *  @param[in] cSource
  *    HIDDevice to copy from
  *
  *  @return
  *    Reference to this HIDDevice
  */
  HIDDevice &operator =(const HIDDevice &cSource)
  {
    // Device data
    m_sName							= cSource.m_sName;
    m_nVendor						= cSource.m_nVendor;
    m_nProduct						= cSource.m_nProduct;

    // Device capabilities
    m_nUsagePage					= cSource.m_nUsagePage;
    m_nUsage						= cSource.m_nUsage;
    m_nInputReportSize				= cSource.m_nInputReportSize;
    m_nOutputReportSize				= cSource.m_nOutputReportSize;
    m_nFeatureReportByteLength		= cSource.m_nFeatureReportByteLength;
    m_nNumberLinkCollectionNodes	= cSource.m_nNumberLinkCollectionNodes;
    m_nNumberInputButtonCaps		= cSource.m_nNumberInputButtonCaps;
    m_nNumberInputValueCaps			= cSource.m_nNumberInputValueCaps;
    m_nNumberInputDataIndices		= cSource.m_nNumberInputDataIndices;
    m_nNumberOutputButtonCaps		= cSource.m_nNumberOutputButtonCaps;
    m_nNumberOutputValueCaps		= cSource.m_nNumberOutputValueCaps;
    m_nNumberOutputDataIndices		= cSource.m_nNumberOutputDataIndices;
    m_nNumberFeatureButtonCaps		= cSource.m_nNumberFeatureButtonCaps;
    m_nNumberFeatureValueCaps		= cSource.m_nNumberFeatureValueCaps;
    m_nNumberFeatureDataIndices		= cSource.m_nNumberFeatureDataIndices;

    // Controls
    m_lstInputButtons				= cSource.m_lstInputButtons;
    m_lstInputValues				= cSource.m_lstInputValues;
    m_lstOutputValues				= cSource.m_lstOutputValues;

    // Done
    return *this;
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] cSource
  *    HIDDevice to compare with
  *
  *  @return
  *    'true', if both are equal, else 'false'
  */
  [[nodiscard]] bool operator ==(const HIDDevice &cSource)
  {
    // Device data
    return (m_nVendor						== cSource.m_nVendor &&
            m_nProduct						== cSource.m_nProduct &&
            // Device capabilities
            m_nUsagePage					== cSource.m_nUsagePage &&
            m_nUsage						== cSource.m_nUsage &&
            m_nInputReportSize				== cSource.m_nInputReportSize &&
            m_nOutputReportSize				== cSource.m_nOutputReportSize &&
            m_nFeatureReportByteLength		== cSource.m_nFeatureReportByteLength &&
            m_nNumberLinkCollectionNodes	== cSource.m_nNumberLinkCollectionNodes &&
            m_nNumberInputButtonCaps		== cSource.m_nNumberInputButtonCaps &&
            m_nNumberInputValueCaps			== cSource.m_nNumberInputValueCaps &&
            m_nNumberInputDataIndices		== cSource.m_nNumberInputDataIndices &&
            m_nNumberOutputButtonCaps		== cSource.m_nNumberOutputButtonCaps &&
            m_nNumberOutputValueCaps		== cSource.m_nNumberOutputValueCaps &&
            m_nNumberOutputDataIndices		== cSource.m_nNumberOutputDataIndices &&
            m_nNumberFeatureButtonCaps		== cSource.m_nNumberFeatureButtonCaps &&
            m_nNumberFeatureValueCaps		== cSource.m_nNumberFeatureValueCaps &&
            m_nNumberFeatureDataIndices		== cSource.m_nNumberFeatureDataIndices &&
            // Controls
            m_lstInputButtons				== cSource.m_lstInputButtons &&
            m_lstInputValues				== cSource.m_lstInputValues &&
            m_lstOutputValues				== cSource.m_lstOutputValues);
  }

  /**
  *  @brief
  *    Get device name
  *
  *  @return
  *    Device name
  */
  [[nodiscard]] inline const std::string& getName() const
  {
    return m_sName;
  }

  /**
  *  @brief
  *    Get vendor
  *
  *  @return
  *    Vendor ID
  */
  [[nodiscard]] inline RECore::uint32 getVendor() const
  {
    return m_nVendor;
  }

  /**
  *  @brief
  *    Get product
  *
  *  @return
  *    Product ID
  */
  [[nodiscard]] inline RECore::uint32 getProduct() const
  {
    return m_nProduct;
  }

  /**
  *  @brief
  *    Get usage
  *
  *  @return
  *    Usage ID
  */
  [[nodiscard]] inline RECore::uint16 getUsage() const
  {
    return m_nUsage;
  }

  /**
  *  @brief
  *    Get usage page
  *
  *  @return
  *    Usage page ID
  */
  [[nodiscard]] inline RECore::uint16 getUsagePage() const
  {
    return m_nUsagePage;
  }

  /**
  *  @brief
  *    Get input button controls
  *
  *  @return
  *    List of input button controls
  */
  [[nodiscard]] inline const std::vector<HIDCapability> &getInputButtons() const
  {
    return m_lstInputButtons;
  }

  /**
  *  @brief
  *    Get input value controls
  *
  *  @return
  *    List of input value controls
  */
  [[nodiscard]] inline const std::vector<HIDCapability> &getInputValues() const
  {
    return m_lstInputValues;
  }

  /**
  *  @brief
  *    Get output value controls
  *
  *  @return
  *    List of output value controls
  */
  [[nodiscard]] inline std::vector<HIDCapability> &getOutputValues()
  {
    return m_lstOutputValues;
  }

  /**
  *  @brief
  *    Parse input report
  *
  *  @param[in] pInputReport
  *    HID input report
  *  @param[in] nSize
  *    Size of input report (in bytes)
  */
  inline void parseInputReport(const RECore::uint8 *pInputReport, RECore::uint32 nSize)
  {
    // Call system specific backend function to parse input report data
    parseInputReportData(pInputReport, nSize);

    // TODO(naetherm)
    // This function should contain an own implementation to parse the input report rather than
    // relying on system-specific APIs. However, my attempt to implement one failed because the data indices
    // seem to be awkward and I can't see how to parse the data items correctly. So, let's stick with this
    // for a while, until someone with time and knowledge takes the challenge :-)
  }


  //[-------------------------------------------------------]
  //[ Public virtual HIDDevice functions                    ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Send HID output report
  *
  *  @remarks
  *    This function will send an output report including the current values of all
  *    output values of the device. Use this to update e.g. LEDs or rumble effects.
  */
  inline virtual void sendOutputReport()
  {
    // Call system specific backend function
    sendOutputReportData();

    // TODO(naetherm) This function should contain an own implementation rather than relying on system-specific APIs
  }


  //[-------------------------------------------------------]
  //[ Protected virtual HIDDevice functions                 ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Parse HID input report
  *
  *  @param[in] pInputReport
  *    HID input report
  *  @param[in] nSize
  *    Size of input report (in bytes)
  *
  *  @remarks
  *    Implement this function to parse an input report using system specific
  *    functions (e.g. Windows HID API). This method will be called automatically
  *    by parseInputReport.
  */
  inline virtual void parseInputReportData(const RECore::uint8*, RECore::uint32)
  {
    // To be implemented in derived classes
  }

  /**
  *  @brief
  *    Send HID output report
  *
  *  @remarks
  *    Implement this function to create an output report including the values of all
  *    output values using system specific functions (e.g. Windows HID API). This method
  *    will be called automatically by sendOutputReport.
  */
  inline virtual void sendOutputReportData()
  {
    // To be implemented in derived classes
  }


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  // Device data
  std::string	m_sName;	///< Device name
  RECore::uint32	m_nVendor;	///< Vendor ID
  RECore::uint32	m_nProduct;	///< Product ID

  // Device capabilities
  RECore::uint16 m_nUsagePage;					///< Device usage page
  RECore::uint16 m_nUsage;						///< Device usage
  RECore::uint16 m_nFeatureReportByteLength;	///< Feature report byte length
  RECore::uint16 m_nNumberLinkCollectionNodes;	///< Number of link collection nodes
  RECore::uint16 m_nNumberInputButtonCaps;		///< Number of input buttons
  RECore::uint16 m_nNumberInputValueCaps;		///< Number of input values
  RECore::uint16 m_nNumberInputDataIndices;		///< Number of input data indices
  RECore::uint16 m_nNumberOutputButtonCaps;		///< Number of output buttons
  RECore::uint16 m_nNumberOutputValueCaps;		///< Number of output values
  RECore::uint16 m_nNumberOutputDataIndices;	///< Number of output data indices
  RECore::uint16 m_nNumberFeatureButtonCaps;	///< Number of feature buttons
  RECore::uint16 m_nNumberFeatureValueCaps;		///< Number of feature values
  RECore::uint16 m_nNumberFeatureDataIndices;	///< Number of feature data indices

  // Controls
  std::vector<HIDCapability> m_lstInputButtons;	///< List of input buttons
  std::vector<HIDCapability> m_lstInputValues;	///< List of input values
  std::vector<HIDCapability> m_lstOutputValues;	///< List of output values


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
