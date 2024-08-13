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

#include <cmath>
#include <wx/wx.h>
#include "CDrawPanel.h"
#include "CMainFrame.h"
#include "WRUtilities.h"

#define ID_RULER_POSITION 2001
#define ID_RULER_POSITION_SCALE_ON_LEFT 2002
#define ID_RULER_POSITION_SCALE_ON_RIGHT 2003
#define ID_RULER_POSITION_SCALE_ON_TOP 2004
#define ID_RULER_POSITION_SCALE_ON_BOTTOM 2005
#define ID_PIXELS_AS_UNIT 2006
#define ID_CENTIMETRES_AS_UNIT 2007
#define ID_INCHES_AS_UNIT 2008
#define ID_PICAS_AS_UNIT 2009
#define ID_ALWAYS_ON_TOP 2010
#define ID_NEW_RULER_LENGTH 2011
#define ID_OPTIONS 2012

BEGIN_EVENT_TABLE(CDrawPanel, wxPanel)

// Catch paint events.
EVT_PAINT(CDrawPanel::PaintEvent)

// Catch mouse events.
EVT_MOUSE_EVENTS(CDrawPanel::OnMouseEvent)
EVT_LEFT_DOWN(CDrawPanel::OnLeftDown)
EVT_LEFT_UP(CDrawPanel::OnLeftUp)
EVT_MOTION(CDrawPanel::OnMouseMove)
EVT_LEAVE_WINDOW(CDrawPanel::OnMouseLeave)

// Catch menu events.
//EVT_MENU(ID_, CDrawPanel::OnClick)
EVT_MENU(ID_OPTIONS, CDrawPanel::OnOptionsClick)
EVT_MENU(ID_NEW_RULER_LENGTH, CDrawPanel::OnNewRulerLengthClick)
EVT_MENU(ID_PIXELS_AS_UNIT, CDrawPanel::OnPixelsAsUnitClick)
EVT_MENU(ID_CENTIMETRES_AS_UNIT, CDrawPanel::OnCentimetresAsUnitClick)
EVT_MENU(ID_INCHES_AS_UNIT, CDrawPanel::OnInchesAsUnitClick)
EVT_MENU(ID_PICAS_AS_UNIT, CDrawPanel::OnPicasAsUnitClick)
EVT_MENU(ID_ALWAYS_ON_TOP, CDrawPanel::OnAlwaysOnTopClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_LEFT, CDrawPanel::OnScaleOnLeftClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_RIGHT, CDrawPanel::OnScaleOnRightClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_TOP, CDrawPanel::OnScaleOnTopClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_BOTTOM, CDrawPanel::OnScaleOnBottomClick)
EVT_MENU(wxID_ABOUT, CDrawPanel::OnAboutClick)
EVT_MENU(wxID_CLOSE, CDrawPanel::OnCloseClick)
END_EVENT_TABLE()

CDrawPanel::CDrawPanel(wxFrame* pParent) :
    wxPanel(pParent),
    m_bDragging(false)
{
    // No code.
}

