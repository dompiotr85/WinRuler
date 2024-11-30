/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include <wx/wx.h>
#include <wx/display.h>
#include "CMainFrame.h"

namespace WinRuler
{
	static int Clamp(int value, int min, int max)
	{
		if (value <= min)
		{
			return 0;
		}
		else if (value < max)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}

	BEGIN_EVENT_TABLE(CMainFrame, wxFrame)

	// Mouse events handler.
	EVT_MOUSE_EVENTS(CMainFrame::OnMouseEvent)

	// Catch menu events.
	EVT_MENU(ID_OPTIONS, CMainFrame::OnOptionsClicked)
	EVT_MENU(ID_NEW_RULER_LENGTH, CMainFrame::OnNewRulerLengthClicked)
	EVT_MENU(ID_PIXELS_AS_UNIT, CMainFrame::OnPixelsAsUnitClicked)
	EVT_MENU(ID_CENTIMETRES_AS_UNIT, CMainFrame::OnCentimetresAsUnitClicked)
	EVT_MENU(ID_INCHES_AS_UNIT, CMainFrame::OnInchesAsUnitClicked)
	EVT_MENU(ID_PICAS_AS_UNIT, CMainFrame::OnPicasAsUnitClicked)
	EVT_MENU(ID_ALWAYS_ON_TOP, CMainFrame::OnAlwaysOnTopClicked)
	EVT_MENU(ID_RULER_POSITION_SCALE_ON_LEFT, CMainFrame::OnScaleOnLeftClicked)
	EVT_MENU(ID_RULER_POSITION_SCALE_ON_RIGHT, CMainFrame::OnScaleOnRightClicked)
	EVT_MENU(ID_RULER_POSITION_SCALE_ON_TOP, CMainFrame::OnScaleOnTopClicked)
	EVT_MENU(ID_RULER_POSITION_SCALE_ON_BOTTOM, CMainFrame::OnScaleOnBottomClicked)
	EVT_MENU(wxID_ABOUT, CMainFrame::OnAboutClicked)
	EVT_MENU(wxID_CLOSE, CMainFrame::OnCloseClicked)

	END_EVENT_TABLE()

	CMainFrame::CMainFrame(const wxString& Title) :
		wxFrame(
			nullptr, wxID_ANY, Title, wxDefaultPosition, wxDefaultSize,
			wxBORDER_NONE | wxCLIP_CHILDREN | wxSTAY_ON_TOP)
	{
		// Initialize CMainFrame.
		Init();

		// Create CMainFrame controls.
		CreateControls();

		// Setup sizers.
		SetupSizers();

		// Load all settings of our application.
		LoadApplicationSettings();

		// Try to apply all loaded settings now when everything is stored and
		// our MainFrame need special changes.
		ChangeRulerLength(m_iRulerLength);
		ChangeRulerPosition(m_eRulerPosition);
		ChangeRulerUnitOfMeasurement(m_eRulerUnits);
		StayOnTop(m_bAlwaysOnTop);
		if ((m_eRulerBackgroundType == ERulerBackgroundType::btImage) &&
			(!LoadAndPrepareRulerBackgroundImage()))
		{
			wxLogError("Can not load ruler background image!");
		}

		if (m_bRulerTransparency)
		{
			if (CanSetTransparent())
			{
				SetTransparent(m_iRulerTransparencyValue);
			}
		}
		else
		{
			SetTransparent(255);
		}

		// Bind OnExit event for command button clicked event.
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CMainFrame::OnExit, this, wxID_EXIT);

		// Bind OnClose event for close window event.
		Bind(wxEVT_CLOSE_WINDOW, &CMainFrame::OnClose, this);

		// Bind OnMove event for window move event.
		Bind(wxEVT_MOVE, &CMainFrame::OnMove, this);

