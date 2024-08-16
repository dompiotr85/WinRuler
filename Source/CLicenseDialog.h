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
#include <wx/richtext/richtextctrl.h>

namespace WinRuler
{
    class CLicenseDialog :
        public wxDialog
    {
        DECLARE_EVENT_TABLE()
    public:
		/**
		 * Constructor with default parameters.
		 *
		 * @param Parent	Pointer to parent instance.
		 * @param Id		Window Id. Default: wxID_ANY
		 * @param Title		Reference to dialog title string.
		 *					Default: "MIT License"
		 * @param Pos		Dialog position. Default: wxDefaultPosition
		 * @param Size		Dialog size. Default: wxSize(500, 320)
		 * @param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL
		 **/
		CLicenseDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("MIT License"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxSize(500, 320),
			long Style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~CLicenseDialog();

		/**
		 * Initialize about dialog.
		 **/
		void Init();

		/**
		 * Creates all controls for this about dialog.
		 **/
		void CreateControls();
	private:
		/**
		 * OnClose() method event.
		 *
		 * @param Event		Reference to wxCloseEvent instance.
		 **/
		void OnClose(wxCloseEvent& Event);
	public:
		// Content panel.
		wxPanel* m_pContentPanel;

		// Rich text control.
		wxRichTextCtrl* m_pTextCtrl;

		// Close button.
		wxButton* m_pCloseButton;
    };
} // end namespace WinRuler
