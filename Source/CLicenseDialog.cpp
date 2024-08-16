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

#include "CLicenseDialog.h"

namespace WinRuler
{
	BEGIN_EVENT_TABLE(CLicenseDialog, wxDialog)
	EVT_CLOSE(CLicenseDialog::OnClose)
	END_EVENT_TABLE()

	CLicenseDialog::CLicenseDialog(
		wxWindow* Parent, wxWindowID Id,
		const wxString& Title, const wxPoint& Pos, const wxSize& Size,
		long Style) :
		wxDialog(Parent, Id, Title, Pos, Size, Style)
	{
		// Initialize class.
		Init();

		// Create class controls.
		CreateControls();

		// Centre dialog.
		Centre();
	}

	CLicenseDialog::~CLicenseDialog()
	{
		// Release all instances from heap.
		wxDELETE(m_pCloseButton);
		wxDELETE(m_pTextCtrl);
		wxDELETE(m_pContentPanel);
	}

	void CLicenseDialog::Init()
	{
		// Set all class instances as NULL.
		m_pContentPanel = NULL;
		m_pCloseButton = NULL;
	}

	void CLicenseDialog::CreateControls()
	{
		// Retrieve our CLicenseDialog.
		CLicenseDialog* pDialog = this;

		// Create m_pContentPanel.
		m_pContentPanel =
			new wxPanel(
					pDialog, wxID_ANY, wxDefaultPosition,
					wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL);

		// Create m_pTextCtrl and load it content from LICENSE file.
		m_pTextCtrl =
			new wxRichTextCtrl(
					pDialog, wxID_ANY, wxEmptyString,
					wxPoint(10, 10), wxSize(470, 215),
					wxRE_MULTILINE | wxRE_READONLY);
		if (!m_pTextCtrl->LoadFile(
				wxString("../../../../LICENSE"), wxRICHTEXT_TYPE_TEXT))
		{
			wxMessageBox(
				wxString("Can''t load LICENSE file!"),
				wxString("WinRuler - Error"));
		}

		// Create m_pCloseButton.
		m_pCloseButton =
			new wxButton(
					pDialog, wxID_OK, wxString("&Close"),
					wxPoint(420, 235),
					wxSize(60, 30));
	}

	void CLicenseDialog::OnClose(wxCloseEvent& Event)
	{
		// Destroy class.
		Destroy();
	}
} // end namespace WinRuler
