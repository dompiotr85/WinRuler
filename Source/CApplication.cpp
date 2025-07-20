/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include <wx/wx.h>
#include <wx/display.h>

#include "CApplication.h"
#include "WRUtilities.h"

namespace WinRuler
{
	bool CApplication::OnInit()
	{
		// Set frame appearance to use system theme.
		SetAppearance(wxApp::Appearance::System);

		// Only in Debug, create our wxLog instance and set it as active
		// target.
#ifdef _DEBUG
		m_pLogger = new wxLogWindow(nullptr, "Log", true, false);
		wxLog::SetActiveTarget(m_pLogger);
#endif

		// Initialize all supported image handlers.
		::wxInitAllImageHandlers();

		// Retrieve PPI information for all screen detected.
		g_vPixelPerInch.clear();
		for (unsigned int i = 0; i < wxDisplay::GetCount(); ++i)
		{
			const wxDisplay display(i);

			g_vPixelPerInch.emplace_back();
			g_vPixelPerInch.back() = display.GetPPI();
		}

		// If our application is executed for the first time, store bool
		// true here, otherwise store false. We will need that information
		// bellow.
		bool bAppExecForTheFirstTime = ApplicationExecutedForTheFirstTime();

		// Create dynamically (on heap) new CMainFrame class and store it in
		// mainFrame.
		m_pMainFrame = new CMainFrame("WinRuler");

		// Set MainFrame client size depending on ruler scale position and
		// centre it on the screen.
		switch (m_pMainFrame->m_eRulerPosition)
		{
		case ERulerPosition::rpLeft:
		case ERulerPosition::rpRight:
			m_pMainFrame->SetClientSize(60, m_pMainFrame->m_iRulerLength);

			break;
		case ERulerPosition::rpTop:
		case ERulerPosition::rpBottom:
			m_pMainFrame->SetClientSize(m_pMainFrame->m_iRulerLength, 60);

			break;
		}

		// Centre MainFrame if application is executed for the first time.
		if (bAppExecForTheFirstTime)
		{
			m_pMainFrame->Centre();
		}

		// Load application icon and set it on m_pMainFrame.
		m_pIcon = new wxIcon();
		if (!m_pIcon->LoadFile(
				wxString("../../../../Resources/WinRuler.ico"),
				wxBITMAP_TYPE_ICO))
		{ // In case of error, log error message and release m_pIcon instance.
			wxLogError("Can't load application icon!");

			wxDELETE(m_pIcon);

			return false;
		}
		m_pMainFrame->SetIcon(*m_pIcon);

		// Show mainFrame.
		m_pMainFrame->Show();

		// Return true.
		return true;
	}

	int CApplication::OnExit()
	{
#ifdef _DEBUG
		// Clean our wxLog instance.
		wxLog::SetActiveTarget(nullptr);
		if (m_pLogger != nullptr)
		{
			wxDELETE(m_pLogger);
		}
#endif

		// Release m_pIcon instance.
		if (m_pIcon != nullptr)
		{
			wxDELETE(m_pIcon);
		}

		// Release m_pMainFrame instance.
		/*if (m_pMainFrame != nullptr)
		{
			wxDELETE(m_pMainFrame);
		}*/

		// Clear h_vPixelPerInch vector.
		g_vPixelPerInch.clear();

		// Execute and return inherited method OnExit() value.
		return wxApp::OnExit();
	}

	int CApplication::FilterEvent(wxEvent& Event)
	{
		// Retrieve event type.
		auto Type = Event.GetEventType();

		// If retrieved event type is motion or left button down or left button
		// up, then ...
		if (Type == wxEVT_MOTION || 
			Type == wxEVT_LEFT_DOWN || 
			Type == wxEVT_LEFT_UP)
		{
			// ... have only go up once; then this subroutine will be called
			// again.
			//Event.ResumePropagation(100);
			Event.ResumePropagation(1);

			// Don't return Event_Ignore or Event_Processed, have to let it to
			// propagate up instead.
			//return Event_Processed;
		}

		// Return Event_Skip.
		return Event_Skip;
	}

	bool CApplication::ApplicationExecutedForTheFirstTime()
	{
		// Prepare path to our database file.
		wxString dbPath = wxGetCwd() + "/WinRuler.db";

		// If database file exists, it means that our application was executed
		// before and database file was already created. Return false.
		if (wxFileExists(dbPath))
			return false;

		// If we reach this point, it means that our application is executed
		// for the first time.
		return true;
	}
} // end namespace WinRuler
