/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <unordered_set>
#include <string>

#include "REHeaderTool/Misc/Settings.h"
#include "REHeaderTool/Misc/Filesystem.h"

namespace REHeaderTool
{
	class CodeGenManagerSettings : public Settings
	{
		private:
			/** Section name used in the toml file for CodeGenManager settings. */
			static constexpr char const*	_tomlSectionName	= "CodeGenManagerSettings";

			/**
			*	Collection of files to process for code generation.
			*	These files will be processed without any further check if they exist.
			*/
			std::unordered_set<fs::path, PathHash>	_toProcessFiles;

			/**
			*	Collection of ignored files.
			*	These files will never be processed (except if they are also part of the _toProcessFiles collection).
			*/
			std::unordered_set<fs::path, PathHash>	_ignoredFiles;

			/**
			*	Collection of directories containing files to process for code generation.
			*	All directories contained in the given directories will be recursively inspected, except if they are ignored.
			*	All files contained in any processed directory will be processed, except if they are ignored or if their extension is not contained in _supportedExtensions.
			*/
			std::unordered_set<fs::path, PathHash>	_toProcessDirectories;

			/**
			*	Collection of ignored directories.
			*	All directories contained there will be ignored, except if they are included.
			*	All files contained in any ignored directory will be ignored, except if they are included.
			*/
			std::unordered_set<fs::path, PathHash>	_ignoredDirectories;

			/** Extensions of files that should be considered for code generation. */
			std::unordered_set<std::string>			_supportedFileExtensions;

			/** Dirty flag set if _toProcessFiles hasn't been refreshed since last modification. */
			bool									_toProcessFilesDirtyFlag		= false;

			/** Dirty flag set if _ignoredFiles hasn't been refreshed since last modification. */
			bool									_ignoredFilesDirtyFlag			= false;

			/** Dirty flag set if _toProcessDirectories hasn't been refreshed since last modification. */
			bool									_toProcessDirectoriesDirtyFlag	= false;

			/** Dirty flag set if _ignoredDirectories hasn't been refreshed since last modification. */
			bool									_ignoredDirectoriesDirtyFlag	= false;

			/**
			*	@brief	Transform all existing paths in a collection in their canonical equivalent if they exist.
			*			If a path doesn't exist, it remains unchanged in the collection.
			* 
			*	@param set The set of paths to transform.
			*/
			static void	sanitizePaths(std::unordered_set<fs::path, PathHash>& set)	noexcept;

		protected:
			/**
			*	@brief Load all settings from the provided toml data.
			* 
			*	@param tomlData	Data loaded from a toml file.
			*	@param logger	Optional logger used to issue loading logs. Can be nullptr.
			* 
			*	@return true if no error occured during loading, else false.
			*/
			virtual bool	loadSettingsValues(toml::value const&	tomlData,
											   ILogger*			logger)							noexcept	override;

			/**
			*	@brief	Load the supportedExtensions setting from toml.
			*			Loaded extensions completely replace previously supported extensions if any.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadSupportedFileExtensions(toml::value const&	generationSettings,
														ILogger*			logger)				noexcept;

			/**
			*	@brief	Load the _toProcessFiles setting from toml.
			*			Loaded files completely replace the previous _toProcessFiles.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadToProcessFiles(toml::value const&	generationSettings,
											   ILogger*				logger)						noexcept;

			/**
			*	@brief	Load the _toProcessDirectories setting from toml.
			*			Loaded directories completely replace the previous _toProcessDirectories.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadToProcessDirectories(toml::value const&	generationSettings,
													 ILogger*			logger)					noexcept;

			/**
			*	@brief	Load the _ignoredFiles setting from toml.
			*			Loaded files completely replace previous _ignoredFiles.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadIgnoredFiles(toml::value const&	generationSettings,
											 ILogger*			logger)							noexcept;

			/**
			*	@brief	Load the _ignoredDirectories setting from toml.
			*			Loaded directories completely replace previous _ignoredDirectories.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadIgnoredDirectories(toml::value const&	generationSettings,
												   ILogger*				logger)					noexcept;

		public:
			/**
			*	@brief	Add a file to the list of processed files.
			*			If the path is invalid, doesn't exist, is not a file, or is already in the list, nothing happens.
			*	
			*	@param path Path to the file to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addToProcessFile(fs::path const& path)						noexcept;

			/**
			*	@brief Remove a file from the list of processed files.
			*	
			*	@param path Path to the file to remove.
			*/
			void removeToProcessFile(fs::path const& path)					noexcept;

			/**
			*	@brief Clear the list of files to process.
			*/
			void clearToProcessFiles()										noexcept;

