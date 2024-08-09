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
#define ID_ABOUT 2006
#define ID_CLOSE 2007

BEGIN_EVENT_TABLE(CDrawPanel, wxPanel)
// Catch paint events.
EVT_PAINT(CDrawPanel::PaintEvent)
EVT_MOUSE_EVENTS(CDrawPanel::OnMouseEvent)

//EVT_MENU(ID_, CDrawPanel::OnClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_LEFT, CDrawPanel::OnScaleOnLeftClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_RIGHT, CDrawPanel::OnScaleOnRightClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_TOP, CDrawPanel::OnScaleOnTopClick)
EVT_MENU(ID_RULER_POSITION_SCALE_ON_BOTTOM, CDrawPanel::OnScaleOnBottomClick)
EVT_MENU(wxID_ABOUT, CDrawPanel::OnAboutClick)
EVT_MENU(wxID_CLOSE, CDrawPanel::OnCloseClick)
END_EVENT_TABLE()

// Forward declarations.
class CMainFrame;

CDrawPanel::CDrawPanel(wxFrame* pParent) :
    wxPanel(pParent)
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
        // Show PopupMenu at position.
        wxMenu* pMenu = new wxMenu();
        wxMenu* pRulerPositionMenu = new wxMenu();

        pRulerPositionMenu->Append(ID_RULER_POSITION_SCALE_ON_LEFT, wxString("Scale on &left"));
        pRulerPositionMenu->Append(ID_RULER_POSITION_SCALE_ON_RIGHT, wxString("Scale on &right"));
        pRulerPositionMenu->Append(ID_RULER_POSITION_SCALE_ON_TOP, wxString("Scale on &top"));
        pRulerPositionMenu->Append(ID_RULER_POSITION_SCALE_ON_BOTTOM, wxString("Scale on &bottom"));

        pMenu->Append(ID_RULER_POSITION, wxString("&Ruler's position"), pRulerPositionMenu);

        pMenu->Append(wxID_ABOUT, wxString("&About"));
        pMenu->Append(wxID_CLOSE, wxString("&Close"));

        PopupMenu(pMenu, Event.GetPosition());
    }
    else
    {
        Event.Skip();
    }
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

void CDrawPanel::OnCloseClick(wxCommandEvent& WXUNUSED(Event))
{
    ::wxExit();
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
        case CMainFrame::ruCentimeters:
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
        case CMainFrame::ruCentimeters:
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
        case CMainFrame::ruCentimeters:
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
        case CMainFrame::ruCentimeters:
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
        /*
        // Draw some text.
        dc.DrawText(wxT("Testing"), 40, 60);

        // Draw a circle.
        dc.SetBrush(*wxGREEN_BRUSH); // Green filling
        dc.SetPen(wxPen(wxColor(255, 0, 0), 5)); // 5-pixels-thick red outline.
        dc.DrawCircle(wxPoint(200, 100), 25);

        // Draw a rectangle.
        dc.SetBrush(*wxBLUE_BRUSH); // Blue filling.
        dc.SetPen(wxPen(wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline.
        dc.DrawRectangle(300, 100, 400, 200);

        // Draw a line.
        dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // Black line, 3 pixels thick
        dc.DrawLine(300, 100, 700, 300); // Draw line across the rectangle.

        // Look at the wxDC docs to learn how to draw other stuff.
        */
}
