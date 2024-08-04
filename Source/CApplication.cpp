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
#include "CMainFrame.h"
#include <wx/wx.h>

bool CApplication::OnInit()
{
	CMainFrame* mainFrame = new CMainFrame("WinRuler");

	mainFrame->SetClientSize(800, 80);
	mainFrame->Center();

	mainFrame->Show();

	return true;
}

int CApplication::FilterEvent(wxEvent& event)
{
	auto type = event.GetEventType();
	if (type == wxEVT_MOTION || type == wxEVT_LEFT_DOWN || type == wxEVT_LEFT_UP)
	{
		// Have only go up once; then this subroutine will be called again
		//event.ResumePropagation(100);
		event.ResumePropagation(1);
		// Don't return 0 or 1, have to let it to propagate up instead
		// return 1;
	}

	return -1;
}
