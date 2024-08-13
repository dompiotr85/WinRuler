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

bool CApplication::OnInit()
{
	wxApp::OnInit();

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

	// Set mainFrame client size and center it on screen.
	m_pMainFrame->SetClientSize(800, 60);
	m_pMainFrame->Center();

	// Show mainFrame.
	m_pMainFrame->Show();

	// Return true.
	return true;
}

int CApplication::OnExit()
{
	g_vPixelPerInch.clear();
	//if (!m_pMainFrame)
	//	delete m_pMainFrame;

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
