/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "COptionsDialog.h"
#include "CMainFrame.h"

namespace WinRuler
{
	BEGIN_EVENT_TABLE(COptionsDialog, wxDialog)
	EVT_CLOSE(COptionsDialog::OnClose)
	EVT_CHOICE(ID_BackgroundTypeChoice, COptionsDialog::OnBackgroundTypeChoiceChanged)
	EVT_CHECKBOX(ID_RulerTransparency, COptionsDialog::OnRulerTransparencyCheckBoxClicked)
	END_EVENT_TABLE()

	COptionsDialog::COptionsDialog(
		wxWindow* Parent, wxWindowID Id, const wxString& Title,
		const wxPoint& Pos, const wxSize& Size, long Style) :
		wxDialog(Parent, Id, Title, Pos, Size, Style)
	{
		// Initialize class.
		Init();

		// Create class controls.
		CreateControls();

		// Centre dialog.
		Centre();
	}

	COptionsDialog::~COptionsDialog()
	{
		// Release all instances from heap.
		wxDELETE(m_pRulerTransparencySlider);
		wxDELETE(m_pRulerTransparencyText);
		wxDELETE(m_pRulerTransparencyCheckBox);
		wxDELETE(m_pFirstMarkerColourPicker);
		wxDELETE(m_pSecondMarkerColourPicker);
		wxDELETE(m_pRulerScaleColourPicker);
		wxDELETE(m_pBackgroundImagePicker);
		wxDELETE(m_pBackgroundColourPicker);
		wxDELETE(m_pBackgroundStartColourPicker);
		wxDELETE(m_pBackgroundEndColourPicker);
		wxDELETE(m_pBackgroundColourText);
		wxDELETE(m_pBackgroundStartEndColourText);
		wxDELETE(m_pBackgroundImageText);
		wxDELETE(m_pBackgroundTypeChoice);
		wxDELETE(m_pNotebook);
		wxDELETE(m_pOKButton);
		wxDELETE(m_pBottomPanel);
	}

	void COptionsDialog::Init()
	{
		// Set all class instances as NULL.
		m_pBottomPanel = NULL;
		m_pOKButton = NULL;
		m_pNotebook = NULL;
		m_pBackgroundTypeChoice = NULL;
		m_pBackgroundColourText = NULL;
		m_pBackgroundStartEndColourText = NULL;
		m_pBackgroundImageText = NULL;
		m_pBackgroundColourPicker = NULL;
		m_pBackgroundStartColourPicker = NULL;
		m_pBackgroundEndColourPicker = NULL;
		m_pBackgroundImagePicker = NULL;
		m_pRulerScaleColourPicker = NULL;
		m_pFirstMarkerColourPicker = NULL;
		m_pSecondMarkerColourPicker = NULL;
		m_pRulerTransparencyCheckBox = NULL;
		m_pRulerTransparencyText = NULL;
		m_pRulerTransparencySlider = NULL;
	}

	void COptionsDialog::CreateControls()
	{
		// Retrieve pointer to CMainFrame.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Set client size.
		SetClientSize(wxSize(700, 660));

		// Create Notebook.
		m_pNotebook =
			new wxNotebook(
				this, wxID_ANY, wxPoint(0, 0),
				wxSize(
					GetClientSize().GetWidth(),
					GetClientSize().GetHeight() - 40),
				wxNB_TOP);

		// Create Notebook pages.
		wxPanel* pRulerPanel = new wxPanel(m_pNotebook, wxID_ANY);
		wxPanel* pUnitOfMeasurementPanel = new wxPanel(m_pNotebook, wxID_ANY);

		//////////////////////
		// Create Ruler page.

		// Create ruler background type choice.
		wxArrayString Choices;
		Choices.Add(wxString("Solid"));
		Choices.Add(wxString("Gradient"));
		Choices.Add(wxString("Image"));

		wxStaticText* pBackgroundTypeText =
			new wxStaticText(
				pRulerPanel, wxID_ANY, wxString("Ruler background type:"));
		m_pBackgroundTypeChoice =
			new wxChoice(
				pRulerPanel, ID_BackgroundTypeChoice,
				wxDefaultPosition, wxDefaultSize, Choices);
		// Select current ruler background type.
		m_pBackgroundTypeChoice->Select(pMainFrame->m_eRulerBackgroundType);

		// Create ruler background colours picker.
		m_pBackgroundColourText =
			new wxStaticText(pRulerPanel, wxID_ANY, wxString("Ruler background colour:"));
		m_pBackgroundColourPicker =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerBackgroundColour,
				wxDefaultPosition, wxDefaultSize, wxCLRP_SHOW_LABEL);

