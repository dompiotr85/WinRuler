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
#include "CDrawPanel.h"

BEGIN_EVENT_TABLE(CDrawPanel, wxPanel)
// Catch paint events.
EVT_PAINT(CDrawPanel::PaintEvent)
END_EVENT_TABLE()

CDrawPanel::CDrawPanel(wxFrame* parent) :
    wxPanel(parent)
{
    // No code.
}

void CDrawPanel::PaintEvent(wxPaintEvent& event)
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
    // Draw some text.
    dc.DrawText(wxT("Testing"), 40, 60);

    // Draw a circle.
    dc.SetBrush(*wxGREEN_BRUSH); // Green filling
    dc.SetPen(wxPen(wxColor(255, 0, 0), 5)); // 5-pixels-thick red outline.
    dc.DrawCircle(wxPoint(200, 100), 25 /* radius */);

    // Draw a rectangle.
    dc.SetBrush(*wxBLUE_BRUSH); // Blue filling.
    dc.SetPen(wxPen(wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline.
    dc.DrawRectangle(300, 100, 400, 200);

    // Draw a line.
    dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // Black line, 3 pixels thick
    dc.DrawLine(300, 100, 700, 300); // Draw line across the rectangle.

    // Look at the wxDC docs to learn how to draw other stuff.
}
