/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include <cmath>
#include <wx/wx.h>
#include "CMainFrame.h"
#include "CDrawPanel.h"
#include "WRUtilities.h"

namespace WinRuler
{
    BEGIN_EVENT_TABLE(CDrawPanel, wxPanel)

    // Catch paint events.
    EVT_PAINT(CDrawPanel::OnPaintEvent)

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
            CMainFrame* pMainFrame = 
                static_cast<CMainFrame*>(this->GetParent());

            // Create pMenu, pRulerPositionMenu.
            wxMenu* pMenu = new wxMenu();
            wxMenu* pRulerPositionMenu = new wxMenu();

            // Build pRulerPositionMenu items.
            pRulerPositionMenu->AppendRadioItem(
                ID_RULER_POSITION_SCALE_ON_LEFT,
                wxString("Scale on &left side"))->Check(
                    pMainFrame->m_eRulerPosition == ERulerPosition::rpLeft ? 
                        true : false);
            pRulerPositionMenu->AppendRadioItem(
                ID_RULER_POSITION_SCALE_ON_RIGHT,
                wxString("Scale on &right side"))->Check(
                    pMainFrame->m_eRulerPosition == ERulerPosition::rpRight ? 
                        true : false);
            pRulerPositionMenu->AppendRadioItem(
                ID_RULER_POSITION_SCALE_ON_TOP,
                wxString("Scale on &top"))->Check(
                    pMainFrame->m_eRulerPosition == ERulerPosition::rpTop ? 
                        true : false);
            pRulerPositionMenu->AppendRadioItem(
                ID_RULER_POSITION_SCALE_ON_BOTTOM,
                wxString("Scale on &bottom"))->Check(
                    pMainFrame->m_eRulerPosition == ERulerPosition::rpBottom ?
                        true : false);

            // Append pRulerPositionMenu as submenu of ID_RULER_POSITION item.
            pMenu->Append(
                ID_RULER_POSITION, wxString("&Ruler position"),
                pRulerPositionMenu);
            pMenu->AppendSeparator();

            // Append measuring units as radio items.
            pMenu->AppendRadioItem(
                ID_PIXELS_AS_UNIT,
                wxString("&Pixels as unit"))->Check(
                    pMainFrame->m_eRulerUnits == ERulerUnits::ruPixels ? 
                        true : false);
            pMenu->AppendRadioItem(
                ID_CENTIMETRES_AS_UNIT,
                wxString("&Centimetres as unit"))->Check(
                    pMainFrame->m_eRulerUnits == ERulerUnits::ruCentimetres ? 
                        true : false);
            pMenu->AppendRadioItem(
                ID_INCHES_AS_UNIT,
                wxString("&Inches as unit"))->Check(
                    pMainFrame->m_eRulerUnits == ERulerUnits::ruInches ? 
                        true : false);
            pMenu->AppendRadioItem(
                ID_PICAS_AS_UNIT,
                wxString("&Picas as unit"))->Check(
                    pMainFrame->m_eRulerUnits == ERulerUnits::ruPicas ? 
                        true : false);
            pMenu->AppendSeparator();

            // Append AlwaysOnTop item separated.
            wxMenuItem* pMenuItem =
                pMenu->AppendCheckItem(
                    ID_ALWAYS_ON_TOP, wxString("&Always on top"));
            pMenuItem->Check(pMainFrame->m_bAlwaysOnTop);
            pMenu->AppendSeparator();

            // Append new ruler length MenuItem.
            pMenu->Append(
                ID_NEW_RULER_LENGTH, wxString("Set new ruler &length..."));
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

    void CDrawPanel::OnPaintEvent(wxPaintEvent& Event)
    {
		// Create paint device context.
        wxPaintDC dc(this);

		// Render on created device context.
        Render(dc);
    }

    void CDrawPanel::PaintNow()
    {
		// Create paint device context.
        wxClientDC dc(this);

		// Render on created device context.
        Render(dc);
    }

