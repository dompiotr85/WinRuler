/**
 * Copyright © 2024 Piotr Domañski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
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

		// Load application icon and set it on m_pMainFrame.
		m_pIcon = new wxIcon();
		if (!m_pIcon->LoadFile(wxString("../../../../Resources/WinRuler.ico"), wxBITMAP_TYPE_ICO))
		{
			wxMessageBox(
				wxString("Can't load application icon!"),
				wxString("WinRuler - Error"),
				wxOK | wxCENTRE | wxICON_ERROR, m_pMainFrame);

			return false;
		}
		m_pMainFrame->SetIcon(*m_pIcon);

		// Set mainFrame client size depending on Ruler scale position and
		// center it on screen.
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

		// Show mainFrame.
		m_pMainFrame->Show();

		// Return true.
		return true;
	}

	int CApplication::OnExit()
	{
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
