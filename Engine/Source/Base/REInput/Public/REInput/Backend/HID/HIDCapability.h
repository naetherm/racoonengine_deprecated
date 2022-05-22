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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Capability of a HID device
*/
class HIDCapability final
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  HIDCapability() :
    m_nReportID(0),
    m_nLinkCollection(0),
    m_nUsagePage(0),
    m_nUsage(0),
    m_nUsageMin(0),
    m_nUsageMax(0),
    m_nDataIndex(0),
    m_nDataIndexMin(0),
    m_nDataIndexMax(0),
    m_nBitSize(0),
    m_nLogicalMin(0),
    m_nLogicalMax(0),
    m_nPhysicalMin(0),
    m_nPhysicalMax(0),
    m_nValue(0)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    Capability to copy from
  */
  explicit HIDCapability(const HIDCapability &cSource) :
    m_nReportID(cSource.m_nReportID),
    m_nLinkCollection(cSource.m_nLinkCollection),
    m_nUsagePage(cSource.m_nUsagePage),
    m_nUsage(cSource.m_nUsage),
    m_nUsageMin(cSource.m_nUsageMin),
    m_nUsageMax(cSource.m_nUsageMax),
    m_nDataIndex(cSource.m_nDataIndex),
    m_nDataIndexMin(cSource.m_nDataIndexMin),
    m_nDataIndexMax(cSource.m_nDataIndexMax),
    m_nBitSize(cSource.m_nBitSize),
    m_nLogicalMin(cSource.m_nLogicalMin),
    m_nLogicalMax(cSource.m_nLogicalMax),
    m_nPhysicalMin(cSource.m_nPhysicalMin),
    m_nPhysicalMax(cSource.m_nPhysicalMax),
    m_nValue(cSource.m_nValue)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  ~HIDCapability()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Assignment operator
  *
  *  @param[in] cSource
  *    Capability to copy from
  *
  *  @return
  *    Reference to this HIDCapability
  */
  HIDCapability &operator =(const HIDCapability &cSource)
  {
    m_nReportID			= cSource.m_nReportID;
    m_nLinkCollection	= cSource.m_nLinkCollection;
    m_nUsagePage		= cSource.m_nUsagePage;
    m_nUsage			= cSource.m_nUsage;
    m_nUsageMin			= cSource.m_nUsageMin;
    m_nUsageMax			= cSource.m_nUsageMax;
    m_nDataIndex		= cSource.m_nDataIndex;
    m_nDataIndexMin		= cSource.m_nDataIndexMin;
    m_nDataIndexMax		= cSource.m_nDataIndexMax;
    m_nBitSize			= cSource.m_nBitSize;
    m_nLogicalMin		= cSource.m_nLogicalMin;
    m_nLogicalMax		= cSource.m_nLogicalMax;
    m_nPhysicalMin		= cSource.m_nPhysicalMin;
    m_nPhysicalMax		= cSource.m_nPhysicalMax;
    m_nValue			= cSource.m_nValue;
    return *this;
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] cSource
  *    Capability to compare with
  *
  *  @return
  *    'true' if both are equal, else 'false'
  */
  [[nodiscard]] bool operator ==(const HIDCapability &cSource) const
  {
    return (m_nReportID	== cSource.m_nReportID &&  m_nLinkCollection == cSource.m_nLinkCollection &&
            m_nUsagePage == cSource.m_nUsagePage && m_nUsage == cSource.m_nUsage &&
            m_nUsageMin == cSource.m_nUsageMin && m_nUsageMax == cSource.m_nUsageMax &&
            m_nDataIndex == cSource.m_nDataIndex && m_nDataIndexMin == cSource.m_nDataIndexMin && m_nDataIndexMax == cSource.m_nDataIndexMax &&
            m_nBitSize == cSource.m_nBitSize && m_nLogicalMin == cSource.m_nLogicalMin &&
            m_nLogicalMax == cSource.m_nLogicalMax && m_nPhysicalMin == cSource.m_nPhysicalMin &&
            m_nPhysicalMax == cSource.m_nPhysicalMax && m_nValue == cSource.m_nValue);
  }


  //[-------------------------------------------------------]
  //[ Public data                                           ]
  //[-------------------------------------------------------]
public:
  // Capability info
  RECore::uint8  m_nReportID;		///< Report ID
  RECore::uint16 m_nLinkCollection;	///< Link collection
  RECore::uint16 m_nUsagePage;		///< Usage page ID
  RECore::uint16 m_nUsage;			///< Usage ID
  RECore::uint16 m_nUsageMin;		///< Usage ID minimum
  RECore::uint16 m_nUsageMax;		///< Usage ID maximum
  RECore::uint16 m_nDataIndex;		///< Data index (index in input report, not byte index)
  RECore::uint16 m_nDataIndexMin;	///< Data index minimum
  RECore::uint16 m_nDataIndexMax;	///< Data index maximum
  RECore::uint16 m_nBitSize;		///< Size of data in bits
  RECore::uint16 m_nLogicalMin;		///< Logical minimum value (e.g. for axes)
  RECore::uint16 m_nLogicalMax;		///< Logical maximum value (e.g. for axes)
  RECore::uint16 m_nPhysicalMin;	///< Physical minimum value (e.g. for axes)
  RECore::uint16 m_nPhysicalMax;	///< Physical maximum value (e.g. for axes)

  // Current data value
  RECore::uint32 m_nValue;	///< Current value of input data


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