		// Initialize Border Dragging.
		BorderDragInit();
	}

	CMainFrame::~CMainFrame()
	{
		// Save application settings to database.
		SaveApplicationSettings();

		// Unbind command button clicked event.
		this->Unbind(
			wxEVT_COMMAND_BUTTON_CLICKED, &CMainFrame::OnExit, this,
			wxID_EXIT);

		// Release COptionsDialog instance.
		wxDELETE(m_pOptionsDialog);

		// Release CAboutDialog instance.
		wxDELETE(m_pAboutDialog);

		// Release CNewRulerLengthDialog instance.
		wxDELETE(m_pNewRulerLengthDialog);

		// Release CDrawPanel instance.
		wxDELETE(m_pDrawPanel);
	}

	void CMainFrame::Init()
	{
		//SetDoubleBuffered(true);

		// Ruler's scale position.
		m_eRulerPosition = ERulerPosition::rpTop;

		// Ruler's unit of measurement.
		m_eRulerUnits = ERulerUnits::ruCentimetres;

		// Ruler's background type.
		m_eRulerBackgroundType = ERulerBackgroundType::btGradient;

		// Ruler's scale colour.
		m_cRulerScaleColour = wxColour(0, 0, 0);

		// Ruler's background colour (used when btSolid is background type).
		m_cRulerBackgroundColour = wxColour(255, 164, 119);

		// Ruler's background start colour (used when btGradient is background
		// type).
		m_cRulerBackgroundStartColour = wxColour(255, 196, 119);

		// Ruler's background end colour (used when btGradient is background
		// type).
		m_cRulerBackgroundEndColour = wxColour(255, 142, 61);

		// Ruler's length.
		m_iRulerLength = 800;

		// Ruler's minimum length limit.
		m_iRulerMinimumLengthLimit = 100;

		// Ruler AlwaysOnTop state.
		m_bAlwaysOnTop = true;

		// Ruler transparency flag.
		m_bRulerTransparency = false;

		// Ruler transparency value.
		m_iRulerTransparencyValue = 255;

		// First marker position.
		m_iFirstMarkerPosition = 0;

		// Second marker position.
		m_iSecondMarkerPosition = -1;

		// First marker colour.
		m_cFirstMarkerColour = wxColour(0, 0, 255);

		// Second marker colour.
		m_cSecondMarkerColour = wxColour(255, 0, 0);

		m_sRulerBackgroundImagePath = wxString("");
	}

	void CMainFrame::CreateControls()
	{
		// Create new CDrawPanel and store it in m_pDrawPanel.
		m_pDrawPanel = new CDrawPanel(static_cast<wxFrame*>(this));

		// Set double buffered flag for our CDrawPanel instance.
		m_pDrawPanel->SetDoubleBuffered(true);
	}

	void CMainFrame::SetupSizers()
	{
		// Create new wxBoxSizer.
		wxBoxSizer* pSizer = new wxBoxSizer(wxHORIZONTAL);

		// Add m_pDrawPanel to pSizer.
		pSizer->Add(
			m_pDrawPanel,
			wxSizerFlags().Proportion(1).Expand());

		// Set pSizer in CMainFrame.
		SetSizer(pSizer);

		// Set CMainFrame auto layout.
		SetAutoLayout(true);
	}

	bool CMainFrame::LoadAndPrepareRulerBackgroundImage()
	{
		// Verify that there is image file that can be loaded from location
		// specified in m_sRulerBackgroundImagePath.
		if (!wxFile::Exists(m_sRulerBackgroundImagePath))
		{
			// There is no file at specified localization, so log error and
			// return false.
			wxLogError("There is no file at specified localization!");

			return false;
		}

		// Load image from m_sRulerBackgroundImagePath location to Bitmap
		// instance.
		wxBitmap* Bitmap = new wxBitmap(m_sRulerBackgroundImagePath, wxBITMAP_TYPE_PNG);

		// Extract parts of loaded Bitmap into separate bitmaps.
		m_RulerBackgroundBitmapLeftH = Bitmap->GetSubBitmap(wxRect(0, 0, 4, 60));
		m_RulerBackgroundBitmapMiddleH = Bitmap->GetSubBitmap(wxRect(4, 0, 2, 60));
		m_RulerBackgroundBitmapRightH = Bitmap->GetSubBitmap(wxRect(6, 0, 4, 60));

		// Rotate previously created bitmaps and store them in other bitmaps.
		m_RulerBackgroundBitmapTopV =
			static_cast<wxBitmap>(
				m_RulerBackgroundBitmapLeftH.ConvertToImage().Rotate90());
		m_RulerBackgroundBitmapMiddleV =
			static_cast<wxBitmap>(
				m_RulerBackgroundBitmapMiddleH.ConvertToImage().Rotate90());
		m_RulerBackgroundBitmapBottomV =
			static_cast<wxBitmap>(
				m_RulerBackgroundBitmapRightH.ConvertToImage().Rotate90());

		wxDELETE(Bitmap);

		// All operation was successful, return true.
		return true;
	}

	void CMainFrame::OnOptionsClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Options frame requested."));
#endif

		// Create new COptionsDialog instance.
		m_pOptionsDialog = new COptionsDialog(static_cast<wxFrame*>(this));

		// Call COptionsDialog::ShowModal() method. If wxID_OK is returned,
		// then save new options.
		if (m_pOptionsDialog->ShowModal() == wxID_OK)
		{
			// Set ruler background type.
			m_eRulerBackgroundType =
				static_cast<ERulerBackgroundType>(
					m_pOptionsDialog->m_pBackgroundTypeChoice->GetSelection());

			// Set ruler background colour.
			m_cRulerBackgroundColour =
				static_cast<wxColour>(
					m_pOptionsDialog->m_pBackgroundColourPicker->GetColour());

			// Set ruler background start colour and end colour.
			m_cRulerBackgroundStartColour =
				static_cast<wxColour>(
					m_pOptionsDialog->m_pBackgroundStartColourPicker->GetColour());
			m_cRulerBackgroundEndColour =
				static_cast<wxColour>(
					m_pOptionsDialog->m_pBackgroundEndColourPicker->GetColour());

			// Set ruler background image.
			m_sRulerBackgroundImagePath =
				static_cast<wxString>(
					m_pOptionsDialog->m_pBackgroundImagePicker->GetFileName().GetFullPath());

			// Load and prepare ruler background images.
			if ((m_eRulerBackgroundType == ERulerBackgroundType::btImage) &&
				(!LoadAndPrepareRulerBackgroundImage()))
			{
				wxLogError("Can not load ruler background image!");
			}

			// Set ruler scale colour.
			m_cRulerScaleColour =
				static_cast<wxColour>(
					m_pOptionsDialog->m_pRulerScaleColourPicker->GetColour());

			// Set first and second marker colour.
			m_cFirstMarkerColour =
				static_cast<wxColour>(
					m_pOptionsDialog->m_pFirstMarkerColourPicker->GetColour());
			m_cSecondMarkerColour =
				static_cast<wxColour>(
					m_pOptionsDialog->m_pSecondMarkerColourPicker->GetColour());

			// Set ruler transparency.
			m_bRulerTransparency =
				static_cast<bool>(
					m_pOptionsDialog->m_pRulerTransparencyCheckBox->IsChecked());
			m_iRulerTransparencyValue =
				static_cast<wxByte>(
					m_pOptionsDialog->m_pRulerTransparencySlider->GetValue());
			if (m_bRulerTransparency)
			{
				if (CanSetTransparent())
				{
					SetTransparent(m_iRulerTransparencyValue);
				}
			}
			else
			{
				SetTransparent(255);
			}

			// Set snap to edges of the screen distance and snap to other
			// windows distance.
            m_iSnapToEdgesOfScreenDistance =
                m_pOptionsDialog->m_pSnapToEdgesOfScreenSpinCtrl->GetValue();
            m_iSnapToOtherWindowsDistance =
                m_pOptionsDialog->m_pSnapToOtherWindowsSpinCtrl->GetValue();

			// Save all settings of our application.
			SaveApplicationSettings();

			// Refresh CMainFrame.
			Refresh();
		}

		// Release create COptionsDialog instance.
		wxDELETE(m_pOptionsDialog);
	}

	void CMainFrame::OnNewRulerLengthClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("NewRulerLength frame requested"));
