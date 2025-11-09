/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#ifndef _CLANGUAGEMANAGER_H_
#define _CLANGUAGEMANAGER_H_

#include <wx/wx.h>
#include <sqlite3.h>
#include <list>
#include <unordered_map>

namespace WinRuler
{
	/**
	 * Class responsible for loading and managing language files.
	 **/
	class CLanguageManager
	{
	public:
		/**
		 * Constructor.
		 * 
		 * \param LanguagesPath		The path to the directory where all
		 *							language files are stored.
		 **/
		CLanguageManager(const wxString& LanguagesPath);

		/**
		 * Destructor.
		 **/
		~CLanguageManager();

		/**
		 * Loads a language file from the specified filename.
		 * 
		 * \param fileName		The filename of the language file that will be
		 *						loaded.
		 * 
		 * \return	Returns true if the language file was loaded successfully,
		 *			false otherwise.
		 **/
		bool LoadLanguageFile(const wxString& fileName);

		/**
		 * Reloads a language from the specified filename. If no language was
		 * loaded before, it will load the language file as if it was the first
		 * time (it calls LoadLanguageFile).
		 *
		 * \param fileName		The filename of the language file that will be
		 *						loaded.
		 *
		 * \return	Returns true if the language file was reloaded
		 *			successfully, false otherwise.
		 **/
		bool ReloadLanguageFile(const wxString& fileName);

		/**
		 * Gets translation string based on specified key.
		 * 
		 * \param key		The key for the translation string.
		 * 
		 * \return	Returns the translation string if found, or "<missing>"
		 *			text if the key does not exist in the loaded language file.
		 **/
		wxString GetTranslation(const wxString& key) const;

		/**
		 * Gets the currently loaded language name.
		 * 
		 * \return	Returns the name of the currently loaded language or an
		 *			empty string if no language is loaded.
		 **/
		wxString GetCurrentLanguageName() const;

		/**
		 * Gets the currently loaded language code.
		 * 
		 * \return	Returns the language code of the currently loaded language
		 *			or an empty string if no language is loaded.
		 **/
		wxString GetCurrentLanguageCode() const;

		/**
		 * Gets the currently loaded language filename.
		 * 
		 * \return	Returns the filename of the currently loaded language or an
		 *			empty string if no language is loaded.
		 **/
		wxString GetCurrentLanguageFilename() const;

		/**
		 * Gets the directory path where all language files are stored.
		 * 
		 * \return	Returns the directory path of the language files.
		 **/
		wxString GetLanguagesPath() const;

		/**
		 * Sets new directory path where all language files are stored.
		 *
		 * \param NewDirectoryPath		The new directory path to set.
		 **/
		void SetLanguagesPath(const wxString& NewPath);

		/**
		 * Gets the languages file list as std::list<wxString>.
		 * 
		 * /return	Returns the languages file list as std::list<wxString>.
		 **/
		std::list<wxString>& GetLanguagesFileList();
	private:
		/**
		 * Lists all files in the specified directory and stores their names
		 * in the provided list.
		 *
		 * \param path		The directory path to list files from.
		 * \param fileList	The list where the names of the files will be
		 *					stored.
		 *
		 * \return	Returns number of files listed in the directory, 0 if no
		 *			files were found or -1 if somehow an error occurred.
		 **/
		int ListFilesInDirectory(
			const wxString& path, std::list<wxString>& fileList);

		//! The directory path where all language files are stored.
		wxString m_LanguagesPath;

		//! The currently loaded language filename.
		wxString m_CurrentLanguageFilename;

		//! The currently loaded language name.
		wxString m_CurrentLanguageName;

		//! The currently loaded language code.
		wxString m_CurrentLanguageCode;

		//! The list of language files found in the specified directory.
		std::list<wxString> m_LanguagesFileList;

		//! Indicates whether a language file has been loaded.
		bool m_LanguageFileLoaded;

		//! SQLite database pointer.
		sqlite3* m_pDb = nullptr;

		//! Translations unordered map.
		std::unordered_map<wxString, wxString> m_Translations;
	};
} // end namespace WinRuler

#endif // _CLANGUAGEMANAGER_H_
