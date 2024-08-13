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

#include <wx/wx.h>
#include <wx/sizer.h>
#include "CMainFrame.h"

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
EVT_MOUSE_EVENTS(CMainFrame::OnMouseEvent)
END_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& Title) :
	wxFrame(
		nullptr, wxID_ANY, Title, wxDefaultPosition, wxDefaultSize,
		wxBORDER_NONE | wxCLIP_CHILDREN | wxSTAY_ON_TOP)
{
	m_pSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pDrawPanel = new CDrawPanel((wxFrame*) this);
	m_pSizer->Add(m_pDrawPanel, 1, wxEXPAND);

	this->SetSizer(m_pSizer);
	this->SetAutoLayout(true);

	this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CMainFrame::OnExit, this, wxID_EXIT);
	this->Bind(wxEVT_CLOSE_WINDOW, &CMainFrame::OnClose, this);

	// Initialize Border Drag.
	BorderDragInit();
}

CMainFrame::~CMainFrame()
{
	if (!m_pSizer)
		delete m_pSizer;
	if (!m_pDrawPanel)
		delete m_pDrawPanel;

	this->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &CMainFrame::OnExit, this, wxID_EXIT);
}

void CMainFrame::OnExit(wxCommandEvent& Event)
{
	if (wxMessageBox(
			"Are you sure that you want to exit the WinRuler?",
			"WinRuler - Please confirm", wxICON_QUESTION | wxYES_NO) == wxYES)
	{
		this->Close(true);
	}
}

void CMainFrame::OnClose(wxCloseEvent& Event)
{
	// Check, that ALT+F4 was pressed.
	if (wxGetKeyState(WXK_ALT) && wxGetKeyState(WXK_F4))
	{
		// Show message about closing.
		if (wxMessageBox(
			"Are you sure that you want to exit the WinRuler?",
			"WinRuler - Plase confirm", wxICON_QUESTION | wxYES_NO) == wxYES)
		{
			// Is YES pressed, close Frame.
			this->Close(true);
		}

		// Ignore closing of Frame.
		return;
	}

	// If ALT+F4 not pressed, skip this Event.
	Event.Skip();
}

void CMainFrame::ChangeRulerPosition(ERulerPosition NewPosition)
{
	if (m_eRulerPosition != NewPosition)
	{
		switch (NewPosition)
		{
		case rpLeft:
		case rpRight:
			SetSize(60, m_iRulerLength);

			break;
		case rpTop:
		case rpBottom:
			SetSize(m_iRulerLength, 60);

			break;
		}

		m_eRulerPosition = NewPosition;
		
		Refresh();
	}
}

void CMainFrame::StayOnTop(bool State)
{
	if (m_bAlwaysOnTop != State)
	{
		m_bAlwaysOnTop = State;

		if (m_bAlwaysOnTop)
		{
			this->SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN | wxSTAY_ON_TOP);
		}
		else
		{
			this->SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN);
		}
	}
}

void CMainFrame::ChangeRulerUnitOfMeasurement(ERulerUnits NewUnit)
{
	// If NewUnit is different than current unit of measurement, then change it
	// and refresh ruler.
	if (m_eRulerUnits != NewUnit)
	{
		m_eRulerUnits = NewUnit;
		
		Refresh();
	}
}

void CMainFrame::OnMouseEvent(wxMouseEvent& Event)
{
	auto Pos = Event.GetPosition();
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
	else if ((Event.Moving() || Event.Leaving() || Event.Entering()) && (m_eBorderDragMode == HT_client))
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
	if (IsMaximized())	// Maximized frame can't be resized.
	{
		return HT_client;
	}

	wxRect rect = GetRect();

	int x = Clamp(Pos.x, rect.x + m_nOffsetBorder, rect.x + rect.width - m_nOffsetBorder);
	int y = Clamp(Pos.y, rect.y + m_nOffsetBorder, rect.y + rect.height - m_nOffsetBorder);

	static int hotArea[3][3] =
	{
		{ HT_topLeft, HT_top, HT_topRight },
		{ HT_left, HT_client, HT_right },
		{ HT_bottomLeft, HT_bottom, HT_bottomRight }
	};

	switch (m_eRulerPosition)
	{
	case rpLeft:
	case rpRight:
		hotArea[0][0] = HT_client;
		hotArea[0][1] = HT_top;
		hotArea[0][2] = HT_client;
		hotArea[1][0] = HT_client;
		hotArea[1][1] = HT_client;
		hotArea[1][2] = HT_client;
		hotArea[2][0] = HT_client;
		hotArea[2][1] = HT_bottom;
		hotArea[2][2] = HT_client;
		break;
	case rpTop:
	case rpBottom:
		hotArea[0][0] = HT_client;
		hotArea[0][1] = HT_client;
		hotArea[0][2] = HT_client;
		hotArea[1][0] = HT_left;
		hotArea[1][1] = HT_client;
		hotArea[1][2] = HT_right;
		hotArea[2][0] = HT_client;
		hotArea[2][1] = HT_client;
		hotArea[2][2] = HT_client;
		break;
	}

	m_ptDirection.x = x - 1;
	m_ptDirection.y = y - 1;

	return hotArea[y][x];
}

void CMainFrame::BorderDragInit()
{
	m_eBorderDragMode = HT_client;
	m_nOffsetBorder = 10;
}

void CMainFrame::OnLeaveBorder(int hitPos)
{
	SetResizeCursor(hitPos);
}

void CMainFrame::OnEnterBorder()
{
	SetCursor(*wxSTANDARD_CURSOR);
}

void CMainFrame::SetResizeCursor(int htPos)
{
	switch (m_eRulerPosition)
	{
	case rpLeft:
	case rpRight:
		switch (htPos)
		{
		case HT_top:
		case HT_bottom:
			SetCursor(wxCURSOR_SIZENS);
			
			break;
		default:
			SetCursor(*wxSTANDARD_CURSOR);
		}
		
		break;
	case rpTop:
	case rpBottom:
		switch (htPos)
		{
		case HT_left:
		case HT_right:
			SetCursor(wxCURSOR_SIZEWE);
			
			break;
		default:
			SetCursor(*wxSTANDARD_CURSOR);
		}
		
		break;
	}
}

void CMainFrame::ResizeSize(const wxPoint& pos)
{
	auto offset = pos - m_ptDragStart;
	wxRect rect;

	rect.width = offset.x * m_ptDirection.x + m_rectBorder.width;
	rect.height = offset.y * m_ptDirection.y + m_rectBorder.height;
	rect.x = offset.x * (m_ptDirection.x == -1 ? 1 : 0) + m_rectBorder.x;
	rect.y = offset.y * (m_ptDirection.y == -1 ? 1 : 0) + m_rectBorder.y;

	SetSize(rect);
	Update();

	if (m_eRulerPosition == rpLeft || m_eRulerPosition == rpRight)
	{
		m_iRulerLength = rect.GetHeight();
	}
	else if (m_eRulerPosition == rpTop || m_eRulerPosition == rpBottom)
	{
		m_iRulerLength = rect.GetWidth();
	}

	m_pDrawPanel->Refresh();
}
