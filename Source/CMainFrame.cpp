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
	EVT_MOUSE_EVENTS(CMainFrame::OnMouseEvent)

	// Catch menu events.
	//EVT_MENU(ID_, CMainFrame::OnClick)
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

		// Bind OnExit method for command button clicked event.
		this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CMainFrame::OnExit, this, wxID_EXIT);

		// Bind OnClose method for close window event.
		this->Bind(wxEVT_CLOSE_WINDOW, &CMainFrame::OnClose, this);

		// Initialize Border Dragging.
		BorderDragInit();
	}

	CMainFrame::~CMainFrame()
	{
		// Release m_pAboutDialog.
		wxDELETE(m_pAboutDialog);

		// Release m_pNewRulerLengthDialog.
		wxDELETE(m_pNewRulerLengthDialog);

		// Release m_pSizer.
		//wxDELETE(m_pSizer);
		
		// Release m_pDrawPanel.
		wxDELETE(m_pDrawPanel);

		// Unbind command button clicked event.
		this->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &CMainFrame::OnExit, this, wxID_EXIT);
	}

	void CMainFrame::Init()
	{
		// Ruler's scale position.
		m_eRulerPosition = rpTop;

		// Ruler's unit of measurement.
		m_eRulerUnits = ruCentimetres;

		// Ruler's background type.
		m_eRulerBackgroundType = btGradient;

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
	}

	void CMainFrame::CreateControls()
	{
		// Create new wxBoxSizer and store it in m_pSizer.
		m_pSizer = new wxBoxSizer(wxHORIZONTAL);

		// Create new CDrawPanel and store it in m_pDrawPanel. 
		m_pDrawPanel = new CDrawPanel((wxFrame*)this);

		// Add m_pDrawPanel to our m_pSizer.
		m_pSizer->Add(m_pDrawPanel, 1, wxEXPAND);

		// Set our m_pSizer in CMainFrame.
		this->SetSizer(m_pSizer);

		// Set CMainFrame auto layout.
		this->SetAutoLayout(true);
	}

	void CMainFrame::OnOptionsClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// todo: Implement WinRuler's options dialog window.
	}

	void CMainFrame::OnNewRulerLengthClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Create new CNewRulerLengthDialog instance.
		m_pNewRulerLengthDialog = new CNewRulerLengthDialog(this);

		// Call CNewRulerLengthDialog::ShowModal() method.
		m_pNewRulerLengthDialog->ShowModal();

		// Release created CNewRulerLengthDialog instance.
		wxDELETE(m_pNewRulerLengthDialog);
	}

	void CMainFrame::OnPixelsAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's unit of measurement to pixels.
		ChangeRulerUnitOfMeasurement(ruPixels);
	}

	void CMainFrame::OnCentimetresAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's unit of measurement to centimetres.
		ChangeRulerUnitOfMeasurement(ruCentimetres);
	}

	void CMainFrame::OnInchesAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's unit of measurement to inches.
		ChangeRulerUnitOfMeasurement(ruInches);
	}

	void CMainFrame::OnPicasAsUnitClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's unit of measurement to picas.
		ChangeRulerUnitOfMeasurement(ruPicas);
	}

	void CMainFrame::OnAlwaysOnTopClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's StayOnTop state to negative one.
		StayOnTop(!m_bAlwaysOnTop);
	}

	void CMainFrame::OnScaleOnLeftClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's position to left.
		ChangeRulerPosition(rpLeft);
	}

	void CMainFrame::OnScaleOnTopClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's position to top.
		ChangeRulerPosition(rpTop);
	}

	void CMainFrame::OnScaleOnRightClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's position to right.
		ChangeRulerPosition(rpRight);
	}

	void CMainFrame::OnScaleOnBottomClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Change ruler's position to bottom.
		ChangeRulerPosition(rpBottom);
	}

	void CMainFrame::OnAboutClicked(wxCommandEvent& WXUNUSED(Event))
	{
		// Create new CAboutDialog instance.
		m_pAboutDialog = new CAboutDialog(this);
		
		// Call CAboutDialog::ShowModal() method.
		m_pAboutDialog->ShowModal();

		// Release created CAboutDialog instance.
		wxDELETE(m_pAboutDialog);
	}

	void CMainFrame::OnCloseClicked(wxCommandEvent& Event)
	{
		// Call OnExit evnet.
		OnExit(Event);
	}

	void CMainFrame::OnExit(wxCommandEvent& Event)
	{
		// If user decided that WinRuler should be closed, then ...
		if (wxMessageBox(
			"Are you sure that you want to exit the WinRuler?",
			"WinRuler - Please confirm", wxICON_QUESTION | wxYES_NO) == wxYES)
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
				"WinRuler - Plase confirm", wxICON_QUESTION | wxYES_NO) == wxYES)
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
		// If current ruler's position is different than proposed ruler's
		// position, then ...
		if (m_eRulerPosition != NewPosition)
		{
			// ... set proposed ruler's position as current ruler's
			// position and ...
			m_eRulerPosition = NewPosition;

			// ... set new size of CMainFrame.
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

			// At end refresh CMainFrame.
			Refresh();
		}
	}

	void CMainFrame::StayOnTop(bool State)
	{
		// If current StayOnTop state is different than proposed state, then...
		if (m_bAlwaysOnTop != State)
		{
			// .. update current StayOnTop state and ...
			m_bAlwaysOnTop = State;

			// ... set CMainFrame window style according to current StayOnTop
			// state.
			if (m_bAlwaysOnTop)
			{
				SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN | wxSTAY_ON_TOP);
			}
			else
			{
				SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN);
			}

			Refresh();
		}
	}

	void CMainFrame::ChangeRulerUnitOfMeasurement(ERulerUnits NewUnit)
	{
		// If NewUnit is different than current unit of measurement, then
		// change it and refresh ruler.
		if (m_eRulerUnits != NewUnit)
		{
			m_eRulerUnits = NewUnit;

			Refresh();
		}
	}

	void CMainFrame::ChangeRulerLength(int NewLength)
	{
		// If NewLength is different than current ruler's length, then ...
		if (m_iRulerLength != NewLength && NewLength >= m_iRulerMinimumLengthLimit)
		{
			// ... set NewLength as current ruler's length.
			m_iRulerLength = NewLength;

			// Prepare new size for the ruler.
			wxSize NewSize;
			switch (m_eRulerPosition)
			{
			case rpLeft:
			case rpRight:
				NewSize = wxSize(60, NewLength);
				
				break;
			case rpTop:
			case rpBottom:
				NewSize = wxSize(NewLength, 60);

				break;
			}

			// Update size.
			SetSize(NewSize);

			Refresh();
		}
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
		// if CMainFrame is maximized frame, it can't be resized.
		if (IsMaximized())
		{
			return HT_client;
		}

		// Retrieve frame's rect.
		wxRect Rect = GetRect();

		// Calculate x and y values.
		int x = Clamp(Pos.x, Rect.GetX() + m_nOffsetBorder, Rect.GetX() + Rect.GetWidth() - m_nOffsetBorder);
		int y = Clamp(Pos.y, Rect.GetY() + m_nOffsetBorder, Rect.GetY() + Rect.GetHeight() - m_nOffsetBorder);

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

	void CMainFrame::BorderDragInit()
	{
		// Initialize border dragging values.
		m_eBorderDragMode = HT_client;
		m_nOffsetBorder = 10;
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
		case rpLeft:
		case rpRight:
			switch (HitPos)
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
			switch (HitPos)
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
		case rpLeft:
		case rpRight:
			Rect.SetHeight(
				std::max<int>(m_iRulerMinimumLengthLimit, Rect.GetHeight()));
			break;
		case rpTop:
		case rpBottom:
			Rect.SetWidth(
				std::max<int>(m_iRulerMinimumLengthLimit, Rect.GetWidth()));
			break;
		}
		
		Rect.SetX(Offset.x * (m_ptDirection.x == -1 ? 1 : 0) + m_rectBorder.x);
		Rect.SetY(Offset.y * (m_ptDirection.y == -1 ? 1 : 0) + m_rectBorder.y);

		// Set new size.
		SetSize(Rect);

		// Update CMainFrame instance.
		Update();

		// Depending on current m_eRulerPosition, update m_iRulerLength value.
		switch (m_eRulerPosition)
		{
		case rpLeft:
		case rpRight:
			m_iRulerLength = Rect.GetHeight();

			break;
		case rpTop:
		case rpBottom:
			m_iRulerLength = Rect.GetWidth();

			break;
		}

		// Refresh m_pDrawPanel.
		m_pDrawPanel->Refresh();
	}
} // end namespace WinRuler
