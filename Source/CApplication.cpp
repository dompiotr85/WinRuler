/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "CApplication.h"
#include "WRUtilities.h"
#include <wx/wx.h>
#include <wx/display.h>

namespace WinRuler
{
	bool CApplication::OnInit()
	{
		// Execute inherited method OnInit().
		wxApp::OnInit();

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

		// Create dynamically (on heap) new CMainFrame class and store it in
		// mainFrame.
		m_pMainFrame = new CMainFrame("WinRuler");

		// Set MainFrame client size depending on ruler scale position and
		// centre it on the screen.
		switch (m_pMainFrame->m_eRulerPosition)
		{
		case CMainFrame::rpLeft:
		case CMainFrame::rpRight:
			m_pMainFrame->SetClientSize(wxSize(60, m_pMainFrame->m_iRulerLength));

			break;
		case CMainFrame::rpTop:
		case CMainFrame::rpBottom:
			m_pMainFrame->SetClientSize(wxSize(m_pMainFrame->m_iRulerLength, 60));

			break;
		}
		m_pMainFrame->Centre();

		// Load application icon and set it on m_pMainFrame.
		m_pIcon = new wxIcon();
		if (!m_pIcon->LoadFile(
				wxString("../../../../Resources/WinRuler.ico"),
				wxBITMAP_TYPE_ICO))
		{
			wxLogError("Can't load application icon!");

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
		// Clean our wxLog instance.
		wxLog::SetActiveTarget(nullptr);
		wxDELETE(m_pLogger);

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
		if (Type == wxEVT_MOTION || Type == wxEVT_LEFT_DOWN || Type == wxEVT_LEFT_UP)
		{
			// ... have only go up once; then this subroutine will be called again
			//Event.ResumePropagation(100);
			Event.ResumePropagation(1);

			// Don't return Event_Ignore or Event_Processed, have to let it to
			// propagate up instead.
			//return Event_Processed;
		}

		// Return Event_Skip.
		return Event_Skip;
	}
} // end namespace WinRuler
