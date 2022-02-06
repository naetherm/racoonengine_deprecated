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
#include "RECore/RECore.h"
#include "RECore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Command line option
*/
class CommandLineOption {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Command line option type
		*/
		enum EType {
			OptionFlag,			/**< A flag, e.g. 'command -v' or 'command --verbose' */
			OptionParam,		/**< A parameter, e.g. 'command -f name' or 'command --filename name' */
			OptionArgument		/**< An argument, e.g. 'command name' */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline CommandLineOption();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~CommandLineOption();

		/**
		*  @brief
		*    Get option type
		*
		*  @return
		*    Type of option
		*/
    [[nodiscard]] inline EType getType() const;

		/**
		*  @brief
		*    Set option type
		*
		*  @param[in] nType
		*    Type of option
		*/
		inline void setType(EType nType);

		/**
		*  @brief
		*    Check if this option is required
		*
		*  @return
		*    'true' if the option is required, else 'false'
		*/
    [[nodiscard]] inline bool isRequired() const;

		/**
		*  @brief
		*    Set if this option is required
		*
		*  @param[in] bRequired
		*    'true' if the option is required, else 'false'
		*/
		inline void setRequired(bool bRequired);

		/**
		*  @brief
		*    Get name
		*
		*  @return
		*    Option name (e.g. "Name")
		*/
    [[nodiscard]] inline String getName() const;

		/**
		*  @brief
		*    Set name
		*
		*  @param[in] sName
		*    Option name (e.g. "Name")
		*/
		inline void setName(const String &sName);

		/**
		*  @brief
		*    Get short name
		*
		*  @return
		*    Short option name (e.g. "-a")
		*/
    [[nodiscard]] inline String getShortName() const;

		/**
		*  @brief
		*    Set short name
		*
		*  @param[in] sName
		*    Short option name (e.g. "-a")
		*/
		inline void setShortName(const String &sName);

		/**
		*  @brief
		*    Get long name
		*
		*  @return
		*    Long option name (e.g. "--optiona")
		*/
    [[nodiscard]] inline String getLongName() const;

		/**
		*  @brief
		*    Set long name
		*
		*  @param[in] sName
		*    Long option name (e.g. "--optiona")
		*/
		inline void setLongName(const String &sName);

		/**
		*  @brief
		*    Get description
		*
		*  @return
		*    Description string
		*/
    [[nodiscard]] inline String getDescription() const;

		/**
		*  @brief
		*    Set description
		*
		*  @param[in] sDescription
		*    Description string
		*/
		inline void setDescription(const String &sDescription);

		/**
		*  @brief
		*    Get default value
		*
		*  @return
		*    Default value
		*/
    [[nodiscard]] inline String getDefault() const;

		/**
		*  @brief
		*    Set default value
		*
		*  @param[in] sDefault
		*    Default value
		*/
		inline void setDefault(const String &sDefault);

		/**
		*  @brief
		*    Check if option value is set (either by command line or by default value)
		*
		*  @return
		*    'true' if the value is set, else 'false'
		*/
    [[nodiscard]] inline bool isSet() const;

		/**
		*  @brief
		*    Get option value
		*
		*  @return
		*    Value of option ("true"/"false" for boolean values)
		*/
		[[nodiscard]] inline String getValue() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] sValue
		*    Option value
		*/
		inline void setValue(const String &sValue);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EType	m_nType;		/**< Type of command line option */
		bool	m_bRequired;	/**< Is this option required? */
		String	m_sName;		/**< Name, can be "" except for OptionArgument */
		String	m_sShortName;	/**< Short name (e.g. "-a"), can be "" */
		String	m_sLongName;	/**< Long name (e.g. "--optiona"), can be "" */
		String	m_sDescription;	/**< Description */
		String	m_sDefault;		/**< Default value */
		String	m_sValue;		/**< Current value */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Tools/CommandLineOption.inl"