#endif

		// Create new CNewRulerLengthDialog instance.
		m_pNewRulerLengthDialog = new CNewRulerLengthDialog(static_cast<wxFrame*>(this));

		// Call CNewRulerLengthDialog::ShowModal() method.
		if (m_pNewRulerLengthDialog->ShowModal() == wxID_OK)
		{
			// Save all settings of our application.
			SaveApplicationSettings();
		}

		// Release created CNewRulerLengthDialog instance.
		wxDELETE(m_pNewRulerLengthDialog);
	}

	void CMainFrame::OnPixelsAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
		wxLogInfo(wxString("Pixels as unit of measurement selected."));
#endif

		// Change ruler's unit of measurement to pixels.
		ChangeRulerUnitOfMeasurement(ERulerUnits::ruPixels);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnCentimetresAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Centimetres as unit of measurement selected."));
#endif

		// Change ruler's unit of measurement to centimetres.
		ChangeRulerUnitOfMeasurement(ERulerUnits::ruCentimetres);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnInchesAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Inches as unit of measurement selected."));
#endif

		// Change ruler's unit of measurement to inches.
		ChangeRulerUnitOfMeasurement(ERulerUnits::ruInches);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnPicasAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Picas as unit of measurement selected."));
#endif

		// Change ruler's unit of measurement to picas.
		ChangeRulerUnitOfMeasurement(ERulerUnits::ruPicas);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnAlwaysOnTopClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(!m_bAlwaysOnTop ? wxString("AlwaysOnTop enabled.") : wxString("AlwaysOnTop disabled."));
#endif

		// Change ruler's StayOnTop state to negative one.
		StayOnTop(!m_bAlwaysOnTop);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnScaleOnLeftClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Scale on left selected."));
#endif

		// Change ruler's position to left.
		ChangeRulerPosition(ERulerPosition::rpLeft);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnScaleOnTopClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Scale on top selected."));
#endif

		// Change ruler's position to top.
		ChangeRulerPosition(ERulerPosition::rpTop);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnScaleOnRightClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Scale on right selected."));
#endif

		// Change ruler's position to right.
		ChangeRulerPosition(ERulerPosition::rpRight);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnScaleOnBottomClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Scale on bottom selected."));