void CDrawPanel::OnMouseEvent(wxMouseEvent& Event)
{
    auto Pos = Event.GetPosition();
    wxWindow* pWindow = wxDynamicCast(Event.GetEventObject(), wxWindow);
    Pos = pWindow->ClientToScreen(Pos);
    if (Event.RightDown())
    {
        // Create pMenu, pRulerPositionMenu.
        wxMenu* pMenu = new wxMenu();
        wxMenu* pRulerPositionMenu = new wxMenu();

        // Retrieve pointer to CMainFrame class.
        CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

        // Build pRulerPositionMenu items.
        pRulerPositionMenu->AppendRadioItem(
            ID_RULER_POSITION_SCALE_ON_LEFT,
            wxString("Scale on &left side"))->Check(
                pMainFrame->m_eRulerPosition == CMainFrame::rpLeft ? true : false);
        pRulerPositionMenu->AppendRadioItem(
            ID_RULER_POSITION_SCALE_ON_RIGHT,
            wxString("Scale on &right side"))->Check(
                pMainFrame->m_eRulerPosition == CMainFrame::rpRight ? true : false);
        pRulerPositionMenu->AppendRadioItem(
            ID_RULER_POSITION_SCALE_ON_TOP,
            wxString("Scale on &top"))->Check(
                pMainFrame->m_eRulerPosition == CMainFrame::rpTop ? true : false);
        pRulerPositionMenu->AppendRadioItem(
            ID_RULER_POSITION_SCALE_ON_BOTTOM,
            wxString("Scale on &bottom"))->Check(
                pMainFrame->m_eRulerPosition == CMainFrame::rpBottom ? true : false);

        // Append pRulerPositionMenu as submenu of ID_RULER_POSITION item.
        pMenu->Append(ID_RULER_POSITION, wxString("&Ruler position"), pRulerPositionMenu);
        pMenu->AppendSeparator();

        // Append measuring units as radio items.
        pMenu->AppendRadioItem(
            ID_PIXELS_AS_UNIT,
            wxString("&Pixels as unit"))->Check(
                pMainFrame->m_eRulerUnits == CMainFrame::ruPixels ? true : false);
        pMenu->AppendRadioItem(
            ID_CENTIMETRES_AS_UNIT,
            wxString("&Centimetres as unit"))->Check(
                pMainFrame->m_eRulerUnits == CMainFrame::ruCentimetres ? true : false);
        pMenu->AppendRadioItem(
            ID_INCHES_AS_UNIT,
            wxString("&Inches as unit"))->Check(
                pMainFrame->m_eRulerUnits == CMainFrame::ruInches ? true : false);
        pMenu->AppendRadioItem(
            ID_PICAS_AS_UNIT,
            wxString("&Picas as unit"))->Check(
                pMainFrame->m_eRulerUnits == CMainFrame::ruPicas ? true : false);
        pMenu->AppendSeparator();

        // Append AlwaysOnTop item separated.
        wxMenuItem* pMenuItem = pMenu->AppendCheckItem(ID_ALWAYS_ON_TOP, wxString("&Always on top"));
        pMenuItem->Check(pMainFrame->m_bAlwaysOnTop);
        pMenu->AppendSeparator();

        // Append new ruler length MenuItem.
        pMenu->Append(ID_NEW_RULER_LENGTH, wxString("Set new ruler &length"));
        pMenu->AppendSeparator();

        // Append option menu item.
        pMenu->Append(ID_OPTIONS, wxString("&Options..."));
        pMenu->AppendSeparator();

        // Append About and Close items separated.
        pMenu->Append(wxID_ABOUT, wxString("&About"));
        pMenu->AppendSeparator();
        pMenu->Append(wxID_CLOSE, wxString("&Close\tAlt+F4"));

        // Pop penu.
        PopupMenu(pMenu, Event.GetPosition());

        // Release pMenu.
        delete pMenu;
    }
    else
    {
        Event.Skip();
    }
}

void CDrawPanel::OnOptionsClick(wxCommandEvent& WXUNUSED(Event))
{

}

void CDrawPanel::OnNewRulerLengthClick(wxCommandEvent& WXUNUSED(Event))
{

}

void CDrawPanel::OnPixelsAsUnitClick(wxCommandEvent& WXUNUSED(Event))
{
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

    pMainFrame->ChangeRulerUnitOfMeasurement(CMainFrame::ruPixels);
}

void CDrawPanel::OnCentimetresAsUnitClick(wxCommandEvent& WXUNUSED(Event))
{
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

    pMainFrame->ChangeRulerUnitOfMeasurement(CMainFrame::ruCentimetres);
}

void CDrawPanel::OnInchesAsUnitClick(wxCommandEvent& WXUNUSED(Event))
{
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

    pMainFrame->ChangeRulerUnitOfMeasurement(CMainFrame::ruInches);
}

void CDrawPanel::OnPicasAsUnitClick(wxCommandEvent& WXUNUSED(Event))
{
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

    pMainFrame->ChangeRulerUnitOfMeasurement(CMainFrame::ruPicas);
}

void CDrawPanel::OnAlwaysOnTopClick(wxCommandEvent& WXUNUSED(Event))
{
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

    pMainFrame->StayOnTop(!pMainFrame->m_bAlwaysOnTop);
}

void CDrawPanel::OnScaleOnLeftClick(wxCommandEvent& WXUNUSED(Event))
{
    ((CMainFrame*) this->GetParent())->ChangeRulerPosition(CMainFrame::rpLeft);
}

void CDrawPanel::OnScaleOnTopClick(wxCommandEvent& WXUNUSED(Event))
{
    ((CMainFrame*) this->GetParent())->ChangeRulerPosition(CMainFrame::rpTop);
}

void CDrawPanel::OnScaleOnRightClick(wxCommandEvent& WXUNUSED(Event))
{
    ((CMainFrame*) this->GetParent())->ChangeRulerPosition(CMainFrame::rpRight);
}

void CDrawPanel::OnScaleOnBottomClick(wxCommandEvent& WXUNUSED(Event))
{
    ((CMainFrame*) this->GetParent())->ChangeRulerPosition(CMainFrame::rpBottom);
}

