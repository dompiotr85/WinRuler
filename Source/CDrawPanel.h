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
public:
    CDrawPanel(wxFrame* parent);

    /**
     * Called by the system of by wxWidgets when the panel needs to be redrawn.
     * You can also trigger this call by calling Refresh()/Update().
     **/
    void PaintEvent(wxPaintEvent& event);
	
    /**
     * Alternatively, you can use a clientDC to paint on the panel at any time.
     * Using this generally does not free you from catching paint events, since it
     * is possible that e.g. the window manager throws away your drawing when
     * the window comes to the background, and expects you will redraw it when
     * the window comes back (by sending a paint event).
     * 
     * In most cases, this will not be needed at all; simply handling paint events
     * and calling Refresh() when a refresh is needed will do the job.
     **/
    void PaintNow();

    /**
     * Here we do the actual rendering. I put it in a separate method so that it
     * can work no matter what type of DC (e.g. wxPaintDC or wxClientDC) is used.
     **/
    void Render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};