#endif

		// Change ruler's position to bottom.
		ChangeRulerPosition(ERulerPosition::rpBottom);

		// Save all settings of our application.
		SaveApplicationSettings();
	}

	void CMainFrame::OnAboutClicked(wxCommandEvent& WXUNUSED(Event))
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("About dialog requested."));
#endif

		// Create new CAboutDialog instance.
		m_pAboutDialog = new CAboutDialog(static_cast<wxFrame*>(this));

		// Call CAboutDialog::ShowModal() method.
		m_pAboutDialog->ShowModal();

		// Release created CAboutDialog instance.
		wxDELETE(m_pAboutDialog);
	}

	void CMainFrame::OnCloseClicked(wxCommandEvent& Event)
	{
#ifdef _DEBUG
	    wxLogInfo(wxString("Application close requested."));
#endif

		// Call OnExit event.
		OnExit(Event);
	}

	void CMainFrame::OnExit(wxCommandEvent& Event)
	{
		// If user decided that WinRuler should be closed, then ...
		if (wxMessageBox(
				"Are you sure that you want to exit the WinRuler?",
				"WinRuler - Please confirm",
				wxICON_QUESTION | wxYES_NO) == wxYES)
		{
			// ... close CMainFrame.
			this->Close(true);
		}
	}

	void CMainFrame::OnClose(wxCloseEvent& Event)
	{
		// Check, that ALT+F4 was pressed.
		if (wxGetKeyState(WXK_ALT) && wxGetKeyState(WXK_F4))
		{
			// If user decided that WinRuler should be closed, then ...
			if (wxMessageBox(
					"Are you sure that you want to exit the WinRuler?",
					"WinRuler - Plase confirm",
					wxICON_QUESTION | wxYES_NO) == wxYES)
			{
				// .. close CMainFrame.
				this->Close(true);
			}

			// Return.
			return;
		}

		// If ALT+F4 not pressed, ship this Event.
		Event.Skip();
	}

	void CMainFrame::ChangeRulerPosition(ERulerPosition NewPosition)
	{
		// If requested ruler position is already set, return from this method.
		if (NewPosition == m_eRulerPosition)
			return;

		// Set proposed ruler's position as current ruler's position and ...
		m_eRulerPosition = NewPosition;

		// ... set new size of CMainFrame.
		switch (NewPosition)
		{
		case ERulerPosition::rpLeft:
		case ERulerPosition::rpRight:
			// Set vertical position.
			SetSize(60, m_iRulerLength);

			break;
		case ERulerPosition::rpTop:
		case ERulerPosition::rpBottom:
			// Set horizontal position.
			SetSize(m_iRulerLength, 60);

			break;
		}

		// At end refresh CMainFrame.
		Refresh();
	}

	void CMainFrame::StayOnTop(bool State)
	{
		// If requested State is already set, return from this method.
		if (State == m_bAlwaysOnTop)
			return;

		// Update current StayOnTop state and ...
		m_bAlwaysOnTop = State;

		// ... set CMainFrame window style according to the current StayOnTop
		// state.
		if (m_bAlwaysOnTop)
		{
			SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN | wxSTAY_ON_TOP);
		}
		else
		{
			SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN);
		}

		// Refresh CMainFrame.
		Refresh();
	}

	void CMainFrame::ChangeRulerUnitOfMeasurement(ERulerUnits NewUnit)
	{
		// If requested NewUnit is already set, return from this method.
		if (NewUnit == m_eRulerUnits)
			return;

		// Set new ruler units.
		m_eRulerUnits = NewUnit;

		// Refresh CMainFrame.
		Refresh();
	}

	void CMainFrame::ChangeRulerLength(int NewLength)
	{
		// If requested NewLength is already set or requested NewLength is
		// smaller than m_iRulerMinimumLengthLimit, then return from this
		// method.
		if (NewLength == m_iRulerLength || NewLength < m_iRulerMinimumLengthLimit)
			return;

		// Set NewLength as current ruler's length.
		m_iRulerLength = NewLength;

		// Prepare new size for the ruler.
		wxSize NewSize;
		switch (m_eRulerPosition)
		{
		case ERulerPosition::rpLeft:
		case ERulerPosition::rpRight:
			NewSize = wxSize(60, m_iRulerLength);
			break;
		case ERulerPosition::rpTop:
		case ERulerPosition::rpBottom:
			NewSize = wxSize(m_iRulerLength, 60);
			break;
		}

		// Update size.
		SetSize(NewSize);

		// Refresh CMainFrame.
		Refresh();
	}

	bool CMainFrame::LoadSettingsFromDatabase(
		const wxString& dbPath, std::map<wxString, wxString>& Settings)
	{
		sqlite3* db = nullptr;
		sqlite3_stmt* stmt = nullptr;
		int rc;

		// Open connection with database.
		rc =
			sqlite3_open_v2(
				dbPath.mb_str(), &db,
				SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"Can not open database '%s': %s", dbPath, sqlite3_errmsg(db));

			if (db)
			{
				sqlite3_close(db);
			}

			return false;
		}

		// Create table 'Settings' if not exists.
		const char* CreateTableSQL = R"(
			CREATE TABLE IF NOT EXISTS Settings (
				Key TEXT PRIMARY KEY,
				Value TEXT NOT NULL
			);
		)";

		rc = sqlite3_exec(db, CreateTableSQL, nullptr, nullptr, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"Can not create table 'Settings': %s", sqlite3_errmsg(db));

			sqlite3_close(db);

			return false;
		}

		// Prepare SELECT query to retrieve all settings.
		const char* SelectSQL = "SELECT Key, Value FROM Settings;";
		rc = sqlite3_prepare_v2(db, SelectSQL, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"There was an error while SELECT query was prepared: %s",
				sqlite3_errmsg(db));

			sqlite3_close(db);

			return false;
		}

		// Execute query and load results into Settings map.
		while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
		{
			wxString Key =
				wxString::FromUTF8(
					reinterpret_cast<const char*>(
						sqlite3_column_text(stmt, 0)));
			wxString Value =
				wxString::FromUTF8(
					reinterpret_cast<const char*>(
						sqlite3_column_text(stmt, 1)));

			Settings[Key] = Value;

#ifdef _DEBUG
			wxLogDebug("Setting loaded: %s = %s", Key, Value);
#endif
		}

		if (rc != SQLITE_DONE)
		{
			wxLogError(
				"There was an error while SELECT query was executed: %s",
				sqlite3_errmsg(db));

			sqlite3_finalize(stmt);
			sqlite3_close(db);

			return false;
		}

		// Release resources.
		sqlite3_finalize(stmt);
		sqlite3_close(db);

#ifdef _DEBUG
		wxLogInfo("All database settings was loaded successful.");