void CDrawPanel::OnAboutClick(wxCommandEvent& WXUNUSED(Event))
{
    wxMessageBox("This is about window", "About...");
}

void CDrawPanel::OnCloseClick(wxCommandEvent& Event)
{
    //::wxExit();
    ((CMainFrame*) this->GetParent())->OnExit(Event);
}

void CDrawPanel::PaintEvent(wxPaintEvent& Event)
{
    wxPaintDC dc(this);
    Render(dc);
}

void CDrawPanel::PaintNow()
{
    wxClientDC dc(this);
    Render(dc);
}

void CDrawPanel::Render(wxDC& dc)
{
    // Retrieve surface size.
    wxSize size(this->GetClientSize());
    wxRect surfaceRect(0, 0, size.x, size.y);

    // Draw ruler's surface.
    DrawRulerSurface(dc, surfaceRect);

    // Draw ruler's scale.
    DrawRulerScale(dc, surfaceRect);

    // Draw ruler's markers.
    DrawRulerMarkers(dc, surfaceRect);
}

void CDrawPanel::DrawRulerMarkers(wxDC& dc, wxRect& SurfaceRect)
{

}

void CDrawPanel::DrawRulerScale(wxDC& dc, wxRect& SurfaceRect)
{
    // Prepare DC. We will draw ruler's scale on it.
    unsigned int I, sT, pT;
    double ID;
    wxString TmpS;
    CMainFrame* pMainFrame = ((CMainFrame*) this->GetParent());
    
    dc.SetBrush(wxBrush(wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT));
    dc.SetPen(wxPen(pMainFrame->m_RulerScaleColour, 1, wxPENSTYLE_SOLID));

    wxFont font(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(font);

    switch (pMainFrame->m_eRulerPosition)
    {
    case CMainFrame::rpLeft:
        switch (pMainFrame->m_eRulerUnits)
        {
        case CMainFrame::ruCentimetres:
            sT = (SurfaceRect.GetBottom() - SurfaceRect.GetTop()) - 10;
            ID = 0.0;
            I = CentimetresToPixelsVertical(0, ID);
            while (I < sT)
            {
                pT = 4 + I;

                if (AreSame(ID, 0.0))
                {
                    dc.DrawLine(wxPoint(0, pT), wxPoint(12, pT));

                    TmpS = "0.0";
                    dc.DrawText(TmpS, wxPoint(14, pT - 2));
                }
                else
                {
                    if (AreSame(std::fmod(ID, 1.0), 0.0))
                    {
                        dc.DrawLine(wxPoint(0, pT), wxPoint(5, pT));

                        TmpS = wxString::FromDouble(ID, 2);
                        wxCoord TextWidth, TextHeight;
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(TmpS, wxPoint(5, pT - (TextHeight / 2)));
                    }
                    else
                    {
                        dc.DrawLine(wxPoint(0, pT), wxPoint(3, pT));
                    }
                }

                ID = ID + 0.125;
                I = CentimetresToPixelsVertical(0, ID);
            }

            dc.DrawLine(wxPoint(0, 4 + sT), wxPoint(12, 4 + sT));

            TmpS = wxString::FromDouble(PixelsToCentimetresVertical(0, sT), 2);
            wxCoord TextWidth, TextHeight;
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
            dc.DrawText(TmpS, wxPoint(14, 4 + sT - TextHeight));

            break;
        case CMainFrame::ruInches:

            break;
        case CMainFrame::ruPixels:

            break;
        case CMainFrame::ruPicas:

            break;
        }

        break;
    case CMainFrame::rpTop:
        switch (pMainFrame->m_eRulerUnits)
        {
        case CMainFrame::ruCentimetres:
            sT = (SurfaceRect.GetRight() - SurfaceRect.GetLeft()) - 10;
            ID = 0.0;
            I = CentimetresToPixelsHorizontal(0, ID);
            while (I < sT)
            {
                pT = 4 + I;

                if (AreSame(ID, 0.0))
                {
                    dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 12));

                    TmpS = "0.00";
                    dc.DrawText(TmpS, wxPoint(pT, 12));
                }
                else
                {
                    if (AreSame(std::fmod(ID, 1.0), 0.0))
                    {
                        dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 5));

                        TmpS = wxString::FromDouble(ID, 2);
                        wxCoord TextWidth, TextHeight;
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(TmpS, wxPoint(pT - (TextWidth / 2), 5));
                    }
                    else
                    {
                        dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 3));
                    }
                }

                ID = ID + 0.125;
                I = CentimetresToPixelsHorizontal(0, ID);
            }

            dc.DrawLine(wxPoint(4 + sT, 0), wxPoint(4 + sT, 12));

            TmpS = wxString::FromDouble(PixelsToCentimetresHorizontal(0, sT), 2);
            wxCoord TextWidth, TextHeight;
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
            dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, 12));

            break;
        case CMainFrame::ruInches:

            break;
        case CMainFrame::ruPixels:

            break;
        case CMainFrame::ruPicas:

            break;
        }

        break;
    case CMainFrame::rpRight:
        switch (pMainFrame->m_eRulerUnits)
        {
        case CMainFrame::ruCentimetres:
            sT = (SurfaceRect.GetBottom() - SurfaceRect.GetTop()) - 10;
            ID = 0.0;
            I = CentimetresToPixelsVertical(0, ID);
            while (I < sT)
            {
                pT = 4 + I;

                if (AreSame(ID, 0.0))
                {
                    dc.DrawLine(wxPoint(SurfaceRect.GetRight() - 1 - 12, pT), wxPoint(SurfaceRect.GetRight(), pT));

                    TmpS = "0.00";

                    wxCoord TextWidth, TextHeight;
                    dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                    dc.DrawText(TmpS, wxPoint(SurfaceRect.GetRight() - 1 - 14 - TextWidth, pT - 2));
                }
                else
                {
                    if (AreSame(std::fmod(ID, 1.0), 0.0))
                    {
                        dc.DrawLine(wxPoint(SurfaceRect.GetRight() - 1 - 5, pT), wxPoint(SurfaceRect.GetRight(), pT));

                        TmpS = wxString::FromDouble(ID, 2);
                        wxCoord TextWidth, TextHeight;
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(TmpS, wxPoint(SurfaceRect.GetRight() - 1 - 5 - TextWidth, pT - (TextHeight / 2)));
                    }
                    else
                    {
                        dc.DrawLine(wxPoint(SurfaceRect.GetRight() - 1 - 3, pT), wxPoint(SurfaceRect.GetRight(), pT));
                    }
                }

                ID = ID + 0.125;
                I = CentimetresToPixelsHorizontal(0, ID);
            }

            dc.DrawLine(wxPoint(SurfaceRect.GetRight() - 1 - 12, 4 + sT), wxPoint(SurfaceRect.GetRight(), 4 + sT));

            TmpS = wxString::FromDouble(PixelsToCentimetresHorizontal(0, sT), 2);
            wxCoord TextWidth, TextHeight;
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
            dc.DrawText(TmpS, wxPoint(SurfaceRect.GetRight() - 1 - 14 - TextWidth, 4 + sT - TextHeight));

            break;
        case CMainFrame::ruInches:

            break;
        case CMainFrame::ruPixels:

            break;
        case CMainFrame::ruPicas:

            break;
        }

        break;
    case CMainFrame::rpBottom:
        switch (pMainFrame->m_eRulerUnits)
        {
        case CMainFrame::ruCentimetres:
            sT = (SurfaceRect.GetRight() - SurfaceRect.GetLeft()) - 10;
            ID = 0.0;
            I = CentimetresToPixelsHorizontal(0, ID);
            while (I < sT)
            {
                pT = 4 + I;

                if (AreSame(ID, 0.0))
                {
                    dc.DrawLine(wxPoint(pT, SurfaceRect.GetBottom() - 1 - 12), wxPoint(pT, SurfaceRect.GetBottom()));

                    TmpS = "0.00";

                    wxCoord TextWidth, TextHeight;
                    dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                    dc.DrawText(TmpS, wxPoint(pT, SurfaceRect.GetBottom() - 1 - 12 - TextHeight));
                }
                else
                {
                    if (AreSame(std::fmod(ID, 1.0), 0.0))
                    {
                        dc.DrawLine(wxPoint(pT, SurfaceRect.GetBottom() - 1 - 5), wxPoint(pT, SurfaceRect.GetBottom()));

                        TmpS = wxString::FromDouble(ID, 2);
                        wxCoord TextWidth, TextHeight;
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(TmpS, wxPoint(pT - (TextWidth / 2), SurfaceRect.GetBottom() - 1 - 5 - TextHeight));
                    }
                    else
                    {
                        dc.DrawLine(wxPoint(pT, SurfaceRect.GetBottom() - 1 - 3), wxPoint(pT, SurfaceRect.GetBottom()));
                    }
                }

                ID = ID + 0.125;
                I = CentimetresToPixelsHorizontal(0, ID);
            }

            dc.DrawLine(wxPoint(4 + sT, SurfaceRect.GetBottom() - 1 - 12), wxPoint(4 + sT, SurfaceRect.GetBottom()));

            TmpS = wxString::FromDouble(PixelsToCentimetresHorizontal(0, sT), 2);
            wxCoord TextWidth, TextHeight;
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
            dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, SurfaceRect.GetBottom() - 1 - 12 - TextHeight));

            break;
        case CMainFrame::ruInches:

            break;
        case CMainFrame::ruPixels:

            break;
        case CMainFrame::ruPicas:

            break;
        }

        break;
    }
}

