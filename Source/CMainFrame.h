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
#include "CDrawPanel.h"

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

/**
 * MainFrame class definition. 
 **/
class CMainFrame :
	public wxFrame
{
public:
	/**
	 * Ruler's position. It describes position of ruler's scale.
	 **/
	typedef enum ERulerPosition
	{
		// Ruler's scale is on left side of the ruler.
		rpLeft,
		// Ruler's scale is on top side of the ruler.
		rpTop,
		// Ruler's scale is on right side of the ruler.
		rpRight,
		// Ruler's scale is on bottom side of the ruler.
		rpBottom
	} ERulerPosition;

	/**
	 * Ruler's units of measurement types.
	 **/
	typedef enum ERulerUnits
	{
		// Centimeters as unit of measurement.
		ruCentimetres,
		// Unches as unit of measurement.
		ruInches,
		// Picas as unit of measurement.
		ruPicas,
		// Pixels as unit of measurement.
		ruPixels
	} ERulerUnits;

	/**
	 * Ruler's background type.
	 **/
	typedef enum ERulerBackgroundType
	{
		// Solid color as ruler's background.
		btSolid,
		// Gradient color as ruler's background.
		btGradient,
		// Image as ruler's background.
		btImage
	} ERulerBackgroundType;
public:
	/**
	 * CMainFrame constructor.
	 *
	 * @param Title		Reference to MainFrame title string.
	 **/
	CMainFrame(const wxString& Title);
	~CMainFrame();

	void OnExit(wxCommandEvent& Event);
	void OnClose(wxCloseEvent& Event);

	void OnMouseEvent(wxMouseEvent& Event);

	void ChangeRulerPosition(ERulerPosition NewPosition);
	void StayOnTop(bool State);
	void ChangeRulerUnitOfMeasurement(ERulerUnits NewUnit);
protected:
	void BorderDragInit();
	int BorderHitTest(const wxPoint& Pos);
	void SetResizeCursor(int htPos);
	void OnLeaveBorder(int hitPos);
	void OnEnterBorder();
	void ResizeSize(const wxPoint& Pos);
public:
	// Ruler's scale position.
	ERulerPosition m_eRulerPosition = rpTop;
	ERulerUnits m_eRulerUnits = ruCentimetres;
	ERulerBackgroundType m_eRulerBackgroundType = btSolid;

	wxColor m_RulerScaleColour = wxColour(0, 0, 0);
	wxColor m_RulerBackgroundColor = wxColour(255, 164, 119);

	int m_iRulerLength = 800;

	bool m_bAlwaysOnTop = true;

	// DrawPanel which is used for all drawing of our ruler.
	CDrawPanel* m_pDrawPanel;

	// Box sizer.
	wxBoxSizer* m_pSizer;
public:
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

	// Direction vector.
	wxPoint m_ptDirection;

	DECLARE_EVENT_TABLE()
};
