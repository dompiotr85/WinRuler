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

class CDrawPanel :
    public wxPanel
{
private:
    void DrawRulerMarkers(wxDC& dc, wxRect& SurfaceRect);
    void DrawRulerScale(wxDC& dc, wxRect& SurfaceRect);
	void DrawRulerSurface(wxDC& dc, wxRect& SurfaceRect);

    void OnOptionsClick(wxCommandEvent& Event);
    void OnNewRulerLengthClick(wxCommandEvent& Event);
    void OnPixelsAsUnitClick(wxCommandEvent& Event);
    void OnCentimetresAsUnitClick(wxCommandEvent& Event);
    void OnInchesAsUnitClick(wxCommandEvent& Event);
    void OnPicasAsUnitClick(wxCommandEvent& Event);
    void OnAlwaysOnTopClick(wxCommandEvent& Event);
	void OnScaleOnLeftClick(wxCommandEvent& Event);
	void OnScaleOnTopClick(wxCommandEvent& Event);
	void OnScaleOnRightClick(wxCommandEvent& Event);
	void OnScaleOnBottomClick(wxCommandEvent& Event);
	void OnAboutClick(wxCommandEvent& Event);
	void OnCloseClick(wxCommandEvent& Event);
public:
    CDrawPanel(wxFrame* pParent);

    /**
     * Called by the system of by wxWidgets when the panel needs to be redrawn.
     * You can also trigger this call by calling Refresh()/Update().
     **/
    void PaintEvent(wxPaintEvent& Event);
	
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
public:
    DECLARE_EVENT_TABLE()
};