#endif

		return true;
	}

	bool CMainFrame::SaveSettingsToDatabase(
		const wxString& dbPath, const std::map<wxString, wxString>& Settings)
	{
		// If there is no settings in Settings map, then display warning
		// message and return true.
		if (Settings.empty())
		{
#ifdef _DEBUG
			wxLogWarning("There is no settings to save in database.");
#endif

			return true;
		}

		sqlite3* db = nullptr;
		sqlite3_stmt* stmt = nullptr;
		int rc;

		// Open connection with SQLite database.
		rc =
			sqlite3_open_v2(
				dbPath.mb_str(), &db,
				SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"Can not open database '%s': %s", dbPath, sqlite3_errmsg(db));

			if (db)
			{
				sqlite3_close(db);
			}

			return false;
		}

		// Create table 'Settings' if not exists.
		const char* CreateTableSQL = R"(
			CREATE TABLE IF NOT EXISTS Settings (
				Key TEXT PRIMARY KEY,
				Value TEXT NOT NULL
			);
		)";

		rc = sqlite3_exec(db, CreateTableSQL, nullptr, nullptr, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"Can not create table 'Settings': %s", sqlite3_errmsg(db));

			sqlite3_close(db);

			return false;
		}

		// Prepare SQL query INSERT OR REPLACE.
		const char* InsertSQL =
			"INSERT OR REPLACE INTO Settings (Key, Value) VALUES (?, ?);";
		rc = sqlite3_prepare_v2(db, InsertSQL, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"There was an error while preparing SQL query INSERT: %s",
				sqlite3_errmsg(db));

			sqlite3_close(db);

			return false;
		}

		// Starting transaction.
		rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError(
				"There was an error while beginning transaction: %s",
				sqlite3_errmsg(db));

			sqlite3_finalize(stmt);
			sqlite3_close(db);

			return false;
		}

		// Inserting settings into database.
		for (const auto& [Key, Value] : Settings)
		{
			sqlite3_reset(stmt);
			sqlite3_clear_bindings(stmt);

			rc =
				sqlite3_bind_text(
					stmt, 1, Key.mb_str(), -1, SQLITE_TRANSIENT);
			if (rc != SQLITE_OK)
			{
				wxLogError(
					"There was an error while binding Key '%s': %s",
					Key, sqlite3_errmsg(db));

				continue;
			}

			rc =
				sqlite3_bind_text(
					stmt, 2, Value.mb_str(), -1, SQLITE_TRANSIENT);
			if (rc != SQLITE_OK)
			{
				wxLogError(
					"There was an error while binding Value for Key '%s': %s",
					Key, sqlite3_errmsg(db));

				continue;
			}

			rc = sqlite3_step(stmt);
			if (rc != SQLITE_DONE)
			{
				wxLogError(
					"There was an error while inserting setting '%s': %s",
					Key, sqlite3_errmsg(db));
			}
#ifdef _DEBUG
			else
			{
				wxLogDebug("Setting saved: %s = %s", Key, Value);
			}
#endif
		}

		// Ending transaction.
		rc = sqlite3_exec(db, "END TRANSACTION;", nullptr, nullptr, nullptr);
		if (rc != SQLITE_OK)
		{
			wxLogError("Can not end transaction: %s", sqlite3_errmsg(db));

			sqlite3_finalize(stmt);
			sqlite3_close(db);

			return false;
		}

		// Release resources.
		sqlite3_finalize(stmt);
		sqlite3_close(db);

#ifdef _DEBUG
		wxLogInfo("Settings saved into database successful.");
