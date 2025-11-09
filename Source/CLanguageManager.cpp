/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "CLanguageManager.h"

#include <filesystem>
#include <string>

namespace WinRuler
{
	//! Constructor.
	CLanguageManager::CLanguageManager(const wxString& LanguagesPath) :
		m_LanguagesPath(LanguagesPath),
		m_CurrentLanguageFilename(),
		m_CurrentLanguageName(),
		m_CurrentLanguageCode(),
		m_LanguagesFileList(),
		m_LanguageFileLoaded(false),
		m_Translations()
	{
		// Set nulptr to our database pointer.
		m_pDb = nullptr;

		// Populate languages file list.
		ListFilesInDirectory(m_LanguagesPath, m_LanguagesFileList);
	}

	//! Destructor.
	CLanguageManager::~CLanguageManager()
	{
		if (!m_LanguagesFileList.empty())
			m_LanguagesFileList.clear();

		// if database is opened, close it now.
		if (m_pDb)
		{
			sqlite3_close(m_pDb);
			m_pDb = nullptr;
		}
	}

	//! Loads a language file from the specified filename.
	bool CLanguageManager::LoadLanguageFile(const wxString& fileName)
	{
		if (m_LanguagesFileList.empty())
		{
			// Languages file list is empty. Try to populate it using stored
			// languages path.
			int fileCount =
				ListFilesInDirectory(m_LanguagesPath, m_LanguagesFileList);
			if (fileCount <= 0)
			{
				// No files found or an error occurred.
				wxLogInfo("There is no language files!");

				m_LanguageFileLoaded = false;

				return false;
			}
		}

		// Check if the specified file exists in the languages file list. If
		// it does, proceed to load it.
		for (const wxString& entry : m_LanguagesFileList)
		{
			if (entry == fileName)
			{
				// File found, proceed to load it.
				m_CurrentLanguageFilename = fileName;

				// If database is already open, close it now.
				if (m_pDb)
				{
					sqlite3_close(m_pDb);
					m_pDb = nullptr;

					wxLogInfo("Language database was not closed.");
				}

				// Prepare proper path to our language file.
				wxString dbPath = m_LanguagesPath + fileName;

				// Open language database.
				if (sqlite3_open(dbPath.c_str(), &m_pDb) != SQLITE_OK)
				{
					wxLogError("Failed to open language database: %s", dbPath);

					m_LanguageFileLoaded = false;
					return false;
				}
				
				// Prepare our statement.
				const char* sql = "SELECT key, value FROM translations;";
				sqlite3_stmt* stmt;

				if (sqlite3_prepare_v2(
						m_pDb, sql, -1, &stmt, nullptr) != SQLITE_OK)
				{
					wxLogError(
						"Failed to prepare statement for language database!");

					m_LanguageFileLoaded = false;
					return false;
				}

				// Clear our translations unordered map.
				m_Translations.clear();

				// Iterate throu all entries and populate translations map.
				while (sqlite3_step(stmt) == SQLITE_ROW)
				{
					// Read entry key.
					std::string key = 
						reinterpret_cast<const char*>(
							sqlite3_column_text(stmt, 0));

					// Read entry value.
					std::string value = 
						reinterpret_cast<const char*>(
							sqlite3_column_text(stmt, 1));

					// Store it in translations map.
					m_Translations[key] = value;
				}

				// Finalize our statement.
				sqlite3_finalize(stmt);

				// Successfully loaded the language file.
				m_LanguageFileLoaded = true;

				return true;
			}
		}

		// If we reach here, the specified file was not found in the languages
		// file list, so we cannot load proper language file. Return false.
		m_LanguageFileLoaded = false;

		return false;
	}

	//! Reloads a language from the specified filename. If no language was
	//! loaded before, it will load the language file as if it was the first
	//! time (it calls LoadLanguageFile).
	bool CLanguageManager::ReloadLanguageFile(const wxString& fileName)
	{
		bool ret = LoadLanguageFile(fileName);
		m_LanguageFileLoaded = ret;
		
		return ret;
	}

	//! Gets translation string based on specified key.
	wxString CLanguageManager::GetTranslation(const wxString& key) const
	{
		// If language file is not loaded, return <missing> string.
		if (!m_LanguageFileLoaded)
		{
			return "<missing>";
		}
		
		// Find specified key in translations map.
		auto it = m_Translations.find(key);

		// If specified key was found, return its value.
		if (it != m_Translations.end())
		{
			return it->second;
		}

		// If we reach here, return "<missing>".
		return "<missing>";
	}

	//! Gets the currently loaded language name.
	wxString CLanguageManager::GetCurrentLanguageName() const
	{
		return m_CurrentLanguageName;
	}

	//! Gets the currently loaded language code.
	wxString CLanguageManager::GetCurrentLanguageCode() const
	{
		return m_CurrentLanguageCode;
	}

	//! Gets the currently loaded language filename.
	wxString CLanguageManager::GetCurrentLanguageFilename() const
	{
		return m_CurrentLanguageFilename;
	}

	//! Gets the directory path where all language files are stored.
	wxString CLanguageManager::GetLanguagesPath() const
	{
		return m_LanguagesPath;
	}

	//! Sets new directory path where all language files are stored.
	void CLanguageManager::SetLanguagesPath(const wxString& NewPath)
	{
		// Update the languages path with the new one provided.
		m_LanguagesPath = NewPath;

		// Populate the list of language files in the new directory.
		ListFilesInDirectory(m_LanguagesPath, m_LanguagesFileList);
	}

	//! Lists all files in the specified directory and stores their names in
	//! the provided list.
	int CLanguageManager::ListFilesInDirectory(
		const wxString& path, std::list<wxString>& fileList)
	{
		// Clear the file list before populating it.
		fileList.clear();

		try
		{
			std::filesystem::path dirPath(static_cast<std::string>(path));

			// Check if the directory exists and is indeed a directory.
			if (!std::filesystem::exists(dirPath) ||
				!std::filesystem::is_directory(dirPath))
			{
				// Specified path does not exist or is not a directory.
				return -1;
			}

			// Iterate through the directory and collect file names.
			for (const auto& entry : 
					std::filesystem::directory_iterator(dirPath))
			{
				// Check if the entry is a regular file and add its name to
				// the list.
				if (std::filesystem::is_regular_file(entry.status()))
				{
					fileList.push_back(entry.path().filename().string());
				}
			}

			// Return the number of files found.
			return static_cast<int>(fileList.size());
		}
		// In case of any filesystem error, catch it and return -1.
		catch (const std::filesystem::filesystem_error& e)
		{
			wxLogError(
				"There was filesystem error: %s",
				e.what());

			return -1;
		}
	}

	//! Gets the languages file list as std::list<wxString>.
	std::list<wxString>& CLanguageManager::GetLanguagesFileList()
	{
		std::list<wxString> LanguagesFileList;

		if (m_LanguagesFileList.empty())
		{
			// Languages file list is empty. Try to populate it using stored
			// languages path.
			int fileCount =
				ListFilesInDirectory(m_LanguagesPath, m_LanguagesFileList);
			if (fileCount <= 0)
			{
				// No files found or an error occurred.
				wxLogError("There is no language files!");

				m_LanguageFileLoaded = false;

				return LanguagesFileList;
			}
		}
		else
		{
			for (const wxString& entry : m_LanguagesFileList)
				LanguagesFileList.push_back(entry);

			return LanguagesFileList;
		}
	}

} // end namespace WinRuler
