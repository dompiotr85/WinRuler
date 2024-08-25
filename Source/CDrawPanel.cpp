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

namespace WinRuler
{
    BEGIN_EVENT_TABLE(CDrawPanel, wxPanel)

    // Catch paint events.
    EVT_PAINT(CDrawPanel::PaintEvent)

    // Catch mouse events.
    EVT_MOUSE_EVENTS(CDrawPanel::OnMouseEvent)
    EVT_LEFT_DOWN(CDrawPanel::OnLeftDown)
    EVT_LEFT_UP(CDrawPanel::OnLeftUp)
    EVT_MOTION(CDrawPanel::OnMouseMove)
    EVT_LEAVE_WINDOW(CDrawPanel::OnMouseLeave)

    END_EVENT_TABLE()

    CDrawPanel::CDrawPanel(wxFrame* pParent) :
        wxPanel(pParent),
        m_bDragging(false)
    {
        // No code.
    }

    void CDrawPanel::OnMouseEvent(wxMouseEvent& Event)
    {
        wxPoint Pos = Event.GetPosition();
        wxWindow* pWindow = wxDynamicCast(Event.GetEventObject(), wxWindow);
        Pos = pWindow->ClientToScreen(Pos);
        if (Event.RightDown())
        {
            // Retrieve pointer to CMainFrame class.
            CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

            // Create pMenu, pRulerPositionMenu.
            wxMenu* pMenu = new wxMenu();
            wxMenu* pRulerPositionMenu = new wxMenu();

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
            pMenu->Append(ID_NEW_RULER_LENGTH, wxString("Set new ruler &length..."));
            pMenu->AppendSeparator();

            // Append option menu item.
            pMenu->Append(ID_OPTIONS, wxString("&Options..."));
            pMenu->AppendSeparator();

            // Append About and Close items separated.
            pMenu->Append(wxID_ABOUT, wxString("&About..."));
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

    void CDrawPanel::PaintEvent(wxPaintEvent& Event)
    {
        // Create wxPaintDC.
        wxPaintDC dc(this);

        // Draw on created DC.
        Render(dc);
    }

    void CDrawPanel::PaintNow()
    {
        // Create wxPaintDC.
        wxClientDC dc(this);

        // Draw on created DC.
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
        wxString TmpS;
        wxCoord TextWidth, TextHeight;

        // Retrieve pointer to CMainFrame and store it in pMainFrame.
        CMainFrame* pMainFrame = (CMainFrame*)this->GetParent();

        // Set DC's brush to transparent.
        dc.SetBrush(wxBrush(wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT));

        // Set DC's pen colour to first marker colour and style to solid.
        dc.SetPen(wxPen(pMainFrame->m_cFirstMarkerColour, 1, wxPENSTYLE_SOLID));

        // Set DC's font size and font colour.
        wxFont font(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        dc.SetFont(font);
        dc.SetTextForeground(pMainFrame->m_cRulerScaleColour);

        // Draw ruler's marker and its information depending on ruler's
        // position.
        switch (pMainFrame->m_eRulerPosition)
        {
        case CMainFrame::rpLeft:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft(),
                    SurfaceRect.GetTop() + 4 +
                    pMainFrame->m_iFirstMarkerPosition),
                wxPoint(
                    SurfaceRect.GetRight(),
                    SurfaceRect.GetTop() + 4 +
                    pMainFrame->m_iFirstMarkerPosition));

            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresHorizontal(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" cm");

                break;
            case CMainFrame::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesHorizontal(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case CMainFrame::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasHorizontal(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case CMainFrame::ruPixels:
                TmpS =
                    wxString::Format(
                        wxT("%d px"), pMainFrame->m_iFirstMarkerPosition);

                break;
            }

            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

            if (SurfaceRect.GetTop() + 4 + pMainFrame->m_iFirstMarkerPosition +
                4 + TextHeight > SurfaceRect.GetBottom())
            {
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        SurfaceRect.GetLeft() + 
                        ((SurfaceRect.GetWidth() / 3) * 2) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 + 
                        pMainFrame->m_iFirstMarkerPosition - 4 - TextHeight));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        ((SurfaceRect.GetWidth() / 3) * 2) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iFirstMarkerPosition + 4));
            }

            // Draw second marker if its value is other than -1.
            if (pMainFrame->m_iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(pMainFrame->m_cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft(),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iSecondMarkerPosition),
                    wxPoint(
                        SurfaceRect.GetRight(),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iSecondMarkerPosition));

                switch (pMainFrame->m_eRulerUnits)
                {
                case CMainFrame::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case CMainFrame::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case CMainFrame::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case CMainFrame::ruPixels:
                    TmpS =
                        wxString::Format(
                            wxT("%d px"), pMainFrame->m_iSecondMarkerPosition);

                    break;
                }

                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