#endif

		return true;
	}

	void CMainFrame::LoadApplicationSettings()
	{
		// SQLite database file is stored in "$(CURRENT_DIRECTORY)/WinRuler.db".
		wxString dbPath = wxGetCwd() + "/WinRuler.db";
		std::map<wxString, wxString> Settings;

		// If LoadSettingsFromDatabase() failed, display error and return from
		// this method.
		if (!LoadSettingsFromDatabase(dbPath, Settings))
		{
			wxLogError(
				"There was an error while application settings was loaded "
				"from database!");

			return;
		}

		// Process loaded settings.
		for (const auto& [Key, Value] : Settings)
		{
			if (Key == "ruler_position")
			{
				m_eRulerPosition = static_cast<ERulerPosition>(wxAtoi(Value));
			}
			else if (Key == "ruler_units")
			{
				m_eRulerUnits = static_cast<ERulerUnits>(wxAtoi(Value));
			}
			else if (Key == "ruler_background_type")
			{
				m_eRulerBackgroundType =
					static_cast<ERulerBackgroundType>(wxAtoi(Value));
			}
			else if (Key == "ruler_scale_colour")
			{
				m_cRulerScaleColour = static_cast<wxColour>(Value);
			}
			else if (Key == "ruler_background_colour")
			{
				m_cRulerBackgroundColour = static_cast<wxColour>(Value);
			}
			else if (Key == "ruler_background_start_colour")
			{
				m_cRulerBackgroundStartColour = static_cast<wxColour>(Value);
			}
			else if (Key == "ruler_background_end_colour")
			{
				m_cRulerBackgroundEndColour = static_cast<wxColour>(Value);
			}
			else if (Key == "ruler_length")
			{
				m_iRulerLength = static_cast<int>(wxAtoi(Value));
			}
			else if (Key == "ruler_minimum_length_limit")
			{
				m_iRulerMinimumLengthLimit = static_cast<int>(wxAtoi(Value));
			}
			else if (Key == "ruler_always_on_top")
			{
				m_bAlwaysOnTop = (Value == wxString("true"));
			}
			else if (Key == "ruler_transparency")
			{
				m_bRulerTransparency = (Value == wxString("true"));
			}
			else if (Key == "ruler_transparency_value")
			{
				m_iRulerTransparencyValue = static_cast<int>(wxAtoi(Value));
			}
			else if (Key == "ruler_first_marker_colour")
			{
				m_cFirstMarkerColour = static_cast<wxColour>(Value);
			}
			else if (Key == "ruler_second_marker_colour")
			{
				m_cSecondMarkerColour = static_cast<wxColour>(Value);
			}
			else if (Key == "ruler_background_image_path")
			{
				m_sRulerBackgroundImagePath = static_cast<wxString>(Value);
			}
			else if (Key == "vertical_ppi")
			{
				g_vPixelPerInch[0].y = static_cast<int>(wxAtoi(Value));
			}
			else if (Key == "horizontal_ppi")
			{
				g_vPixelPerInch[0].x = static_cast<int>(wxAtoi(Value));
			}
			else if (Key == "window_position")
			{
				wxPoint WindowPosition;

				WindowPosition = ParsePosition(Value);
				SetPosition(WindowPosition);
			}
			else if (Key == "snap_to_edges")
			{
				m_bSnapToEdgesOfScreen = (Value == wxString("true"));
			}
			else if (Key == "snap_to_edges_distance")
			{
				m_iSnapToEdgesOfScreenDistance = static_cast<int>(wxAtoi(Value));
			}
			else if (Key == "snap_to_other_windows")
			{
				m_bSnapToOtherWindows = (Value == wxString("true"));
			}
			else if (Key == "snap_to_other_windows_distance")
			{
				m_iSnapToOtherWindowsDistance = static_cast<int>(wxAtoi(Value));
			}

#ifdef _DEBUG
			// Log that specified setting was applied.
			wxLogMessage("Setting %s = %s was applied.", Key, Value);
#endif
		}
	}

	void CMainFrame::SaveApplicationSettings()
	{
		// SQLite database file is stored in "$(CURRENT_DIRECTORY)/WinRuler.db".
		wxString dbPath = wxGetCwd() + "/WinRuler.db";
		std::map<wxString, wxString> Settings;

		// Add all application settings into our Settings map.
		Settings["ruler_position"] = wxString::Format("%d", m_eRulerPosition);

		Settings["ruler_units"] = wxString::Format("%d", m_eRulerUnits);

		Settings["ruler_background_type"] =
			wxString::Format("%d", m_eRulerBackgroundType);

		Settings["ruler_scale_colour"] =
			m_cRulerScaleColour.GetAsString(wxC2S_HTML_SYNTAX);
		Settings["ruler_background_colour"] =
			m_cRulerBackgroundColour.GetAsString(wxC2S_HTML_SYNTAX);
		Settings["ruler_background_start_colour"] =
			m_cRulerBackgroundStartColour.GetAsString(wxC2S_HTML_SYNTAX);
		Settings["ruler_background_end_colour"] =
			m_cRulerBackgroundEndColour.GetAsString(wxC2S_HTML_SYNTAX);

		Settings["ruler_length"] = wxString::Format("%d", m_iRulerLength);
		Settings["ruler_minimum_length_limit"] =
			wxString::Format("%d", m_iRulerMinimumLengthLimit);

		Settings["ruler_always_on_top"] =
			m_bAlwaysOnTop ? wxString("true") : wxString("false");

		Settings["ruler_transparency"] =
			m_bRulerTransparency ? wxString("true") : wxString("false");
		Settings["ruler_transparency_value"] =
			wxString::Format("%d", m_iRulerTransparencyValue);

		Settings["ruler_first_marker_colour"] =
			m_cFirstMarkerColour.GetAsString(wxC2S_HTML_SYNTAX);
		Settings["ruler_second_marker_colour"] =
			m_cSecondMarkerColour.GetAsString(wxC2S_HTML_SYNTAX);

		Settings["ruler_background_image_path"] = m_sRulerBackgroundImagePath;

		Settings["vertical_ppi"] =
			wxString::Format("%d", g_vPixelPerInch[0].GetY());
		Settings["horizontal_ppi"] =
			wxString::Format("%d", g_vPixelPerInch[0].GetX());

		wxPoint WindowPosition = GetPosition();
		Settings["window_position"] =
			wxString::Format("%d:%d", WindowPosition.x, WindowPosition.y);

		Settings["snap_to_edges"] =
			m_bSnapToEdgesOfScreen ? wxString("true") : wxString("false");
		Settings["snap_to_edges_distance"] =
			wxString::Format("%d", m_iSnapToEdgesOfScreenDistance);

		Settings["snap_to_other_windows"] =
			m_bSnapToOtherWindows ? wxString("true") : wxString("false");
		Settings["snap_to_other_windows_distance"] =
			wxString::Format("%d", m_iSnapToOtherWindowsDistance);

		// If SaveSettingsToDatabase() failed, display error and return from
		// this method.
		if (!SaveSettingsToDatabase(dbPath, Settings))
		{
			wxLogError("There was an error while saving application settings!");
			return;
		}

#ifdef _DEBUG
		// Log that application settings was saved successful.
		wxLogMessage("Application settings saved successful.");
#endif
	}

	void CMainFrame::SnapToEdges(wxPoint& Pos)
	{
		// Retrieve size of the screen.
		wxDisplay Display(wxDisplay::GetFromWindow(this));
		wxRect ScreenRect = Display.GetGeometry();

		// Retrieve size of the window.
		wxSize WindowSize = GetSize();

		// Perform window snapping to edges of the screen.
		if (abs(Pos.x - ScreenRect.GetX()) <= m_iSnapToEdgesOfScreenDistance)
		{ // Left edge of the screen.
			Pos.x = ScreenRect.GetX();
		}

		if (abs((Pos.x + WindowSize.GetX()) -
				(ScreenRect.GetX() + ScreenRect.GetWidth())) <= m_iSnapToEdgesOfScreenDistance)
		{ // Right edge of the screen.
			Pos.x =
				ScreenRect.GetX() + ScreenRect.GetWidth() - WindowSize.GetX();
		}

		if (abs(Pos.y - ScreenRect.GetY()) <= m_iSnapToEdgesOfScreenDistance)
		{ // Top edge of the screen.
			Pos.y = ScreenRect.GetY();
		}

		if (abs((Pos.y + WindowSize.GetY()) -
				(ScreenRect.GetY() + ScreenRect.GetHeight())) <= m_iSnapToEdgesOfScreenDistance)
		{ // Bottom edge of the screen.
			Pos.y =
				ScreenRect.GetY() + ScreenRect.GetHeight() - WindowSize.GetY();
		}

		// Move our window to calculated position.
		SetPosition(Pos);
	}

	void CMainFrame::SnapToOtherWindows()
	{
		// Get CMainFrame window handle.
		HWND RulerHwnd = reinterpret_cast<HWND>(this->GetHandle());

		// Get CMainFrame window coordinates.
		RECT RulerRect;
		GetWindowRect(RulerHwnd, &RulerRect);

		// Retrieve informations about all windows.
		auto windows = GetAllWindows();

		// Itterate throu all windows.
		for (const auto& win : windows)
		{
			// Skip CMainFrame window.
			if (win.hwnd == RulerHwnd)
				continue;

			// Snap to horizontal edges of itterated window.
			if (abs(RulerRect.right - win.Rect.left) <= m_iSnapToOtherWindowsDistance)
			{
				RulerRect.left =
					win.Rect.left - (RulerRect.right - RulerRect.left);
				RulerRect.right = win.Rect.left;
			}
			else if (abs(RulerRect.left - win.Rect.right) <= m_iSnapToOtherWindowsDistance)
			{
				RulerRect.right =
					win.Rect.right + (RulerRect.right - RulerRect.left);
				RulerRect.left = win.Rect.right;
			}

			// Snap to vertical edges of itterated window.
			if (abs(RulerRect.bottom - win.Rect.top) <= m_iSnapToOtherWindowsDistance)
			{
				RulerRect.top =
					win.Rect.top - (RulerRect.bottom - RulerRect.top);
				RulerRect.bottom = win.Rect.top;
			}
			else if (abs(RulerRect.top - win.Rect.bottom) <= m_iSnapToOtherWindowsDistance)
			{
				RulerRect.bottom =
					win.Rect.bottom + (RulerRect.bottom - RulerRect.top);
				RulerRect.top = win.Rect.bottom;
			}
		}

		// Apply calculated coordinates to CMainFrame window.
		MoveWindow(
			RulerHwnd,
			RulerRect.left, RulerRect.top,
			RulerRect.right - RulerRect.left, RulerRect.bottom - RulerRect.top,
			TRUE);
	}

	void CMainFrame::OnMouseEvent(wxMouseEvent& Event)
	{
		wxPoint Pos = Event.GetPosition();
		wxWindow* pWindow = wxDynamicCast(Event.GetEventObject(), wxWindow);
		Pos = pWindow->ClientToScreen(Pos);
		if (Event.LeftDown())
		{
			m_eBorderDragMode = BorderHitTest(Pos);

			if (m_eBorderDragMode != HT_client)	 // Start the drag now.
			{
				// Capture mouse and set the cursor.
				CaptureMouse();

				SetResizeCursor(m_eBorderDragMode);
				m_ptDragStart = Pos;
				m_rectBorder = GetRect();

				return;
			}
		}
		else if (Event.LeftUp() && m_eBorderDragMode != HT_client)
		{
			// Reset the drag mode.
			m_eBorderDragMode = HT_client;

			// Release mouse and unset the cursor.
			ReleaseMouse();
			SetCursor(*wxSTANDARD_CURSOR);
		}  // Left up && dragging.
		else if ((Event.Moving() || Event.Leaving() || Event.Entering()) &&
				 (m_eBorderDragMode == HT_client))
		{
			int hitPos = BorderHitTest(Pos);
			if (Event.Leaving() || m_eBorderDragMode == HT_client)
				OnLeaveBorder(hitPos);
			else
				OnEnterBorder();
		}
		else if (Event.Dragging() && (m_eBorderDragMode != HT_client))
		{
			ResizeSize(Pos);
		}
		else
		{
			Event.Skip();
		}
	}

	int CMainFrame::BorderHitTest(const wxPoint& Pos)
	{
		// if CMainFrame is maximized frame, it can't be resized.
		if (IsMaximized())
		{
			return HT_client;
		}

		// Retrieve frame's rect.
		wxRect Rect = GetRect();

		// Calculate x and y values.
		int x =
			Clamp(
				Pos.x,
				Rect.GetX() + m_iOffsetBorder,
				Rect.GetX() + Rect.GetWidth() - m_iOffsetBorder);
		int y =
			Clamp(
				Pos.y,
				Rect.GetY() + m_iOffsetBorder,
				Rect.GetY() + Rect.GetHeight() - m_iOffsetBorder);

		// Create default HotArea array.
		static int HotArea[3][3] =
		{
			{ HT_topLeft, HT_top, HT_topRight },
			{ HT_left, HT_client, HT_right },
			{ HT_bottomLeft, HT_bottom, HT_bottomRight }
		};

		// Modify HotArea array to take m_eRulerPosition into account.
		switch (m_eRulerPosition)
		{
		case rpLeft:
		case rpRight:
			HotArea[0][0] = HT_client;
			HotArea[0][1] = HT_top;
			HotArea[0][2] = HT_client;
			HotArea[1][0] = HT_client;
			HotArea[1][1] = HT_client;
			HotArea[1][2] = HT_client;
			HotArea[2][0] = HT_client;
			HotArea[2][1] = HT_bottom;
			HotArea[2][2] = HT_client;

			break;
		case rpTop:
		case rpBottom:
			HotArea[0][0] = HT_client;
			HotArea[0][1] = HT_client;
			HotArea[0][2] = HT_client;
			HotArea[1][0] = HT_left;
			HotArea[1][1] = HT_client;
			HotArea[1][2] = HT_right;
			HotArea[2][0] = HT_client;
			HotArea[2][1] = HT_client;
			HotArea[2][2] = HT_client;

			break;
		}

		// Store calculated x and y in m_ptDirection.
		m_ptDirection.x = x - 1;
		m_ptDirection.y = y - 1;

		// Return proper value stored in HotArea array.
		return HotArea[y][x];
	}

	void CMainFrame::OnMove(wxMoveEvent& Event)
	{
		// Retrieve new window position.
		wxPoint Pos = Event.GetPosition();

		// Perform window snapping in needed.
		if (m_bSnapToEdgesOfScreen)
			SnapToEdges(Pos);
		if (m_bSnapToOtherWindows)
			SnapToOtherWindows();

		// Continue event handling.
		Event.Skip();
	}

	void CMainFrame::BorderDragInit()
	{
		// Initialize border dragging values.
		m_eBorderDragMode = HT_client;
		m_iOffsetBorder = 10;
	}

	void CMainFrame::OnLeaveBorder(int HitPos)
	{
		// Set Resize cursor.
		SetResizeCursor(HitPos);
	}

	void CMainFrame::OnEnterBorder()
	{
		// Set mouse cursor to standard cursor.
		SetCursor(*wxSTANDARD_CURSOR);
	}

	void CMainFrame::SetResizeCursor(int HitPos)
	{
		// Depending on m_eRulerPosition and htPos values, set proper mouse
		// cursor.
		switch (m_eRulerPosition)
		{
		case ERulerPosition::rpLeft:
		case ERulerPosition::rpRight:
			switch (HitPos)
			{
			case HT_Pos::HT_top:
			case HT_Pos::HT_bottom:
				SetCursor(wxCURSOR_SIZENS);
				break;
			default:
				SetCursor(*wxSTANDARD_CURSOR);
			}

			break;
		case ERulerPosition::rpTop:
		case ERulerPosition::rpBottom:
			switch (HitPos)
			{
			case HT_Pos::HT_left:
			case HT_Pos::HT_right:
				SetCursor(wxCURSOR_SIZEWE);
				break;
			default:
				SetCursor(*wxSTANDARD_CURSOR);
			}

			break;
        default:
            SetCursor(*wxSTANDARD_CURSOR);
		}
	}

	void CMainFrame::ResizeSize(const wxPoint& Pos)
	{
		// Calculate new position and new size and store it in Rect.
		wxPoint Offset = Pos - m_ptDragStart;
		wxRect Rect;

		// Depending on current m_eRulerPosition, calculate Rect.Width and
		// Rect.Height taking m_iRulerMinimumLengthLimit in mind.
		Rect.SetWidth(Offset.x * m_ptDirection.x + m_rectBorder.GetWidth());
		Rect.SetHeight(Offset.y * m_ptDirection.y + m_rectBorder.GetHeight());

		switch (m_eRulerPosition)
		{
		case ERulerPosition::rpLeft:
		case ERulerPosition::rpRight:
			Rect.SetHeight(
				std::max<int>(m_iRulerMinimumLengthLimit, Rect.GetHeight()));
			break;
		case ERulerPosition::rpTop:
		case ERulerPosition::rpBottom:
			Rect.SetWidth(
				std::max<int>(m_iRulerMinimumLengthLimit, Rect.GetWidth()));
			break;
		}

		Rect.SetX(
			(Offset.x * (m_ptDirection.x == -1 ? 1 : 0)) + m_rectBorder.x);
		Rect.SetY(
			(Offset.y * (m_ptDirection.y == -1 ? 1 : 0)) + m_rectBorder.y);

		// Set new size.
		SetSize(Rect);

		// Update CMainFrame instance.
		Update();

		// Depending on current m_eRulerPosition, update m_iRulerLength value.
		switch (m_eRulerPosition)
		{
		case ERulerPosition::rpLeft:
		case ERulerPosition::rpRight:
			m_iRulerLength = Rect.GetHeight();

			break;
		case ERulerPosition::rpTop:
		case ERulerPosition::rpBottom:
			m_iRulerLength = Rect.GetWidth();

			break;
		}

		// Refresh m_pDrawPanel.
		m_pDrawPanel->Refresh();
	}
} // end namespace WinRuler