			/**
			*	@brief	Add a directory to the list of parsed directories.
			*			If the path is invalid, doesn't exist, is not a directory, or is already in the list, nothing happens.
			*	
			*	@param path Path to the directory to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addToProcessDirectory(fs::path const& path)				noexcept;

			/**
			*	@brief Remove a directory from the list of processed directories.
			*	
			*	@param path Path to the directory to remove.
			*/
			void removeToProcessDirectory(fs::path const& path)				noexcept;

			/**
			*	@brief Clear the list of directories to process.
			*/
			void clearToProcessDirectories()								noexcept;

			/**
			*	@brief	Add a file to the list of ignored files.
			*			If the path is invalid, doesn't exist, is not a file, or is already in the list, nothing happens.
			*	
			*	@param path Path to the file to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addIgnoredFile(fs::path const& path)						noexcept;

			/**
			*	@brief Remove a file from the list of ignored files.
			*	
			*	@param path Path to the file to remove.
			*/
			void removeIgnoredFile(fs::path const& path)					noexcept;

			/**
			*	@brief Clear the list of ignored files.
			*/
			void clearIgnoredFiles()										noexcept;

			/**
			*	@brief	Add a directory to the list of ignored directories.
			*			If the path is invalid, doesn't exist, is not a directory, or is already in the list, nothing happens.
			*	
			*	@param path Path to the directory to add.
			*	
			*	@return true if the path has been added successfuly, else false.
			*/
			bool addIgnoredDirectory(fs::path const& path)					noexcept;

			/**
			*	@brief Remove a directory from the list of ignored directories.
			*	
			*	@param path Path to the directory to remove.
			*/
			void removeIgnoredDirectory(fs::path const& path)				noexcept;

			/**
			*	@brief Clear the list of ignored directories.
			*/
			void clearIgnoredDirectories()									noexcept;

			/**
			*	@brief	Add a file extension to the list of supported file extensions.
			*			A valid file extension must start with '.' followed by characters.
			*	
			*	@param ext Extension to add.
			*	
			*	@return true if the extension has been added successfuly, else false.
			*/
			bool addSupportedFileExtension(fs::path const& extension)		noexcept;

			/**
			*	@brief	Remove a file extension from the list of supported file extensions.
			*	
			*	@param ext Extension to remove.
			*/
			void removeSupportedFileExtension(fs::path const& extension)	noexcept;

			/**
			*	@brief Clear the list of supported extensions.
			*/
			void clearSupportedFileExtensions()								noexcept;

			/**
			*	@brief	Check whether the provided extension is a supported file extension or not.
			* 
			*	@param extension The extension to check.
			* 
			*	@return true if the file extension is supported, else false.
			*/
			bool isSupportedFileExtension(fs::path const& extension)	const	noexcept;

			/**
			*	@brief	Check whether the provided path is an ignored file or not.
			*			The method is not const to allow the path sanitizer to run if the _ignoredFilesDirtyFlag is set.
			* 
			*	@param file Path to the file.
			* 
			*	@return true if the file is ignored, else false.
			*/
			bool isIgnoredFile(fs::path const& file)							noexcept;

			/**
			*	@brief	Check whether the provided path is an ignored directory or not.
			*			The method is not const to allow the path sanitizer to run if the _ignoredDirectoriesDirtyFlag is set.
			* 
			*	@param directory Path to the directory.
			* 
			*	@return true if the directory is ignored, else false.
			*/
			bool isIgnoredDirectory(fs::path const& directory)					noexcept;


			/**
			*	@brief Getter for _toProcessFiles.
			*	
			*	@return _toProcessFiles.
			*/
			std::unordered_set<fs::path, PathHash> const&	getToProcessFiles()			const	noexcept;

			/**
			*	@brief Getter for _toProcessDirectories.
			*	
			*	@return _toProcessDirectories.
			*/
			std::unordered_set<fs::path, PathHash> const&	getToProcessDirectories()	const	noexcept;

			/**
			*	@brief Getter for _ignoredFiles.
			*	
			*	@return _ignoredFiles.
			*/
			std::unordered_set<fs::path, PathHash> const&	getIgnoredFiles()			const	noexcept;

			/**
			*	@brief Getter for _ignoredDirectories.
			*	
			*	@return _ignoredDirectories.
			*/
			std::unordered_set<fs::path, PathHash> const&	getIgnoredDirectories()		const	noexcept;

			/**
			*	@brief Getter for _supportedExtensions.
			*	
			*	@return _supportedExtensions.
			*/
			std::unordered_set<std::string> const&			getSupportedExtensions()	const	noexcept;
	};
}