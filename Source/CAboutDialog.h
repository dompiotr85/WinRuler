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
#include "CLicenseDialog.h"

namespace WinRuler
{
	class CAboutDialog :
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
		 *					Default: "About WinRuler"
		 * @param Pos		Dialog position. Default: wxDefaultPosition
		 * @param Size		Dialog size. Default: wxSize(600, 440)
		 * @param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL
		 **/
		CAboutDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("About WinRuler"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxSize(600, 440),
			long Style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~CAboutDialog();

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

		void OnLicenseButtonClicked(wxCommandEvent& Event);
	private:
		// Content panel.
		wxPanel* m_pContentPanel;

		// Header static bitmap.
		wxStaticBitmap* m_pHeaderStaticBitmap;

		// Version static text.
		wxStaticText* m_pVersionStaticText;

		// Copyright static text.
		wxStaticText* m_pCopyrightStaticText;

		// License button.
		wxButton* m_pLicenseButton;

		// CLicenseDialog instance.
		CLicenseDialog* m_pLicenseDialog;

		// Close button.
		wxButton* m_pCloseButton;
	};
} // end namespace WinRuler