    void CDrawPanel::Render(wxDC& dc)
    {
        // Retrieve pointer to CMainFrame class.
        CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

        // Retrieve surface size.
        wxSize size(this->GetClientSize());
        wxRect surfaceRect(0, 0, size.GetWidth(), size.GetHeight());

        // Draw ruler's surface.
        DrawRulerSurface(
            dc, surfaceRect,
            pMainFrame->m_eRulerPosition,
            pMainFrame->m_eRulerBackgroundType,
            pMainFrame->m_cRulerBackgroundColour,
            pMainFrame->m_cRulerBackgroundStartColour,
            pMainFrame->m_cRulerBackgroundEndColour,
            pMainFrame->m_RulerBackgroundBitmapLeftH,
            pMainFrame->m_RulerBackgroundBitmapMiddleH,
            pMainFrame->m_RulerBackgroundBitmapRightH,
            pMainFrame->m_RulerBackgroundBitmapTopV,
            pMainFrame->m_RulerBackgroundBitmapMiddleV,
            pMainFrame->m_RulerBackgroundBitmapBottomV);

        // Draw ruler's scale.
        DrawRulerScale(
            dc, surfaceRect,
            pMainFrame->m_cRulerScaleColour,
            pMainFrame->m_eRulerPosition,
            pMainFrame->m_eRulerUnits);

        // Draw ruler's markers.
        DrawRulerMarkers(
            dc, surfaceRect, 
            pMainFrame->m_eRulerPosition,
            pMainFrame->m_eRulerUnits,
            pMainFrame->m_cRulerScaleColour,
            pMainFrame->m_cFirstMarkerColour,
            pMainFrame->m_cSecondMarkerColour,
            pMainFrame->m_iFirstMarkerPosition,
            pMainFrame->m_iSecondMarkerPosition);
    }

    void CDrawPanel::DrawRulerMarkers(
        wxDC& dc, wxRect& SurfaceRect,
        ERulerPosition eRulerPosition,
        ERulerUnits eRulerUnits,
        wxColour& cRulerScaleColour,
        wxColour& cFirstMarkerColour,
        wxColour& cSecondMarkerColour,
        int iFirstMarkerPosition,
        int iSecondMarkerPosition)
    {
        wxString TmpS;
        wxCoord TextWidth, TextHeight;

		// If first marker position is -1, do not draw anything and return.
        if (iFirstMarkerPosition == -1)
            return;

		// Prepare device context. We will draw ruler's markers on it.
        
        // Set device context brush to transparent.
        dc.SetBrush(wxBrush(wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT));

		// Set device context pen colour to first marker colour and style to
        // solid.
        dc.SetPen(wxPen(cFirstMarkerColour, 1, wxPENSTYLE_SOLID));

		// Set device context font size to 6 and font colour to
		// m_RulerScaleColour. Additionally, set font style to normal and
		// weight to normal and family to default.
        wxFont font(
            6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        dc.SetFont(font);
		// Set device context text foreground colour to m_RulerScaleColour.
        dc.SetTextForeground(cRulerScaleColour);

		// Draw ruler's marker and its information depending on ruler's unit of
		// measurement and ruler's position.
        switch (eRulerPosition)
        {
        case ERulerPosition::rpLeft:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft(), SurfaceRect.GetTop() + 4 +
                    iFirstMarkerPosition),
                wxPoint(
                    SurfaceRect.GetRight(), SurfaceRect.GetTop() + 4 +
                    iFirstMarkerPosition));

			// Prepare temporary string TmpS for first marker.
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresHorizontal(
                            0, iFirstMarkerPosition), 2) +
                    wxString(" cm");

