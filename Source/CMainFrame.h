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

#pragma once

#include <wx/wx.h>

enum HT_Pos
{
	HT_client,
	HT_left,
	HT_right,
	HT_top,
	HT_topLeft,
	HT_topRight,
	HT_bottom,
	HT_bottomLeft,
	HT_bottomRight
};

// Ruler's position. It describes position of ruler's scale.
enum TRulerPosition
{
	rpLeft,
	rpTop,
	rpRight,
	rpBottom
};

class CMainFrame :
	public wxFrame
{
public:
	CMainFrame(const wxString& Title);

	void OnMouseEvent(wxMouseEvent& ev);
protected:
	void BorderDragInit();
	bool CaptionHitTest(const wxPoint& pos);
	int BorderHitTest(const wxPoint& pos);
	void SetResizeCursor(int htPos);
	void OnLeaveBorder(int hitPos);
	void OnEnterBorder();
	void ResizeSize(const wxPoint& pos);
private:
	int m_eRulerPosition;

	enum
	{
		ID_PANEL_CAPTION,
		ID_BTN_MINIMIZE,
		ID_BTN_MAXIMIZE,
		ID_BTN_CLOSE
	};

	wxPanel* m_panelCaption;

	// This filed records if caption drag opertion. 
	bool m_bCaptionDragStart;

	// Mouse position when dragging started.
	wxPoint m_ptDragStart;

	// Default width of the border.
	int	m_nOffsetBorder;

	// Record the hit position when resizing the frame.
	int m_eBorderDragMode;

	// This filed record frame rect when drag beginning. 
	wxRect m_rectBorder;
	wxCursor m_borderCursorVertical;
	wxCursor m_borderCursorHorizontal;
	wxCursor m_borderCursorOblique;

	// Direction vector.
	wxPoint m_ptDirection;

	DECLARE_EVENT_TABLE()
};
