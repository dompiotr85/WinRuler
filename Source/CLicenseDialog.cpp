/**
 * Copyright © 2024 Piotr Domañski
 * Licensed under the MIT license.
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
		// Create m_pContentPanel.
		m_pContentPanel =
			new wxPanel(
					this, wxID_ANY, wxDefaultPosition,
					wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL);

		// Create m_pTextCtrl and load it content from LICENSE file.
		m_pTextCtrl =
			new wxRichTextCtrl(
					this, wxID_ANY, wxEmptyString,
					wxPoint(10, 10), wxSize(470, 215),
					wxRE_MULTILINE | wxRE_READONLY);
		if (!m_pTextCtrl->LoadFile(
				wxString("../../../../LICENSE"), wxRICHTEXT_TYPE_TEXT))
		{
			wxLogError("Can not load LICENSE file!");
		}

		// Create m_pCloseButton.
		m_pCloseButton =
			new wxButton(
					this, wxID_OK, wxString("&Close"),
					wxPoint(420, 235),
					wxSize(60, 30));
	}

	void CLicenseDialog::OnClose(wxCloseEvent& Event)
	{
		// Destroy class.
		Destroy();
	}
} // end namespace WinRuler