void CDrawPanel::DrawRulerSurface(wxDC& dc, wxRect& SurfaceRect)
{
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

    switch (pMainFrame->m_eRulerPosition)
    {
    case CMainFrame::rpLeft:
        switch (pMainFrame->m_eRulerBackgroundType)
        {
        case CMainFrame::btSolid:
            // Prepare dc's brush.
            dc.SetBrush(wxBrush(pMainFrame->m_RulerBackgroundColor, wxBRUSHSTYLE_SOLID));
            // Prepare dc's pen.
            dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

            // Draw rectangle on whole surface size.
            dc.DrawRectangle(SurfaceRect);

            break;
        case CMainFrame::btGradient:
            break;
        case CMainFrame::btImage:
            break;
        }

        break;
    case CMainFrame::rpTop:
        switch (pMainFrame->m_eRulerBackgroundType)
        {
        case CMainFrame::btSolid:
            // Prepare dc's brush.
            dc.SetBrush(wxBrush(pMainFrame->m_RulerBackgroundColor, wxBRUSHSTYLE_SOLID));
            // Prepare dc's pen.
            dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

            // Draw rectangle on whole surface size.
            dc.DrawRectangle(SurfaceRect);

            break;
        case CMainFrame::btGradient:
            break;
        case CMainFrame::btImage:
            break;
        }

        break;
    case CMainFrame::rpRight:
        switch (pMainFrame->m_eRulerBackgroundType)
        {
        case CMainFrame::btSolid:
            // Prepare dc's brush.
            dc.SetBrush(wxBrush(pMainFrame->m_RulerBackgroundColor, wxBRUSHSTYLE_SOLID));
            // Prepare dc's pen.
            dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

            // Draw rectangle on whole surface size.
            dc.DrawRectangle(SurfaceRect);

            break;
        case CMainFrame::btGradient:
            break;
        case CMainFrame::btImage:
            break;
        }

        break;
    case CMainFrame::rpBottom:
        switch (pMainFrame->m_eRulerBackgroundType)
        {
        case CMainFrame::btSolid:
            // Prepare dc's brush.
            dc.SetBrush(wxBrush(pMainFrame->m_RulerBackgroundColor, wxBRUSHSTYLE_SOLID));
            // Prepare dc's pen.
            dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

            // Draw rectangle on whole surface size.
            dc.DrawRectangle(SurfaceRect);

            break;
        case CMainFrame::btGradient:
            break;
        case CMainFrame::btImage:
            break;
        }

        break;
    }
}