		// Create ruler background start and end colour picker.
		m_pBackgroundStartEndColourText =
			new wxStaticText(
				pRulerPanel, wxID_ANY,
				wxString("Ruler background start and end colour:"));
		m_pBackgroundStartColourPicker =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerBackgroundStartColour,
				wxDefaultPosition, wxDefaultSize, wxCLRP_SHOW_LABEL);
		m_pBackgroundEndColourPicker =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerBackgroundEndColour,
				wxDefaultPosition, wxDefaultSize, wxCLRP_SHOW_LABEL);

		// Create ruler background image picker.
		m_pBackgroundImageText =
			new wxStaticText(
				pRulerPanel, wxID_ANY, wxString("Ruler background image:"));
		m_pBackgroundImagePicker =
			new wxFilePickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_sRulerBackgroundImagePath);

		// Create ruler scale colour picker.
		wxStaticText* pRulerScaleColourText =
			new wxStaticText(
				pRulerPanel, wxID_ANY, wxString("Ruler scale colour:"));
		m_pRulerScaleColourPicker =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerScaleColour);

		// Create ruler first and second marker colour.
		wxStaticText* pRulerFirstMarkerColourText =
			new wxStaticText(
				pRulerPanel, wxID_ANY, wxString("Ruler first marker colour:"));
		m_pFirstMarkerColourPicker =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cFirstMarkerColour);

		wxStaticText* pRulerSecondMarkerColourText =
			new wxStaticText(
				pRulerPanel, wxID_ANY, wxString("Ruler second marker colour:"));
		m_pSecondMarkerColourPicker =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cSecondMarkerColour);

		// Create new ruler transparency checkbox.
		m_pRulerTransparencyCheckBox =
			new wxCheckBox(pRulerPanel, ID_RulerTransparency, wxString("Ruler transparency"));
		m_pRulerTransparencyCheckBox->SetValue(pMainFrame->m_bRulerTransparency);

		// Create new ruler transparency slider.
		m_pRulerTransparencyText =
			new wxStaticText(pRulerPanel, wxID_ANY, wxString("Ruler transparency value:"));

		m_pRulerTransparencySlider =
			new wxSlider(
				pRulerPanel, wxID_ANY, 
				static_cast<int>(pMainFrame->m_iRulerTransparencyValue), 0, 255,
				wxDefaultPosition, wxDefaultSize,
				wxSL_HORIZONTAL | wxSL_VALUE_LABEL);

		// Create new wxBoxSizer and apply this sizer to our items.
		wxBoxSizer* pRulerPanelBoxSizer = new wxBoxSizer(wxVERTICAL);

		pRulerPanelBoxSizer->Add(pBackgroundTypeText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pBackgroundTypeChoice, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->AddSpacer(5);
		pRulerPanelBoxSizer->Add(m_pBackgroundColourText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pBackgroundColourPicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->AddSpacer(5);
		pRulerPanelBoxSizer->Add(m_pBackgroundStartEndColourText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pBackgroundStartColourPicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pBackgroundEndColourPicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->AddSpacer(5);
		pRulerPanelBoxSizer->Add(m_pBackgroundImageText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pBackgroundImagePicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->AddSpacer(5);
		pRulerPanelBoxSizer->Add(pRulerScaleColourText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pRulerScaleColourPicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->AddSpacer(5);
		pRulerPanelBoxSizer->Add(pRulerFirstMarkerColourText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pFirstMarkerColourPicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(pRulerSecondMarkerColourText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pSecondMarkerColourPicker, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->AddSpacer(5);
		pRulerPanelBoxSizer->Add(m_pRulerTransparencyCheckBox, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pRulerTransparencyText, 0, wxEXPAND | wxALL, 5);
		pRulerPanelBoxSizer->Add(m_pRulerTransparencySlider, 0, wxEXPAND | wxALL, 5);

		pRulerPanel->SetSizerAndFit(pRulerPanelBoxSizer);

		// At end of Ruler page creation, enable proper items depending on
		// current settings.
		switch (pMainFrame->m_eRulerBackgroundType)
		{
		case CMainFrame::btSolid:
			m_pBackgroundColourText->Enable(true);
			m_pBackgroundColourPicker->Enable(true);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btGradient:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColourPicker->Enable(true);
			m_pBackgroundEndColourPicker->Enable(true);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btImage:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(true);
			m_pBackgroundImagePicker->Enable(true);

			break;
		}

		if (m_pRulerTransparencyCheckBox->IsChecked())
		{
			m_pRulerTransparencyText->Enable(true);
			m_pRulerTransparencySlider->Enable(true);
		}
		else
		{
			m_pRulerTransparencyText->Enable(false);
			m_pRulerTransparencySlider->Enable(false);
		}

		//////////////////////

		// Add created notebook pages to notebook.
		m_pNotebook->AddPage(
			pRulerPanel, wxString("Ruler"));
		m_pNotebook->AddPage(
			pUnitOfMeasurementPanel, wxString("Units of measurement"));

		// Add BottomPanel and OKButton.
		m_pBottomPanel =
			new wxPanel(
					this, wxID_ANY,
					wxDefaultPosition,
					wxSize(GetClientSize().GetWidth(), 40));
		m_pOKButton = new wxButton(m_pBottomPanel, wxID_OK, wxString("&OK"));
		m_pOKButton->Centre();

		// Create another wxBoxSizer and set our items.
		wxBoxSizer* pBoxSizer = new wxBoxSizer(wxVERTICAL);

		pBoxSizer->Add(m_pNotebook, 1, wxEXPAND | wxALL, 5);
		pBoxSizer->Add(m_pBottomPanel, 0, wxEXPAND | wxALL, 5);

		SetSizerAndFit(pBoxSizer);
	}

	void COptionsDialog::OnRulerTransparencyCheckBoxClicked(wxCommandEvent& Event)
	{
		wxCheckBox* pCheckBox = static_cast<wxCheckBox*>(Event.GetEventObject());
		if (pCheckBox->IsChecked())
		{
			m_pRulerTransparencyText->Enable(true);
			m_pRulerTransparencySlider->Enable(true);
		}
		else
		{
			m_pRulerTransparencyText->Enable(false);
			m_pRulerTransparencySlider->Enable(false);
		}
	}

	void COptionsDialog::OnBackgroundTypeChoiceChanged(wxCommandEvent& Event)
	{
		switch (Event.GetSelection())
		{
		case CMainFrame::btSolid:
			m_pBackgroundColourText->Enable(true);
			m_pBackgroundColourPicker->Enable(true);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btGradient:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColourPicker->Enable(true);
			m_pBackgroundEndColourPicker->Enable(true);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btImage:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(true);
			m_pBackgroundImagePicker->Enable(true);

			break;
		}
	}

	void COptionsDialog::OnClose(wxCloseEvent& Event)
	{
		// Destroy class.
		Destroy();
	}
} // end namespace WinRuler
