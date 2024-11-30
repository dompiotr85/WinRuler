/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include "WRUtilities.h"

namespace WinRuler
{
    /**
     * DrawPanel class definition.
     **/
    class CDrawPanel :
        public wxPanel
    {
        DECLARE_EVENT_TABLE()
    public:
        CDrawPanel(wxFrame* pParent);

        /**
         * Called by the system of by wxWidgets when the panel needs to be redrawn.
         * You can also trigger this call by calling Refresh()/Update().
         **/
        void OnPaintEvent(wxPaintEvent& Event);

        /**
         * Alternatively, you can use a clientDC to paint on the panel at any time.
         * Using this generally does not free you from catching paint events, since
         * it is possible that e.g. the window manager throws away your drawing
         * when the window comes to the background, and expects you will redraw it
         * when the window comes back (by sending a paint event).
         *
         * In most cases, this will not be needed at all; simply handling paint
         * events and calling Refresh() when a refresh is needed will do the job.
         **/
        void PaintNow();

        /**
         * Here we do the actual rendering. I put it in a separate method so that
         * it can work no matter what type of DC (e.g. wxPaintDC or wxClientDC) is
         * used.
         **/
        void Render(wxDC& dc);

        void OnMouseEvent(wxMouseEvent& Event);
    public:
        void DrawRulerMarkers(
            wxDC& dc, wxRect& SurfaceRect,
            ERulerPosition eRulerPosition,
            ERulerUnits eRulerUnits,
            wxColour& cRulerScaleColour,
            wxColour& cFirstMarkerColour,
            wxColour& cSecondMarkerColour,
            int iFirstMarkerPosition,
            int iSecondMarkerPosition);

        void DrawRulerScale(
            wxDC& dc, wxRect& SurfaceRect,
            wxColour& cRulerScaleColour,
            ERulerPosition eRulerPosition,
            ERulerUnits eRulerUnits);
        
        void DrawRulerSurface(
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
            wxBitmap RulerBackgroundBitmapBottomV);
    private:
        void OnLeftDown(wxMouseEvent& Event);
        void OnLeftUp(wxMouseEvent& Event);
        void OnMouseMove(wxMouseEvent& Event);
        void OnMouseLeave(wxMouseEvent& Event);
    private:
        bool m_bDragging;
        wxPoint m_DragStartPos;
        wxSize m_InitialSize;
        wxPoint m_InitialPos;
    };
} // end namespace WinRuler
