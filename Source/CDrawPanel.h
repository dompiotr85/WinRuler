/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include "WRUtilities.h"

namespace WinRuler
{
    /**
     * CDrawPanel class definition.
     **/
    class CDrawPanel :
        public wxPanel
    {
        DECLARE_EVENT_TABLE()
    public:
        CDrawPanel(wxFrame* pParent);

        /**
         * Called by the system of by wxWidgets when the panel needs to be
         * redrawn. You can also trigger this call by calling
         * Refresh() or Update().
         * 
		 * \param Event     The event that triggered the call.
         **/
        void OnPaintEvent(wxPaintEvent& Event);

        /**
         * Alternatively, you can use a clientDC to paint on the panel at any
         * time. Using this generally does not free you from catching paint
         * events, since it is possible that e.g. the window manager throws
         * away your drawing when the window comes to the background, and
         * expects you will redraw it when the window comes back (by sending
         * a paint event).
         *
         * In most cases, this will not be needed at all; simply handling paint
         * events and calling Refresh() when a refresh is needed will do the
         * job.
         **/
        void PaintNow();

        /**
         * Here we do the actual rendering. I put it in a separate method 
         * that it can work no matter what type of DC (e.g. wxPaintDC or
         * wxClientDC) is used.
         * 
		 * \param dc        The device context to use for rendering.
         **/
        void Render(wxDC& dc);

		/**
		 * This is the event handler for mouse events. It is called when the
		 * user clicks on the panel.
		 *
		 * \param Event     The event that triggered the call.
         **/
        void OnMouseEvent(wxMouseEvent& Event);
    public:
		/**
		 * This method draws the ruler markers on the specified device context
         * takeing all specified parameters into account.
         * 
		 * \param dc                    The device context to use for
         *                              rendering.
		 * \param SurfaceRect           The rectangle that defines the area to
         *                              draw in.
		 * \param eRulerPosition        The position of the ruler (horizontal
		 *                              or vertical).
		 * \param eRulerUnits           The units of the ruler (pixels, mm,
		 *                              inches, etc.).
		 * \param cRulerScaleColour     The colour of the ruler scale.
		 * \param cFirstMarkerColour    The colour of the first marker.
		 * \param cSecondMarkerColour   The colour of the second marker.
		 * \param iFirstMarkerPosition  The position of the first marker.
		 * \param iSecondMarkerPosition The position of the second marker.
         **/
        void DrawRulerMarkers(
            wxDC& dc, wxRect& SurfaceRect,
            ERulerPosition eRulerPosition,
            ERulerUnits eRulerUnits,
            wxColour& cRulerScaleColour,
            wxColour& cFirstMarkerColour,
            wxColour& cSecondMarkerColour,
            int iFirstMarkerPosition,
            int iSecondMarkerPosition);

        /**
		 * This method draws the ruler scale on the specified device context
		 * takeing all specified parameters into account.
         * 
		 * \param dc                    The device context to use for
         *                              rendering.
		 * \param SurfaceRect           The rectangle that defines the area to
		 *                              render in.
		 * \param cRulerScaleColour     The colour of the ruler scale.
		 * \param eRulerPosition        The position of the ruler (horizontal
		 *                              or vertical).
		 * \param eRulerUnits           The units of the ruler (pixels, mm,
		 *                              inches, etc.).
         **/
        void DrawRulerScale(
            wxDC& dc, wxRect& SurfaceRect,
            wxColour& cRulerScaleColour,
            ERulerPosition eRulerPosition,
            ERulerUnits eRulerUnits);
        
        /**
		 * This method draws the ruler surface on the specified device context
		 * takeing all specified parameters into account.
         * 
		 * \param dc                        The device context to use for
		 *                                  rendering.
		 * \param SurfaceRect               The rectangle that defines the area
         *                                  to render in.
		 * \param eRulerPosition            The position of the ruler
         *                                  (horizontal or vertical).
		 * \param eRulerBackgroundType      The type of the ruler background
		 *                                  (solid color, gradient or bitmap).
		 * \param cRulerBackgroundColour    The colour of the ruler background.
		 * \param cRulerBackgroundStartColour The start colour of the gradient
         *                                    if gradient background type is
         *                                    used, otherwise ignored.
		 * \param cRulerBackgroundEndColour   The end colour of the gradient
		 *                                    if gradient background type is
		 *                                    used, otherwise ignored.
		 * \param RulerBackgroundBitmapLeftH The bitmap to use for the
		 *                                   horizontal left background if
         *                                   bitmap background type is used,
		 *                                   otherwise ignored.
		 * \param RulerBackgroundBitmapMiddleH The bitmap to use for the
		 *                                     horizontal middle background if
		 *                                     bitmap background type is used,
		 *                                     otherwise ignored.
		 * \param RulerBackgroundBitmapRightH The bitmap to use for the
		 *                                    horizontal right background if
		 *                                    bitmap background type is used,
		 *                                    otherwise ignored.
		 * \param RulerBackgroundBitmapTopV The bitmap to use for the vertical
		 *                                  top background if bitmap background
		 *                                  type is used, otherwise ignored.
		 * \param RulerBackgroundBitmapMiddleV The bitmap to use for the
		 *                                     vertical middle background if
		 *                                     bitmap background type is used,
		 *                                     otherwise ignored.
		 * \param RulerBackgroundBitmapBottomV The bitmap to use for the
		 *                                     vertical bottom background type
		 *                                     if bitmap background type is
		 *                                     used, otherwise ignored.
         **/
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
        /**
		 * This is the event handler for the left mouse button down event.
         * 
		 * \param Event     The event that triggered the call.
         **/
        void OnLeftDown(wxMouseEvent& Event);

        /**
		 * This is the event handler for the left mouse button up event.
         * 
		 * \param Event     The event that triggered the call.
         **/
        void OnLeftUp(wxMouseEvent& Event);

        /**
		 * This is the event handler for the mouse motion event. It is called
		 * when the user moves the mouse over the panel.
         * 
		 * \param Event     The event that triggered the call.
         **/
        void OnMouseMove(wxMouseEvent& Event);

        /**
		 * This is the event handler for the mouse wheel event. It is called
		 * when the user scrolls the mouse wheel over the panel.
         * 
		 * \param Event     The event that triggered the call.
         **/
        void OnMouseLeave(wxMouseEvent& Event);
    private:
		//! Dragging flag.
        bool m_bDragging;

		//! Dragging start position.
        wxPoint m_DragStartPos;

		//! Initial size of the panel.
        wxSize m_InitialSize;

		//! Initial position of the panel.
        wxPoint m_InitialPos;
    };
} // end namespace WinRuler