void CDrawPanel::OnLeftDown(wxMouseEvent& Event)
{
    wxPoint Pos = Event.GetPosition();
    CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();
    int Width, Height, Offset;
    pMainFrame->GetSize(&Width, &Height);
    Offset = pMainFrame->m_nOffsetBorder;

    if (Event.LeftDown() && 
        ((Pos.x >= Offset && Pos.y >= Offset) &&
         (Pos.x < Width - Offset && Pos.y < Height - Offset)))
    {
        // Retrieve mouse position from Event.
        wxPoint Pos = Event.GetPosition();

        m_bDragging = true;

        // Convert to screen position.
        m_DragStartPos = ClientToScreen(Pos);

        CaptureMouse();
    }
    else
    {
        Event.Skip();
    }
}

void CDrawPanel::OnLeftUp(wxMouseEvent& Event)
{
    if (m_bDragging)
    {
        m_bDragging = false;

        ReleaseMouse();
    }
    else
    {
        Event.Skip();
    }
}

void CDrawPanel::OnMouseMove(wxMouseEvent& Event)
{
    if (m_bDragging)
    {
        wxPoint Pos = ClientToScreen(Event.GetPosition());
        wxPoint diff = Pos - m_DragStartPos;
        GetParent()->Move(GetParent()->GetPosition() + diff);

        // Update starting position.
        m_DragStartPos = Pos;
    }
    else
    {
        Event.Skip();
    }
}

void CDrawPanel::OnMouseLeave(wxMouseEvent& Event)
{
    if (!m_bDragging)
    {
        SetCursor(wxNullCursor);
    }
    else
    {
        Event.Skip();
    }
}