                if (SurfaceRect.GetTop() + 4 + pMainFrame->m_iSecondMarkerPosition +
                    4 + TextHeight > SurfaceRect.GetBottom())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            ((SurfaceRect.GetWidth() / 3) * 2) - (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            pMainFrame->m_iSecondMarkerPosition - 4 - TextHeight));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            ((SurfaceRect.GetWidth() / 3) * 2) - (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            pMainFrame->m_iSecondMarkerPosition + 4));
                }
            }
            
            break;
        case CMainFrame::rpTop:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft() + 4 +
                    pMainFrame->m_iFirstMarkerPosition,
                    SurfaceRect.GetTop()),
                wxPoint(
                    SurfaceRect.GetLeft() + 4 +
                    pMainFrame->m_iFirstMarkerPosition,
                    SurfaceRect.GetBottom()));

            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresVertical(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" cm");

                break;
            case CMainFrame::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesVertical(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case CMainFrame::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasVertical(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case CMainFrame::ruPixels:
                TmpS =
                    wxString::Format(
                        wxT("%d px"), pMainFrame->m_iFirstMarkerPosition);

                break;
            }

            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

            if (SurfaceRect.GetLeft() + 4 + pMainFrame->m_iFirstMarkerPosition +
                TextWidth > SurfaceRect.GetRight())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + 
                        pMainFrame->m_iFirstMarkerPosition - 4 - TextWidth,
                        SurfaceRect.GetTop() +
                        ((SurfaceRect.GetHeight() / 3) * 2)));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iFirstMarkerPosition + 4,
                        SurfaceRect.GetTop() +
                        ((SurfaceRect.GetHeight() / 3) * 2)));
            }

            // Draw second marker if its value is other than -1.
            if (pMainFrame->m_iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(pMainFrame->m_cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iSecondMarkerPosition,
                        SurfaceRect.GetTop()),
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iSecondMarkerPosition,
                        SurfaceRect.GetBottom()));

                switch (pMainFrame->m_eRulerUnits)
                {
                case CMainFrame::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case CMainFrame::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case CMainFrame::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case CMainFrame::ruPixels:
                    TmpS =
                        wxString::Format(
                            wxT("%d px"), pMainFrame->m_iSecondMarkerPosition);

                    break;
                }

                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

                if (SurfaceRect.GetLeft() + 4 +
                    pMainFrame->m_iSecondMarkerPosition + 4 +
                    TextWidth > SurfaceRect.GetRight())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 + 
                            pMainFrame->m_iSecondMarkerPosition - 4 - TextWidth,
                            SurfaceRect.GetTop() +
                            ((SurfaceRect.GetHeight() / 3) * 2)));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 +
                            pMainFrame->m_iSecondMarkerPosition + 4,
                            SurfaceRect.GetTop() +
                            ((SurfaceRect.GetHeight() / 3) * 2)));
                }
            }

            break;
        case CMainFrame::rpRight:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft(),
                    SurfaceRect.GetTop() + 4 +
                    pMainFrame->m_iFirstMarkerPosition),
                wxPoint(
                    SurfaceRect.GetRight(),
                    SurfaceRect.GetTop() + 4 +
                    pMainFrame->m_iFirstMarkerPosition));

            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresHorizontal(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" cm");

                break;
            case CMainFrame::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesHorizontal(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case CMainFrame::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasHorizontal(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case CMainFrame::ruPixels:
                TmpS =
                    wxString::Format(
                        wxT("%d px"), pMainFrame->m_iFirstMarkerPosition);

                break;
            }

            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

            if (SurfaceRect.GetTop() + 4 + pMainFrame->m_iFirstMarkerPosition +
                4 + TextHeight > SurfaceRect.GetBottom())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iFirstMarkerPosition - 4 - TextHeight));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iFirstMarkerPosition + 4));
            }

            // Draw second marker if its value is other than -1.
            if (pMainFrame->m_iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(
                    wxPen(
                        pMainFrame->m_cSecondMarkerColour, 1, 
                        wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft(),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iSecondMarkerPosition),
                    wxPoint(
                        SurfaceRect.GetRight(),
                        SurfaceRect.GetTop() + 4 +
                        pMainFrame->m_iSecondMarkerPosition));

                switch (pMainFrame->m_eRulerUnits)
                {
                case CMainFrame::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case CMainFrame::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case CMainFrame::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case CMainFrame::ruPixels:
                    TmpS =
                        wxString::Format(
                            wxT("%d px"), pMainFrame->m_iSecondMarkerPosition);

                    break;
                }

                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

                if (SurfaceRect.GetTop() + 4 + pMainFrame->m_iSecondMarkerPosition +
                    4 + TextHeight > SurfaceRect.GetBottom())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            pMainFrame->m_iSecondMarkerPosition - 4 - 
                            TextHeight));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            pMainFrame->m_iSecondMarkerPosition + 4));
                }
            }

            break;
        case CMainFrame::rpBottom:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft() + 4 +
                    pMainFrame->m_iFirstMarkerPosition,
                    SurfaceRect.GetTop()),
                wxPoint(
                    SurfaceRect.GetLeft() + 4 +
                    pMainFrame->m_iFirstMarkerPosition,
                    SurfaceRect.GetBottom()));

            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresVertical(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" cm");

                break;
            case CMainFrame::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesVertical(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case CMainFrame::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasVertical(
                            0, pMainFrame->m_iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case CMainFrame::ruPixels:
                TmpS =
                    wxString::Format(
                        wxT("%d px"), pMainFrame->m_iFirstMarkerPosition);

                break;
            }

            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

            if (SurfaceRect.GetLeft() + 4 + pMainFrame->m_iFirstMarkerPosition +
                4 + TextWidth > SurfaceRect.GetRight())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iFirstMarkerPosition - 4 - TextWidth,
                        SurfaceRect.GetTop() +
                        (SurfaceRect.GetHeight() / 3) - TextHeight));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iFirstMarkerPosition + 4,
                        SurfaceRect.GetTop() +
                        (SurfaceRect.GetHeight() / 3) - TextHeight));
            }

            // Draw second marker if its value is other than -1.
            if (pMainFrame->m_iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(pMainFrame->m_cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iSecondMarkerPosition,
                        SurfaceRect.GetTop()),
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        pMainFrame->m_iSecondMarkerPosition,
                        SurfaceRect.GetBottom()));

                switch (pMainFrame->m_eRulerUnits)
                {
                case CMainFrame::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case CMainFrame::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case CMainFrame::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, pMainFrame->m_iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case CMainFrame::ruPixels:
                    TmpS =
                        wxString::Format(
                            wxT("%d px"), pMainFrame->m_iSecondMarkerPosition);

                    break;
                }

                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

                if (SurfaceRect.GetLeft() + 4 +
                    pMainFrame->m_iSecondMarkerPosition + 4 +
                    TextWidth > SurfaceRect.GetRight())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 +
                            pMainFrame->m_iSecondMarkerPosition - 4 - TextWidth,
                            SurfaceRect.GetTop() +
                            (SurfaceRect.GetHeight() / 3) - TextHeight));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 +
                            pMainFrame->m_iSecondMarkerPosition + 4,
                            SurfaceRect.GetTop() +
                            (SurfaceRect.GetHeight() / 3) - TextHeight));
                }
            }

            break;
        }
    }

    void CDrawPanel::DrawRulerScale(wxDC& dc, wxRect& SurfaceRect)
    {
        // Prepare DC. We will draw ruler's scale on it.
        unsigned int I, sT, pT;
        double ID;
        wxString TmpS;
        wxCoord TextWidth, TextHeight;

        // Retrieve our CMainFrame from CDrawPanel's parent instance.
        CMainFrame* pMainFrame = ((CMainFrame*) this->GetParent());

        // Set DC's brush to transparent.
        dc.SetBrush(wxBrush(wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT));

        // Set DC's pen to SOLID, with width equal 1 and colour equal
        // m_RulerScaleColour. 
        dc.SetPen(wxPen(pMainFrame->m_cRulerScaleColour, 1, wxPENSTYLE_SOLID));

        // Set DC's font size to 6 and font colour.
        wxFont font(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        dc.SetFont(font);
        dc.SetTextForeground(pMainFrame->m_cRulerScaleColour);

        // Depending on ruler's position and ruler's unit of measurement:
        switch (pMainFrame->m_eRulerPosition)
        {
        case CMainFrame::rpLeft:    // Ruler's position on left side.
            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres: // Ruler's unit as centimetres.
                sT = SurfaceRect.GetHeight() - 10;
                ID = 0.0;
                I = CentimetresToPixelsVertical(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(wxPoint(0, pT), wxPoint(12, pT));

                        TmpS = wxString("0.00");
                        dc.DrawText(TmpS, wxPoint(14, pT - 2));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(5, pT));

                            TmpS = wxString::FromDouble(ID, 2);
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
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(14, 4 + sT - TextHeight));

                break;
            case CMainFrame::ruInches:  // Ruler's unit as inches.
                sT = SurfaceRect.GetHeight() - 10;
                ID = 0.0;
                I = InchesToPixelsVertical(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(wxPoint(0, pT), wxPoint(12, pT));

                        TmpS = wxString("0.00");
                        dc.DrawText(TmpS, wxPoint(14, pT - 2));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(5, pT));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(5, pT - (TextHeight / 2)));
                        }
                        else
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(3, pT));
                        }
                    }

                    ID = ID + 0.125;
                    I = InchesToPixelsVertical(0, ID);
                }

                dc.DrawLine(wxPoint(0, 4 + sT), wxPoint(12, 4 + sT));

                TmpS = wxString::FromDouble(PixelsToInchesVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(14, 4 + sT - TextHeight));

                break;
            case CMainFrame::ruPixels:  // Ruler's unit as pixels.
                sT = (SurfaceRect.GetHeight() - 8) / 2;

                for (I = 0; I <= sT - 1; I++)
                {
                    pT = 4 + (I * 2);

                    if ((I == 0) || (I == sT - 1))
                    {
                        dc.DrawLine(wxPoint(0, pT), wxPoint(12, pT));

                        if (I == 0)
                        {
                            TmpS = wxString("0");
                            dc.DrawText(TmpS, wxPoint(14, pT - 2));
                        }

                        if (I == sT - 1)
                        {
                            TmpS = wxString::Format(wxT("%d"), (sT - 1) * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(14, pT - TextHeight + 2));
                        }
                    }
                    else
                    {
                        if (I % 10 == 0)
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(5, pT));
                            TmpS = wxString::Format(wxT("%d"), I * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(5, pT - (TextHeight / 2)));
                        }
                        else
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(3, pT));
                        }
                    }
                }

                break;
            case CMainFrame::ruPicas:   // Ruler's unit as picas.
                sT = SurfaceRect.GetHeight() - 10;
                ID = 0.0;
                I = PicasToPixelsVertical(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(wxPoint(0, pT), wxPoint(12, pT));

                        TmpS = wxString("0.00");
                        dc.DrawText(TmpS, wxPoint(14, pT - 2));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 6.0), 0.0))
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(5, pT));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(5, pT - (TextHeight / 2)));
                        }
                        else
                        {
                            if (AreSame(std::fmod(ID, 1.0), 0.0))
                            {
                                dc.DrawLine(wxPoint(0, pT), wxPoint(8, pT));
                            }
                            else
                            {
                                dc.DrawLine(wxPoint(0, pT), wxPoint(3, pT));
                            }
                        }
                    }

                    ID = ID + 0.125;
                    I = PicasToPixelsVertical(0, ID);
                }

                dc.DrawLine(wxPoint(0, 4 + sT), wxPoint(12, 4 + sT));

                TmpS = wxString::FromDouble(PixelsToPicasVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(14, 4 + sT - TextHeight));

                break;
            }

            break;
        case CMainFrame::rpTop:
            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres: // Ruler's unit as centimetres.
                sT = SurfaceRect.GetWidth() - 10;
                ID = 0.0;
                I = CentimetresToPixelsHorizontal(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 12));

                        TmpS = wxString("0.00");
                        dc.DrawText(TmpS, wxPoint(pT, 12));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 5));

                            TmpS = wxString::FromDouble(ID, 2);
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
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, 12));

                break;
            case CMainFrame::ruInches:  // Ruler's unit as inches.
                sT = SurfaceRect.GetWidth() - 10;
                ID = 0.0;
                I = InchesToPixelsHorizontal(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 12));

                        TmpS = wxString("0.00");
                        dc.DrawText(TmpS, wxPoint(pT, 12));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 5));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(pT - (TextWidth / 2), 5));
                        }
                        else
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 3));
                        }
                    }

                    ID = ID + 0.125;
                    I = InchesToPixelsHorizontal(0, ID);
                }

                dc.DrawLine(wxPoint(4 + sT, 0), wxPoint(4 + sT, 12));

                TmpS = wxString::FromDouble(PixelsToInchesHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, 12));

                break;
            case CMainFrame::ruPixels:  // Ruler's unit as pixels.
                sT = (SurfaceRect.GetWidth() - 8) / 2;

                for (I = 0; I <= sT - 1; I++)
                {
                    pT = 4 + (I * 2);

                    if ((I == 0) || (I == sT - 1))
                    {
                        dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 12));

                        if (I == 0)
                        {
                            TmpS = wxString("0");
                            dc.DrawText(TmpS, wxPoint(pT, 12));
                        }

                        if (I == sT - 1)
                        {
                            TmpS = wxString::Format(wxT("%d"), (sT - 1) * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(pT - TextWidth, 12));
                        }
                    }
                    else
                    {
                        if (I % 10 == 0)
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 5));

                            TmpS = wxString::Format(wxT("%d"), I * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(pT - (TextWidth / 2) + 2, 5));
                        }
                        else
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 3));
                        }
                    }
                }

                break;
            case CMainFrame::ruPicas:   // Ruler's unit as picas.
                sT = SurfaceRect.GetWidth() - 10;
                ID = 0.0;
                I = PicasToPixelsHorizontal(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 12));

                        TmpS = wxString("0.00");
                        dc.DrawText(TmpS, wxPoint(pT, 12));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 6.0), 0.0))
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 5));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(TmpS, wxPoint(pT - (TextWidth / 2), 5));
                        }
                        else
                        {
                            if (AreSame(std::fmod(ID, 1.0), 0.0))
                            {
                                dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 8));
                            }
                            else
                            {
                                dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 3));
                            }
                        }
                    }

                    ID = ID + 0.125;
                    I = PicasToPixelsHorizontal(0, ID);
                }

                dc.DrawLine(wxPoint(4 + sT, 0), wxPoint(4 + sT, 12));

                TmpS = wxString::FromDouble(PixelsToPicasHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, 12));

                break;
            }

            break;
        case CMainFrame::rpRight:
            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres: // Ruler's unit as centimetres.
                sT = SurfaceRect.GetHeight() - 10;
                ID = 0.0;
                I = CentimetresToPixelsVertical(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(
                            wxPoint(SurfaceRect.GetRight() - 1 - 12, pT),
                            wxPoint(SurfaceRect.GetRight(), pT));

                        TmpS = wxString("0.00");
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(
                            TmpS, 
                            wxPoint(
                                SurfaceRect.GetRight() - 1 - 14 - TextWidth, 
                                pT - 2));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight() - 1 - 5, pT), 
                                wxPoint(SurfaceRect.GetRight(), pT));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 5 - TextWidth, 
                                    pT - (TextHeight / 2)));
                        }
                        else
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight() - 1 - 3, pT), 
                                wxPoint(SurfaceRect.GetRight(), pT));
                        }
                    }

                    ID = ID + 0.125;
                    I = CentimetresToPixelsVertical(0, ID);
                }

                dc.DrawLine(
                    wxPoint(SurfaceRect.GetRight() - 1 - 12, 4 + sT), 
                    wxPoint(SurfaceRect.GetRight(), 4 + sT));

                TmpS = 
                    wxString::FromDouble(
                        PixelsToCentimetresVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        SurfaceRect.GetRight() - 1 - 14 - TextWidth, 
                        4 + sT - TextHeight));

                break;
            case CMainFrame::ruInches:  // Ruler's unit as inches.
                sT = SurfaceRect.GetHeight() - 10;
                ID = 0.0;
                I = InchesToPixelsVertical(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(
                            wxPoint(SurfaceRect.GetRight(), pT), 
                            wxPoint(SurfaceRect.GetRight() - 1 - 12, pT));

                        TmpS = wxString("0.00");
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(
                            TmpS, 
                            wxPoint(
                                SurfaceRect.GetRight() - 1 - 14 - TextWidth, 
                                pT - 2));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight(), pT), 
                                wxPoint(SurfaceRect.GetRight() - 1 - 5, pT));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 5 - TextWidth, 
                                    pT - (TextHeight / 2)));
                        }
                        else
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight() - 1 - 3, pT), 
                                wxPoint(SurfaceRect.GetRight(), pT));
                        }
                    }

                    ID = ID + 0.125;
                    I = InchesToPixelsVertical(0, ID);
                }

                dc.DrawLine(
                    wxPoint(SurfaceRect.GetRight() - 1 - 12, 4 + sT), 
                    wxPoint(SurfaceRect.GetRight(), 4 + sT));

                TmpS = wxString::FromDouble(PixelsToInchesVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        SurfaceRect.GetRight() - 1 - 14 - TextWidth, 
                        4 + sT - TextHeight));

                break;
            case CMainFrame::ruPixels:  // Ruler's unit as pixels.
                sT = (SurfaceRect.GetHeight() - 8) / 2;

                for (I = 0; I <= sT - 1; I++)
                {
                    pT = 4 + (I * 2);

                    if ((I == 0) || (I == sT - 1))
                    {
                        dc.DrawLine(
                            wxPoint(SurfaceRect.GetRight() - 1 - 12, pT), 
                            wxPoint(SurfaceRect.GetRight(), pT));

                        if (I == 0)
                        {
                            TmpS = wxString("0");
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 15 - TextWidth, 
                                    pT - 2));
                        }

                        if (I == sT - 1)
                        {
                            TmpS = wxString::Format(wxT("%d"), (sT - 1) * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 17 - TextWidth, 
                                    pT - TextHeight + 2));
                        }
                    }
                    else
                    {
                        if (I % 10 == 0)
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight() - 1 - 5, pT), 
                                wxPoint(SurfaceRect.GetRight(), pT));
                            TmpS = wxString::Format(wxT("%d"), I * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 5 - TextWidth, 
                                    pT - (TextHeight / 2)));
                        }
                        else
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight() - 1 - 3, pT), 
                                wxPoint(SurfaceRect.GetRight(), pT));
                        }
                    }
                }

                break;
            case CMainFrame::ruPicas:   // Ruler's unit as picas.
                sT = SurfaceRect.GetHeight() - 10;
                ID = 0.0;
                I = PicasToPixelsVertical(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(
                            wxPoint(SurfaceRect.GetRight() - 1 - 12, pT), 
                            wxPoint(SurfaceRect.GetRight(), pT));

                        TmpS = wxString("0.00");
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(
                            TmpS, 
                            wxPoint(
                                SurfaceRect.GetRight() - 1 - 14 - TextWidth, 
                                pT - 2));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 6.0), 0.0))
                        {
                            dc.DrawLine(
                                wxPoint(SurfaceRect.GetRight() - 1 - 5, pT), 
                                wxPoint(SurfaceRect.GetRight(), pT));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 5 - TextWidth, 
                                    pT - (TextHeight / 2)));
                        }
                        else
                        {
                            if (AreSame(std::fmod(ID, 1.0), 0.0))
                            {
                                dc.DrawLine(
                                    wxPoint(SurfaceRect.GetRight() - 1 - 8, pT), 
                                    wxPoint(SurfaceRect.GetRight(), pT));
                            }
                            else
                            {
                                dc.DrawLine(
                                    wxPoint(SurfaceRect.GetRight() - 1 - 3, pT), 
                                    wxPoint(SurfaceRect.GetRight(), pT));
                            }
                        }
                    }

                    ID = ID + 0.125;
                    I = PicasToPixelsVertical(0, ID);
                }

                dc.DrawLine(
                    wxPoint(SurfaceRect.GetRight() - 1 - 12, 4 + sT), 
                    wxPoint(SurfaceRect.GetRight(), 4 + sT));

                TmpS = wxString::FromDouble(PixelsToPicasVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        SurfaceRect.GetRight() - 1 - 14 - TextWidth, 
                        4 + sT - TextHeight));

                break;
            }

            break;
        case CMainFrame::rpBottom:
            switch (pMainFrame->m_eRulerUnits)
            {
            case CMainFrame::ruCentimetres: // Ruler's unit as centimetres.
                sT = SurfaceRect.GetWidth() - 10;
                ID = 0.0;
                I = CentimetresToPixelsHorizontal(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(
                            wxPoint(pT, SurfaceRect.GetBottom() - 1 - 12), 
                            wxPoint(pT, SurfaceRect.GetBottom()));

                        TmpS = wxString("0.00");
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(
                            TmpS, 
                            wxPoint(
                                pT, 
                                SurfaceRect.GetBottom() - 1 - 12 - TextHeight));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 1 - 5),
                                wxPoint(pT, SurfaceRect.GetBottom()));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    pT - (TextWidth / 2), 
                                    SurfaceRect.GetBottom() - 1 - 5 - TextHeight));
                        }
                        else
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 1 - 3), 
                                wxPoint(pT, SurfaceRect.GetBottom()));
                        }
                    }

                    ID = ID + 0.125;
                    I = CentimetresToPixelsHorizontal(0, ID);
                }

                dc.DrawLine(
                    wxPoint(4 + sT, SurfaceRect.GetBottom() - 1 - 12), 
                    wxPoint(4 + sT, SurfaceRect.GetBottom()));

                TmpS = 
                    wxString::FromDouble(
                        PixelsToCentimetresHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        4 + sT - TextWidth, 
                        SurfaceRect.GetBottom() - 1 - 12 - TextHeight));

                break;
            case CMainFrame::ruInches:  // Ruler's unit as inches.
                sT = SurfaceRect.GetWidth() - 10;
                ID = 0.0;
                I = InchesToPixelsHorizontal(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(
                            wxPoint(pT, SurfaceRect.GetBottom() - 1 - 12), 
                            wxPoint(pT, SurfaceRect.GetBottom()));

                        TmpS = wxString("0.00");
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(
                            TmpS, 
                            wxPoint(
                                pT, 
                                SurfaceRect.GetBottom() - 1 - 12 - TextHeight));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 1.0), 0.0))
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 5),
                                wxPoint(pT, SurfaceRect.GetBottom()));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    pT - (TextWidth / 2), 
                                    SurfaceRect.GetBottom() - 1 - 5 - TextHeight));
                        }
                        else
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 1 - 3), 
                                wxPoint(pT, SurfaceRect.GetBottom()));
                        }
                    }

                    ID = ID + 0.125;
                    I = InchesToPixelsHorizontal(0, ID);
                }

                dc.DrawLine(
                    wxPoint(4 + sT, SurfaceRect.GetBottom() - 1 - 12), 
                    wxPoint(4 + sT, SurfaceRect.GetBottom()));

                TmpS = 
                    wxString::FromDouble(PixelsToInchesHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        4 + sT - TextWidth, 
                        SurfaceRect.GetBottom() - 1 - 12 - TextHeight));

                break;
            case CMainFrame::ruPixels:  // Ruler's unit as pixels.
                sT = (SurfaceRect.GetWidth() - 8) / 2;

                for (I = 0; I <= sT - 1; I++)
                {
                    pT = 4 + (I * 2);

                    if ((I == 0) || (I == sT - 1))
                    {
                        dc.DrawLine(
                            wxPoint(pT, SurfaceRect.GetBottom() - 12), 
                            wxPoint(pT, SurfaceRect.GetBottom()));

                        if (I == 0)
                        {
                            TmpS = wxString("0");
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    pT, 
                                    SurfaceRect.GetBottom() - 12 - TextHeight));
                        }

                        if (I == sT - 1)
                        {
                            TmpS = wxString::Format(wxT("%d"), (sT - 1) * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    pT - TextWidth, 
                                    SurfaceRect.GetBottom() - 12 - TextHeight));
                        }
                    }
                    else
                    {
                        if (I % 10 == 0)
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 6), 
                                wxPoint(pT, SurfaceRect.GetBottom()));

                            TmpS = wxString::Format(wxT("%d"), I * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    pT - (TextWidth / 2) + 2, 
                                    SurfaceRect.GetBottom() - 5 - TextHeight));
                        }
                        else
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 4), 
                                wxPoint(pT, SurfaceRect.GetBottom()));
                        }
                    }
                }

                break;
            case CMainFrame::ruPicas:   // Ruler's unit as picas.
                sT = SurfaceRect.GetWidth() - 10;
                ID = 0.0;
                I = PicasToPixelsHorizontal(0, ID);
                while (I < sT)
                {
                    pT = 4 + I;

                    if (AreSame(ID, 0.0))
                    {
                        dc.DrawLine(
                            wxPoint(pT, SurfaceRect.GetBottom() - 1 - 12), 
                            wxPoint(pT, SurfaceRect.GetBottom()));

                        TmpS = wxString("0.00");
                        dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                        dc.DrawText(
                            TmpS, 
                            wxPoint(
                                pT, 
                                SurfaceRect.GetBottom() - 1 - 12 - TextHeight));
                    }
                    else
                    {
                        if (AreSame(std::fmod(ID, 6.0), 0.0))
                        {
                            dc.DrawLine(
                                wxPoint(pT, SurfaceRect.GetBottom() - 1 - 5), 
                                wxPoint(pT, SurfaceRect.GetBottom()));

                            TmpS = wxString::FromDouble(ID, 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, 
                                wxPoint(
                                    pT - (TextWidth / 2), 
                                    SurfaceRect.GetBottom() - 1 - 5 - TextHeight));
                        }
                        else
                        {
                            if (AreSame(std::fmod(ID, 1.0), 0.0))
                            {
                                dc.DrawLine(
                                    wxPoint(pT, SurfaceRect.GetBottom() - 1 - 8), 
                                    wxPoint(pT, SurfaceRect.GetBottom()));
                            }
                            else
                            {
                                dc.DrawLine(
                                    wxPoint(pT, SurfaceRect.GetBottom() - 1 - 3), 
                                    wxPoint(pT, SurfaceRect.GetBottom()));
                            }
                        }
                    }

                    ID = ID + 0.125;
                    I = PicasToPixelsHorizontal(0, ID);
                }

                dc.DrawLine(
                    wxPoint(4 + sT, SurfaceRect.GetBottom() - 1 - 12), 
                    wxPoint(4 + sT, SurfaceRect.GetBottom()));

                TmpS = 
                    wxString::FromDouble(PixelsToPicasHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(
                    TmpS, 
                    wxPoint(
                        4 + sT - TextWidth, 
                        SurfaceRect.GetBottom() - 1 - 12 - TextHeight));

                break;
            }

            break;
        }
    }

    void CDrawPanel::DrawRulerSurface(wxDC& dc, wxRect& SurfaceRect)
    {
        // Retrieve pointer to our CMainFrame.
        CMainFrame* pMainFrame = (CMainFrame*)this->GetParent();

        // Draw surface of ruler:
        switch (pMainFrame->m_eRulerPosition)
        {
        case CMainFrame::rpLeft:    // Ruler's position on left side.
            switch (pMainFrame->m_eRulerBackgroundType)
            {
            case CMainFrame::btSolid:   // Ruler's background as solid.
                // Prepare DC's brush.
                dc.SetBrush(
                    wxBrush(
                        pMainFrame->m_cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));
                
                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            case CMainFrame::btGradient:    // Ruler's background as gradient.
                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    pMainFrame->m_cRulerBackgroundStartColour,
                    pMainFrame->m_cRulerBackgroundEndColour,
                    wxRIGHT);

                break;
            case CMainFrame::btImage:   // Ruler's background as image.
                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageTopVertical,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));
                
                for (int i = 0; i <= (SurfaceRect.GetHeight() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        pMainFrame->m_bRulerBackgroundImageMiddleVertical,
                        wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY() + 4 + (2 * i)));
                }
                
                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageBottomVertical,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetBottom() - 4));

                break;
            }

            break;
        case CMainFrame::rpTop: // Ruler's position on top.
            switch (pMainFrame->m_eRulerBackgroundType)
            {
            case CMainFrame::btSolid:   // Ruler's background as solid.
                // Prepare DC's brush.
                dc.SetBrush(
                    wxBrush(
                        pMainFrame->m_cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));

                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            case CMainFrame::btGradient:    // Ruler's background as gradient.
                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    pMainFrame->m_cRulerBackgroundStartColour,
                    pMainFrame->m_cRulerBackgroundEndColour,
                    wxDOWN);

                break;
            case CMainFrame::btImage:   // Ruler's background as image.
                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageLeftHorizontal,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetWidth() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        pMainFrame->m_bRulerBackgroundImageMiddleHorizontal,
                        wxPoint(SurfaceRect.GetX() + 4 + (2 * i), SurfaceRect.GetY()));
                }

                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageRightHorizontal,
                    wxPoint(SurfaceRect.GetRight() - 4, SurfaceRect.GetTop()));

                break;
            }

            break;
        case CMainFrame::rpRight:   // Ruler's position on right side.
            switch (pMainFrame->m_eRulerBackgroundType)
            {
            case CMainFrame::btSolid:   // Ruler's background as solid.
                // Prepare DC's brush.
                dc.SetBrush(
                    wxBrush(
                        pMainFrame->m_cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));
                
                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            case CMainFrame::btGradient:    // Ruler's background as gradient.
                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    pMainFrame->m_cRulerBackgroundStartColour,
                    pMainFrame->m_cRulerBackgroundEndColour,
                    wxLEFT);

                break;
            case CMainFrame::btImage:   // Ruler's background as image.
                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageTopVertical,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetHeight() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        pMainFrame->m_bRulerBackgroundImageMiddleVertical,
                        wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY() + 4 + (2 * i)));
                }

                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageBottomVertical,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetBottom() - 4));

                break;
            }

            break;
        case CMainFrame::rpBottom:  // Ruler's position on bottom.
            switch (pMainFrame->m_eRulerBackgroundType)
            {
            case CMainFrame::btSolid:   // Ruler's background as solid.
                // Prepare DC's brush.
                dc.SetBrush(
                    wxBrush(
                        pMainFrame->m_cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));

                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            case CMainFrame::btGradient:    // Ruler's background as gradient.
                // Prepare DC's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    pMainFrame->m_cRulerBackgroundStartColour,
                    pMainFrame->m_cRulerBackgroundEndColour,
                    wxUP);

                break;
            case CMainFrame::btImage:   // Ruler's background as image.
                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageLeftHorizontal,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetWidth() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        pMainFrame->m_bRulerBackgroundImageMiddleHorizontal,
                        wxPoint(SurfaceRect.GetX() + 4 + (2 * i), SurfaceRect.GetY()));
                }

                dc.DrawBitmap(
                    pMainFrame->m_bRulerBackgroundImageRightHorizontal,
                    wxPoint(SurfaceRect.GetRight() - 4, SurfaceRect.GetTop()));

                break;
            }

            break;
        }

        // Draw black outline.
        dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
        dc.SetBrush(wxBrush(wxColour(0, 0, 0), wxBRUSHSTYLE_TRANSPARENT));
        dc.DrawRectangle(
            wxRect(0, 0, SurfaceRect.GetWidth(), SurfaceRect.GetHeight()));
    }

    void CDrawPanel::OnLeftDown(wxMouseEvent& Event)
    {
        // Retrieve Event position.
        wxPoint Pos = Event.GetPosition();

        // Get pointer to our CMainFrame from CDrawPanel parent instance.
        CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

        // Retrieve CMainFrame ClientWidth and ClientHeight.
        int Width, Height, Offset;
        pMainFrame->GetClientSize(&Width, &Height);

        // Retrieve CMainFrame m_nOffsetBorder.
        Offset = pMainFrame->m_nOffsetBorder;

        // If mouse left button is down, then we can set second marker position
        // for measurement.
        if (Event.LeftDown())
        {
            switch (pMainFrame->m_eRulerPosition)
            {
            case CMainFrame::rpLeft:
            case CMainFrame::rpRight:
                if ((Pos.y >= 4) && (Pos.y < pMainFrame->m_iRulerLength - 5))
                {
                    pMainFrame->m_iSecondMarkerPosition = Pos.y - 4;
                }

                break;
            case CMainFrame::rpTop:
            case CMainFrame::rpBottom:
                if ((Pos.x >= 4) && (Pos.x < pMainFrame->m_iRulerLength - 5))
                {
                    pMainFrame->m_iSecondMarkerPosition = Pos.x - 4;
                }

                break;
            }
        }

        // If mouse left button is down and Event mouse position is inside
        // dragging space, then ...
        if (Event.LeftDown() &&
            ((Pos.x >= Offset && Pos.y >= Offset) &&
                (Pos.x < Width - Offset && Pos.y < Height - Offset)))
        {
            // ... signal that we are in dragging mode.
            m_bDragging = true;

            // Convert Event position to screen position.
            m_DragStartPos = ClientToScreen(Pos);

            // Start capturing mouse.
            CaptureMouse();
        }
        else
        {
            // ... otherwise skip this Event.
            Event.Skip();
        }
    }

    void CDrawPanel::OnLeftUp(wxMouseEvent& Event)
    {
        // If currently dragging, then ...
        if (m_bDragging)
        {
            // .. finish dragging and ...
            m_bDragging = false;

            // ... release mouse.
            ReleaseMouse();
        }
        else
        {
            // ... otherwise skip this event.
            Event.Skip();
        }
    }

    void CDrawPanel::OnMouseMove(wxMouseEvent& Event)
    {
        // Retrieve mouse position from event.
        wxPoint Pos = Event.GetPosition();

        // Retrieve pointer to CMainFrame.
        CMainFrame* pMainFrame = (CMainFrame*)this->GetParent();

        // Update marker position depending on current ruler's position.
        switch (pMainFrame->m_eRulerPosition)
        {
        case CMainFrame::rpLeft:
        case CMainFrame::rpRight:
            if ((Pos.y >= 4) && (Pos.y < pMainFrame->m_iRulerLength - 5))
            {
                pMainFrame->m_iFirstMarkerPosition = Pos.y - 4;
            }

            break;
        case CMainFrame::rpTop:
        case CMainFrame::rpBottom:
            if ((Pos.x >= 4) && (Pos.x < pMainFrame->m_iRulerLength - 5))
            {
                pMainFrame->m_iFirstMarkerPosition = Pos.x - 4;
            }

            break;
        }

        Refresh();

        // If currently dragging, then ...
        if (m_bDragging)
        {
            // ... retrieve event position and cast it as ClientToScreen.
            Pos = ClientToScreen(Pos);

            // Calculate difference.
            wxPoint diff = Pos - m_DragStartPos;

            // Move parent frame which is our CMainFrame to new position which
            // is current position + our calculated diff.
            GetParent()->Move(GetParent()->GetPosition() + diff);

            // Update starting position.
            m_DragStartPos = Pos;
        }
        else
        {
            // .. otherwise skip this Event.
            Event.Skip();
        }
    }

    void CDrawPanel::OnMouseLeave(wxMouseEvent& Event)
    {
        // If currently we aren't in dragging mode, then ...
        if (!m_bDragging)
        {
            // .. set cursor to wxNullCursor.
            SetCursor(wxNullCursor);
        }
        else
        {
            // .. otherwise skip this Event.
            Event.Skip();
        }
    }
} // end namespace WinRuler
