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

BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
EVT_MOUSE_EVENTS(CMainFrame::OnMouseEvent)
END_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title) :
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxCLIP_CHILDREN)
{
	wxBoxSizer* Sizer = new wxBoxSizer(wxHORIZONTAL);
	m_DrawPanel = new CDrawPanel((wxFrame*) this);
	Sizer->Add(m_DrawPanel, 1, wxEXPAND);

	this->SetSizer(Sizer);
	this->SetAutoLayout(true);

	// Initialize Border Drag.
	BorderDragInit();
}

void CMainFrame::OnMouseEvent(wxMouseEvent& event)
{
	auto pos = event.GetPosition();
	wxWindow* window = wxDynamicCast(event.GetEventObject(), wxWindow);
	pos = window->ClientToScreen(pos);
	if (event.LeftDown())
	{
		m_eBorderDragMode = BorderHitTest(pos);

		if (m_eBorderDragMode != HT_client)	 // Start the drag now.
		{
			// Capture mouse and set the cursor.
			CaptureMouse();

			SetResizeCursor(m_eBorderDragMode);
			m_ptDragStart = pos;
			m_rectBorder = GetRect();

			return;
		}
	}
	else if (event.LeftUp() && m_eBorderDragMode != HT_client)
	{
		// Reset the drag mode.
		m_eBorderDragMode = HT_client;

		// Release mouse and unset the cursor.
		ReleaseMouse();
		SetCursor(*wxSTANDARD_CURSOR);
	}  // Left up && dragging.
	else if ((event.Moving() || event.Leaving() || event.Entering()) && (m_eBorderDragMode == HT_client))
	{
		int hitPos = BorderHitTest(pos);
		if (event.Leaving() || m_eBorderDragMode == HT_client)
			OnLeaveBorder(hitPos);
		else
			OnEnterBorder();
	}
	else if (event.Dragging() && (m_eBorderDragMode != HT_client))
	{
		ResizeSize(pos);
	}
	else
	{
		event.Skip();
	}
}

int CMainFrame::BorderHitTest(const wxPoint& pos)
{
	if (IsMaximized())	// Maximized frame can't be resized.
	{
		return HT_client;
	}

	wxRect rect = GetRect();

	int x = Clamp(pos.x, rect.x + m_nOffsetBorder, rect.x + rect.width - m_nOffsetBorder);
	int y = Clamp(pos.y, rect.y + m_nOffsetBorder, rect.y + rect.height - m_nOffsetBorder);

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
	//m_rectBorder = GetRect();
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
}