                break;
            case ERulerUnits::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesHorizontal(
                            0, iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case ERulerUnits::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasHorizontal(
                            0, iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case ERulerUnits::ruPixels:
                TmpS =
                    wxString::Format(
                        wxT("%d px"), iFirstMarkerPosition);

                break;
            }

            // Retrieve text width and text height of TmpS string.
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

			// Draw text of first marker on device context depending on ruler's
            // position and text width and height.
            if (SurfaceRect.GetTop() + 4 + iFirstMarkerPosition + 4 +
                TextHeight > SurfaceRect.GetBottom())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        ((SurfaceRect.GetWidth() / 3) * 2) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 +
                        iFirstMarkerPosition - 4 - TextHeight));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        ((SurfaceRect.GetWidth() / 3) * 2) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 +
                        iFirstMarkerPosition + 4));
            }

            // If second marker's position is other than -1, we can draw it.
            if (iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft(), SurfaceRect.GetTop() + 4 +
                        iSecondMarkerPosition),
                    wxPoint(
                        SurfaceRect.GetRight(), SurfaceRect.GetTop() + 4 +
                        iSecondMarkerPosition));

                // Prepare temporary string TmpS for second marker.
                switch (eRulerUnits)
                {
                case ERulerUnits::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case ERulerUnits::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case ERulerUnits::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case ERulerUnits::ruPixels:
                    TmpS =
                        wxString::Format(wxT("%d px"), iSecondMarkerPosition);

                    break;
                }

				// Retrieve text width and text height of TmpS string.
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

                // Draw text of second marker on device context depending on
                // ruler's position and text width and height.
                if (SurfaceRect.GetTop() + 4 + iSecondMarkerPosition + 4 +
                    TextHeight > SurfaceRect.GetBottom())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            ((SurfaceRect.GetWidth() / 3) * 2) - 
                            (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            iSecondMarkerPosition - 4 - TextHeight));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            ((SurfaceRect.GetWidth() / 3) * 2) - 
                            (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 + 
                            iSecondMarkerPosition + 4));
                }
            }

            break;
        case ERulerPosition::rpTop:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition,
                    SurfaceRect.GetTop()),
                wxPoint(
                    SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition,
                    SurfaceRect.GetBottom()));

			// Prepare temporary string TmpS for first marker.
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresVertical(0, iFirstMarkerPosition), 
                        2) +
                    wxString(" cm");

                break;
            case ERulerUnits::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesVertical(0, iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case ERulerUnits::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasVertical(0, iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case ERulerUnits::ruPixels:
                TmpS = wxString::Format(wxT("%d px"), iFirstMarkerPosition);

                break;
            }

			// Retrieve text width and text height of TmpS string.
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

			// Draw text of first marker on device context depending on ruler's
			// position and text width and height.
            if (SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition +
                TextWidth > SurfaceRect.GetRight())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        iFirstMarkerPosition - 4 - TextWidth,
                        SurfaceRect.GetTop() +
                        ((SurfaceRect.GetHeight() / 3) * 2)));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition + 4,
                        SurfaceRect.GetTop() +
                        ((SurfaceRect.GetHeight() / 3) * 2)));
            }

            // If second marker's position is other than -1, we can draw it.
            if (iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + iSecondMarkerPosition,
                        SurfaceRect.GetTop()),
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + iSecondMarkerPosition,
                        SurfaceRect.GetBottom()));

				// Prepare temporary string TmpS for second marker.
                switch (eRulerUnits)
                {
                case ERulerUnits::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case ERulerUnits::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case ERulerUnits::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case ERulerUnits::ruPixels:
                    TmpS =
                        wxString::Format(wxT("%d px"), iSecondMarkerPosition);

                    break;
                }

				// Retrieve text width and text height of TmpS string.
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

				// Draw text of second marker on device context depending on
				// ruler's position and text width and height.
                if (SurfaceRect.GetLeft() + 4 + iSecondMarkerPosition + 4 +
                    TextWidth > SurfaceRect.GetRight())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 +
                            iSecondMarkerPosition - 4 - TextWidth,
                            SurfaceRect.GetTop() +
                            ((SurfaceRect.GetHeight() / 3) * 2)));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 + 
                            iSecondMarkerPosition + 4,
                            SurfaceRect.GetTop() +
                            ((SurfaceRect.GetHeight() / 3) * 2)));
                }
            }

            break;
        case ERulerPosition::rpRight:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft(),
                    SurfaceRect.GetTop() + 4 + iFirstMarkerPosition),
                wxPoint(
                    SurfaceRect.GetRight(),
                    SurfaceRect.GetTop() + 4 + iFirstMarkerPosition));

			// Prepare temporary string TmpS for first marker.
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresHorizontal(0, iFirstMarkerPosition), 
                        2) +
                    wxString(" cm");

                break;
            case ERulerUnits::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesHorizontal(0, iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case ERulerUnits::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasHorizontal(0, iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case ERulerUnits::ruPixels:
                TmpS =
                    wxString::Format(wxT("%d px"), iFirstMarkerPosition);

                break;
            }

			// Retrieve text width and text height of TmpS string.
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

			// Draw text of first marker on device context depending on ruler's
			// position and text width and height.
            if (SurfaceRect.GetTop() + 4 + iFirstMarkerPosition + 4 +
                TextHeight > SurfaceRect.GetBottom())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 +
                        iFirstMarkerPosition - 4 - TextHeight));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() +
                        (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                        SurfaceRect.GetTop() + 4 + iFirstMarkerPosition + 4));
            }

			// If second marker's position is other than -1, we can draw it.
            if (iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft(),
                        SurfaceRect.GetTop() + 4 + iSecondMarkerPosition),
                    wxPoint(
                        SurfaceRect.GetRight(),
                        SurfaceRect.GetTop() + 4 + iSecondMarkerPosition));

				// Prepare temporary string TmpS for second marker.
                switch (eRulerUnits)
                {
                case ERulerUnits::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case ERulerUnits::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case ERulerUnits::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case ERulerUnits::ruPixels:
                    TmpS =
                        wxString::Format(wxT("%d px"), iSecondMarkerPosition);

                    break;
                }

				// Retrieve text width and text height of TmpS string.
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                
				// Draw text of second marker on device context depending on
				// ruler's position and text width and height.
                if (SurfaceRect.GetTop() + 4 + iSecondMarkerPosition + 4 +
                    TextHeight > SurfaceRect.GetBottom())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            iSecondMarkerPosition - 4 - TextHeight));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() +
                            (SurfaceRect.GetWidth() / 3) - (TextWidth / 2),
                            SurfaceRect.GetTop() + 4 +
                            iSecondMarkerPosition + 4));
                }
            }

            break;
        case ERulerPosition::rpBottom:
            // Draw first marker.
            dc.DrawLine(
                wxPoint(
                    SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition,
                    SurfaceRect.GetTop()),
                wxPoint(
                    SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition,
                    SurfaceRect.GetBottom()));

			// Prepare temporary string TmpS for first marker.
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres:
                TmpS =
                    wxString::FromDouble(
                        PixelsToCentimetresVertical(
                            0, iFirstMarkerPosition), 2) +
                    wxString(" cm");

                break;
            case ERulerUnits::ruInches:
                TmpS =
                    wxString::FromDouble(
                        PixelsToInchesVertical(
                            0, iFirstMarkerPosition), 2) +
                    wxString(" in");

                break;
            case ERulerUnits::ruPicas:
                TmpS =
                    wxString::FromDouble(
                        PixelsToPicasVertical(
                            0, iFirstMarkerPosition), 2) +
                    wxString(" pica");

                break;
            case ERulerUnits::ruPixels:
                TmpS = wxString::Format(wxT("%d px"), iFirstMarkerPosition);

                break;
            }

			// Retrieve text width and text height of TmpS string.
            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

			// Draw text of first marker on device context depending on ruler's
			// position and text width and height.
            if (SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition +
                4 + TextWidth > SurfaceRect.GetRight())
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 +
                        iFirstMarkerPosition - 4 - TextWidth,
                        SurfaceRect.GetTop() +
                        (SurfaceRect.GetHeight() / 3) - TextHeight));
            }
            else
            {
                dc.DrawText(
                    TmpS,
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + iFirstMarkerPosition + 4,
                        SurfaceRect.GetTop() +
                        (SurfaceRect.GetHeight() / 3) - TextHeight));
            }

			// If second marker's position is other than -1, we can draw it.
            if (iSecondMarkerPosition != -1)
            {
                // Set pen colour to second marker's colour.
                dc.SetPen(wxPen(cSecondMarkerColour, 1, wxPENSTYLE_SOLID));

                dc.DrawLine(
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + iSecondMarkerPosition,
                        SurfaceRect.GetTop()),
                    wxPoint(
                        SurfaceRect.GetLeft() + 4 + iSecondMarkerPosition,
                        SurfaceRect.GetBottom()));

				// Prepare temporary string TmpS for second marker.
                switch (eRulerUnits)
                {
                case ERulerUnits::ruCentimetres:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToCentimetresHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" cm");

                    break;
                case ERulerUnits::ruInches:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToInchesHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" in");

                    break;
                case ERulerUnits::ruPicas:
                    TmpS =
                        wxString::FromDouble(
                            PixelsToPicasHorizontal(
                                0, iSecondMarkerPosition), 2) +
                        wxString(" pica");

                    break;
                case ERulerUnits::ruPixels:
                    TmpS =
                        wxString::Format(
                            wxT("%d px"), iSecondMarkerPosition);

                    break;
                }

				// Retrieve text width and text height of TmpS string.
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);

				// Draw text of second marker on device context depending on
				// ruler's position and text width and height.
                if (SurfaceRect.GetLeft() + 4 + iSecondMarkerPosition + 4 +
                    TextWidth > SurfaceRect.GetRight())
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 +
                            iSecondMarkerPosition - 4 - TextWidth,
                            SurfaceRect.GetTop() +
                            (SurfaceRect.GetHeight() / 3) - TextHeight));
                }
                else
                {
                    dc.DrawText(
                        TmpS,
                        wxPoint(
                            SurfaceRect.GetLeft() + 4 + 
                            iSecondMarkerPosition + 4,
                            SurfaceRect.GetTop() +
                            (SurfaceRect.GetHeight() / 3) - TextHeight));
                }
            }

            break;
        }
    }

    void CDrawPanel::DrawRulerScale(
        wxDC& dc, wxRect& SurfaceRect,
        wxColour& cRulerScaleColour,
        ERulerPosition eRulerPosition,
        ERulerUnits eRulerUnits)
    {
		// Prepare device context. We will draw ruler's scale on it.
        unsigned int I, sT, pT;
        double ID;
        wxString TmpS;
        wxCoord TextWidth, TextHeight;

		// Set device context brush to transparent.
        dc.SetBrush(wxBrush(wxTransparentColour, wxBRUSHSTYLE_TRANSPARENT));

        // Set device context pen to SOLID, with width equal 1 and colour
		// equal to m_RulerScaleColour.
        dc.SetPen(wxPen(cRulerScaleColour, 1, wxPENSTYLE_SOLID));

        // Set device context font size to 6 and font style to normal, weight
        // to normal and family to default.
        wxFont font(
            6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        dc.SetFont(font);
		// Set device context text foreground colour to m_RulerScaleColour.
        dc.SetTextForeground(cRulerScaleColour);

        // Depending on ruler's position and ruler's unit of measurement:
        switch (eRulerPosition)
        {
        case ERulerPosition::rpLeft:    // Ruler's position on left side.
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres: // Ruler's unit as centimetres.
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
                            dc.DrawText(
                                TmpS, wxPoint(5, pT - (TextHeight / 2)));
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

                TmpS = 
                    wxString::FromDouble(
                        PixelsToCentimetresVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(14, 4 + sT - TextHeight));

                break;
            case ERulerUnits::ruInches:  // Ruler's unit as inches.
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
                            dc.DrawText(
                                TmpS, wxPoint(5, pT - (TextHeight / 2)));
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

                TmpS = 
                    wxString::FromDouble(PixelsToInchesVertical(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(14, 4 + sT - TextHeight));

                break;
            case ERulerUnits::ruPixels:  // Ruler's unit as pixels.
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
                            dc.DrawText(
                                TmpS, wxPoint(14, pT - TextHeight + 2));
                        }
                    }
                    else
                    {
                        if (I % 10 == 0)
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(5, pT));
                            TmpS = wxString::Format(wxT("%d"), I * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS, wxPoint(5, pT - (TextHeight / 2)));
                        }
                        else
                        {
                            dc.DrawLine(wxPoint(0, pT), wxPoint(3, pT));
                        }
                    }
                }

                break;
            case ERulerUnits::ruPicas:   // Ruler's unit as picas.
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
                            dc.DrawText(
                                TmpS, wxPoint(5, pT - (TextHeight / 2)));
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
        case ERulerPosition::rpTop:
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres: // Ruler's unit as centimetres.
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
                            dc.DrawText(
                                TmpS, wxPoint(pT - (TextWidth / 2), 5));
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

                TmpS = 
                    wxString::FromDouble(
                        PixelsToCentimetresHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, 12));

                break;
            case ERulerUnits::ruInches:  // Ruler's unit as inches.
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
                            dc.DrawText(
                                TmpS, wxPoint(pT - (TextWidth / 2), 5));
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

                TmpS = 
                    wxString::FromDouble(PixelsToInchesHorizontal(0, sT), 2);
                dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                dc.DrawText(TmpS, wxPoint(4 + sT - TextWidth, 12));

                break;
            case ERulerUnits::ruPixels:  // Ruler's unit as pixels.
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
                            dc.DrawText(
                                TmpS, wxPoint(pT - (TextWidth / 2) + 2, 5));
                        }
                        else
                        {
                            dc.DrawLine(wxPoint(pT, 0), wxPoint(pT, 3));
                        }
                    }
                }

                break;
            case ERulerUnits::ruPicas:   // Ruler's unit as picas.
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
                            dc.DrawText(
                                TmpS, wxPoint(pT - (TextWidth / 2), 5));
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
        case ERulerPosition::rpRight:
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres: // Ruler's unit as centimetres.
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
            case ERulerUnits::ruInches:  // Ruler's unit as inches.
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
            case ERulerUnits::ruPixels:  // Ruler's unit as pixels.
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
                                    SurfaceRect.GetRight() - 1 - 15 - 
                                    TextWidth, pT - 2));
                        }

                        if (I == sT - 1)
                        {
                            TmpS = wxString::Format(wxT("%d"), (sT - 1) * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS,
                                wxPoint(
                                    SurfaceRect.GetRight() - 1 - 17 - 
                                    TextWidth, pT - TextHeight + 2));
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
            case ERulerUnits::ruPicas:   // Ruler's unit as picas.
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
                                    wxPoint(
                                        SurfaceRect.GetRight() - 1 - 8, pT),
                                    wxPoint(SurfaceRect.GetRight(), pT));
                            }
                            else
                            {
                                dc.DrawLine(
                                    wxPoint(
                                        SurfaceRect.GetRight() - 1 - 3, pT),
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
        case ERulerPosition::rpBottom:
            switch (eRulerUnits)
            {
            case ERulerUnits::ruCentimetres: // Ruler's unit as centimetres.
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
                                SurfaceRect.GetBottom() - 1 - 12 - 
                                TextHeight));
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
                                    SurfaceRect.GetBottom() - 1 - 5 - 
                                    TextHeight));
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
            case ERulerUnits::ruInches:  // Ruler's unit as inches.
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
                                SurfaceRect.GetBottom() - 1 - 12 - 
                                TextHeight));
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
                                    SurfaceRect.GetBottom() - 1 - 5 - 
                                    TextHeight));
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
            case ERulerUnits::ruPixels:  // Ruler's unit as pixels.
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
                                    SurfaceRect.GetBottom() - 12 - 
                                    TextHeight));
                        }

                        if (I == sT - 1)
                        {
                            TmpS = wxString::Format(wxT("%d"), (sT - 1) * 2);
                            dc.GetTextExtent(TmpS, &TextWidth, &TextHeight);
                            dc.DrawText(
                                TmpS,
                                wxPoint(
                                    pT - TextWidth,
                                    SurfaceRect.GetBottom() - 12 - 
                                    TextHeight));
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
            case ERulerUnits::ruPicas:   // Ruler's unit as picas.
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
                                SurfaceRect.GetBottom() - 1 - 12 - 
                                TextHeight));
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
                                    SurfaceRect.GetBottom() - 1 - 5 - 
                                    TextHeight));
                        }
                        else
                        {
                            if (AreSame(std::fmod(ID, 1.0), 0.0))
                            {
                                dc.DrawLine(
                                    wxPoint(
                                        pT, SurfaceRect.GetBottom() - 1 - 8),
                                    wxPoint(pT, SurfaceRect.GetBottom()));
                            }
                            else
                            {
                                dc.DrawLine(
                                    wxPoint(
                                        pT, SurfaceRect.GetBottom() - 1 - 3),
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

    void CDrawPanel::DrawRulerSurface(
        wxDC& dc, wxRect& SurfaceRect,
        ERulerPosition eRulerPosition,
        ERulerBackgroundType eRulerBackgroundType,
        wxColour& cRulerBackgroundColour,
        wxColour& cRulerBackgroundStartColour,
        wxColour& cRulerBackgroundEndColour,
        wxBitmap RulerBackgroundBitmapLeftH,
        wxBitmap RulerBackgroundBitmapMiddleH,
        wxBitmap RulerBackgroundBitmapRightH,
        wxBitmap RulerBackgroundBitmapTopV,
        wxBitmap RulerBackgroundBitmapMiddleV,
        wxBitmap RulerBackgroundBitmapBottomV)
    {
        // Draw surface of ruler:
        switch (eRulerPosition)
        {
        case ERulerPosition::rpLeft:    // Ruler's position on left side.
            switch (eRulerBackgroundType)
            {
            // Ruler's background as solid.
            case ERulerBackgroundType::btSolid:
				// Prepare display context's brush.
                dc.SetBrush(
                    wxBrush(cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));

				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            // Ruler's background as gradient.
            case ERulerBackgroundType::btGradient:
				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    cRulerBackgroundStartColour, cRulerBackgroundEndColour,
                    wxRIGHT);

                break;
               // Ruler's background as image.
            case ERulerBackgroundType::btImage:
                // TODO: For now we use simple bitmap drawing. It must be done
                //       in a better way.

				dc.DrawBitmap(
					RulerBackgroundBitmapLeftH,
					wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));
                dc.DrawBitmap(
                    RulerBackgroundBitmapTopV,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetHeight() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        RulerBackgroundBitmapMiddleV,
                        wxPoint(
                            SurfaceRect.GetX(),
                            SurfaceRect.GetY() + 4 + (2 * i)));
                }

                dc.DrawBitmap(
                    RulerBackgroundBitmapBottomV,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetBottom() - 3));

                break;
            }

            break;
        // Ruler's position on top.
        case ERulerPosition::rpTop:
            switch (eRulerBackgroundType)
            {
            // Ruler's background as solid.
            case ERulerBackgroundType::btSolid:
				// Prepare display context's brush.
                dc.SetBrush(
                    wxBrush(cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));

				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            // Ruler's background as gradient.
            case ERulerBackgroundType::btGradient:
				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    cRulerBackgroundStartColour, cRulerBackgroundEndColour,
                    wxDOWN);

                break;
            // Ruler's background as image.
            case ERulerBackgroundType::btImage:
                // TODO: For now we use simple bitmap drawing. It must be done
                //       in a better way.

                dc.DrawBitmap(
                    RulerBackgroundBitmapLeftH,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetWidth() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        RulerBackgroundBitmapMiddleH,
                        wxPoint(
                            SurfaceRect.GetX() + 4 + (2 * i),
                            SurfaceRect.GetY()));
                }

                dc.DrawBitmap(
                    RulerBackgroundBitmapRightH,
                    wxPoint(SurfaceRect.GetRight() - 3, SurfaceRect.GetTop()));

                break;
            }

            break;
        // Ruler's position on right side.
        case ERulerPosition::rpRight:
            switch (eRulerBackgroundType)
            {
            // Ruler's background as solid.
            case ERulerBackgroundType::btSolid:
				// Prepare display context's brush.
                dc.SetBrush(
                    wxBrush(cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));

				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            // Ruler's background as gradient.
            case ERulerBackgroundType::btGradient:
				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    cRulerBackgroundStartColour, cRulerBackgroundEndColour,
                    wxLEFT);

                break;
            // Ruler's background as image.
            case ERulerBackgroundType::btImage:
                // TODO: For now we use simple bitmap drawing. It must be done
                //       in a better way.

                dc.DrawBitmap(
                    RulerBackgroundBitmapTopV,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetHeight() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        RulerBackgroundBitmapMiddleV,
                        wxPoint(
                            SurfaceRect.GetX(),
                            SurfaceRect.GetY() + 4 + (2 * i)));
                }

                dc.DrawBitmap(
                    RulerBackgroundBitmapBottomV,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetBottom() - 3));

                break;
            }

            break;
        // Ruler's position on bottom.
        case ERulerPosition::rpBottom:
            switch (eRulerBackgroundType)
            {
            // Ruler's background as solid.
            case ERulerBackgroundType::btSolid:
				// Prepare display context's brush.
                dc.SetBrush(
                    wxBrush(cRulerBackgroundColour, wxBRUSHSTYLE_SOLID));

				// Prepare display context's pen.
                dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw rectangle on whole surface size.
                dc.DrawRectangle(SurfaceRect);

                break;
            // Ruler's background as gradient.
            case ERulerBackgroundType::btGradient:
				// Prepare display context's pen
                dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_TRANSPARENT));

                // Draw gradient rectangle on whole surface size.
                dc.GradientFillLinear(
                    SurfaceRect,
                    cRulerBackgroundStartColour, cRulerBackgroundEndColour,
                    wxUP);

                break;
            // Ruler's background as image.
            case ERulerBackgroundType::btImage:
                // TODO: For now we use simple bitmap drawing. It must be done
                //       in a better way.

                dc.DrawBitmap(
                    RulerBackgroundBitmapLeftH,
                    wxPoint(SurfaceRect.GetX(), SurfaceRect.GetY()));

                for (int i = 0; i <= (SurfaceRect.GetWidth() - 8) / 2; i++)
                {
                    dc.DrawBitmap(
                        RulerBackgroundBitmapMiddleH,
                        wxPoint(
                            SurfaceRect.GetX() + 4 + (2 * i),
                            SurfaceRect.GetY()));
                }

                dc.DrawBitmap(
                    RulerBackgroundBitmapRightH,
                    wxPoint(SurfaceRect.GetRight() - 3, SurfaceRect.GetTop()));

                break;
            }

            break;
        }

        // If m_eRulerBackgroundType is btImage, then return. Otherwise we will
		// draw black outline around ruler.
		if (eRulerBackgroundType == ERulerBackgroundType::btImage)
			return;

        dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
        dc.SetBrush(wxBrush(wxColour(0, 0, 0), wxBRUSHSTYLE_TRANSPARENT));
        dc.DrawRectangle(
            wxRect(0, 0, SurfaceRect.GetWidth(), SurfaceRect.GetHeight()));
    }

    void CDrawPanel::OnLeftDown(wxMouseEvent& Event)
    {
        // Retrieve Event position.
        wxPoint Pos = Event.GetPosition();

		// Retrieve pointer to CMainFrame from CDrawPanel parent instance.
        CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

        // Get client width and height from pMainFrame.
        int Width, Height, Offset;
        pMainFrame->GetClientSize(&Width, &Height);

        // Retrieve also m_nOffsetBorder from pMainFrame.
        Offset = pMainFrame->m_iOffsetBorder;

        // If mouse left button is down, then we can set second marker position
        // for measurement.
        if (Event.LeftDown())
        {
            switch (pMainFrame->m_eRulerPosition)
            {
            case rpLeft:
            case rpRight:
                if ((Pos.y >= 4) && (Pos.y < pMainFrame->m_iRulerLength - 5))
                {
                    pMainFrame->m_iSecondMarkerPosition = Pos.y - 4;

#ifdef _DEBUG
                    wxLogInfo(
                        wxString("New position of second marker was set."));
#endif
                }

                break;
            case rpTop:
            case rpBottom:
                if ((Pos.x >= 4) && (Pos.x < pMainFrame->m_iRulerLength - 5))
                {
                    pMainFrame->m_iSecondMarkerPosition = Pos.x - 4;

#ifdef _DEBUG
                    wxLogInfo(
                        wxString("New position of second marker was set."));
#endif
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
#ifdef _DEBUG
            wxLogInfo(wxString("Dragging started."));
#endif

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

#ifdef _DEBUG
            wxLogInfo(wxString("Dragging ended."));
#endif
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
        CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

        // Update marker position depending on current ruler's position.
        switch (pMainFrame->m_eRulerPosition)
        {
        case rpLeft:
        case rpRight:
            if ((Pos.y >= 4) && (Pos.y < pMainFrame->m_iRulerLength - 5))
            {
                pMainFrame->m_iFirstMarkerPosition = Pos.y - 4;
            }

            break;
        case rpTop:
        case rpBottom:
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
            pMainFrame->Move(pMainFrame->GetPosition() + diff);

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
