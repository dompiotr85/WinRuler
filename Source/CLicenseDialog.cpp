/**
 * Copyright © 2024-2025 Piotr Domanski
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

		// Setup sizers.
		SetupSizers();

		// Centre license dialog.
		Centre();
	}

	CLicenseDialog::~CLicenseDialog()
	{
		// Release all instances from heap.
		wxDELETE(m_pCloseButton);
		wxDELETE(m_pTextCtrl);
	}

	void CLicenseDialog::Init()
	{
		// Set all class instances as NULL.
		m_pTextCtrl = NULL;
		m_pCloseButton = NULL;
	}

	void CLicenseDialog::CreateControls()
	{
	    // Set client size.
	    SetClientSize(wxSize(450, 300));

		// Create m_pTextCtrl and load it content from LICENSE file.
		m_pTextCtrl =
			new wxRichTextCtrl(
				this, wxID_ANY, wxEmptyString,
				wxDefaultPosition, wxSize(450, 200),
				wxRE_MULTILINE | wxRE_READONLY);
		if (!m_pTextCtrl->LoadFile(
				wxString("../../../../LICENSE"), wxRICHTEXT_TYPE_TEXT))
		{
			wxLogError("Can not load LICENSE file!");
		}

		// Create m_pCloseButton.
		m_pCloseButton = new wxButton(this, wxID_OK, wxString("&Close"));
	}

	void CLicenseDialog::SetupSizers()
	{
		// Create wxBoxSizer and fit all components.
		wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);

		pSizer->Add(
			m_pTextCtrl,
			wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5));
		pSizer->Add(
			m_pCloseButton,
			wxSizerFlags().Centre().Border(wxALL, 5));

		SetSizerAndFit(pSizer);
	}

	void CLicenseDialog::OnClose(wxCloseEvent& Event)
	{
		// Destroy class.
		Destroy();
	}
} // end namespace WinRuler
