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

		// Setup sizers.
		SetupSizers();

		// Centre dialog.
		Centre();
	}

	COptionsDialog::~COptionsDialog()
	{
		// Release all instances from heap. The release order is important.
		wxDELETE(m_pCalibrateStaticBox);

		wxDELETE(m_pRulerTransparencyCheckBox);
		wxDELETE(m_pRulerTransparencyText);
		wxDELETE(m_pRulerTransparencySlider);
		wxDELETE(m_pSpecialOptionsStaticBox);

		wxDELETE(m_pRulerScaleColourText);
		wxDELETE(m_pRulerScaleColourPicker);
		wxDELETE(m_pRulerFirstMarkerColourText);
		wxDELETE(m_pRulerSecondMarkerColourText);
		wxDELETE(m_pFirstMarkerColourPicker);
		wxDELETE(m_pSecondMarkerColourPicker);
		wxDELETE(m_pScaleAndMarkersStaticBox);

		wxDELETE(m_pBackgroundTypeText);
		wxDELETE(m_pBackgroundTypeChoice);
		wxDELETE(m_pBackgroundColourText);
		wxDELETE(m_pBackgroundStartEndColourText);
		wxDELETE(m_pBackgroundImageText);
		wxDELETE(m_pBackgroundColourPicker);
		wxDELETE(m_pBackgroundStartColourPicker);
		wxDELETE(m_pBackgroundEndColourPicker);
		wxDELETE(m_pBackgroundImagePicker);
		wxDELETE(m_pBackgroundStaticBox);

		wxDELETE(m_pRulerPanel);
		wxDELETE(m_pUOMPanel);

		wxDELETE(m_pOKButton);
		wxDELETE(m_pBottomPanel);

		wxDELETE(m_pNotebook);
	}

	void COptionsDialog::Init()
	{
		m_pRulerPanel = NULL;
		m_pUOMPanel = NULL;
		m_pBottomPanel = NULL;
		m_pOKButton = NULL;
		m_pNotebook = NULL;
		m_pBackgroundStaticBox = NULL;
		m_pBackgroundTypeText = NULL;
		m_pBackgroundTypeChoice = NULL;
		m_pBackgroundColourText = NULL;
		m_pBackgroundStartEndColourText = NULL;
		m_pBackgroundImageText = NULL;
		m_pBackgroundColourPicker = NULL;
		m_pBackgroundStartColourPicker = NULL;
		m_pBackgroundEndColourPicker = NULL;
		m_pBackgroundImagePicker = NULL;
		m_pScaleAndMarkersStaticBox = NULL;
		m_pRulerScaleColourText = NULL;
		m_pRulerScaleColourPicker = NULL;
		m_pRulerFirstMarkerColourText = NULL;
		m_pRulerSecondMarkerColourText = NULL;
		m_pFirstMarkerColourPicker = NULL;
		m_pSecondMarkerColourPicker = NULL;
		m_pSpecialOptionsStaticBox = NULL;
		m_pRulerTransparencyCheckBox = NULL;
		m_pRulerTransparencyText = NULL;
		m_pRulerTransparencySlider = NULL;
		m_pCalibrateStaticBox = NULL;
	}

	void COptionsDialog::CreateControls()
	{
		// Retrieve pointer to CMainFrame.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Set client size.
		// @todo	Right now code below is a dirty hack. Implementation of
		//			OS and platform detection need to be done.
#ifdef __unix__	// If platform is Linux.
		SetClientSize(wxSize(700, 760));
#elif defined(_WIN32) || defined(WIN32)	// If platform in Windows.
		SetClientSize(wxSize(700, 660));
#endif

		// Create Notebook.
		m_pNotebook =
			new wxNotebook(
				this, wxID_ANY,
				wxDefaultPosition,
				wxSize(GetClientSize().GetWidth(), GetClientSize().GetHeight()),
				wxNB_TOP);

		// Create Notebook pages.
		m_pRulerPanel = new wxPanel(m_pNotebook, wxID_ANY);
		m_pUOMPanel = new wxPanel(m_pNotebook, wxID_ANY);

		///////////////////////////////////////////////////////////////////////
		// Ruler page.
		//

		// Create ruler background StaticBox.
		m_pBackgroundStaticBox =
			new wxStaticBox(
				m_pRulerPanel, wxID_ANY, wxString("Background"));

		// Create ruler background type choice.
		wxArrayString Choices;
		Choices.Add(wxString("Solid"));
		Choices.Add(wxString("Gradient"));
		Choices.Add(wxString("Image"));

		m_pBackgroundTypeText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background type:"));
		m_pBackgroundTypeChoice =
			new wxChoice(
				m_pBackgroundStaticBox, ID_BackgroundTypeChoice,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1),
				Choices);

		// Select current ruler background type.
		m_pBackgroundTypeChoice->Select(pMainFrame->m_eRulerBackgroundType);

		// Create ruler background colours picker.
		m_pBackgroundColourText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background colour:"));
		m_pBackgroundColourPicker =
			new wxColourPickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_cRulerBackgroundColour,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1),
				wxCLRP_SHOW_LABEL);

		// Create ruler background start and end colour picker.
		m_pBackgroundStartEndColourText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background start and end colour:"));
		m_pBackgroundStartColourPicker =
			new wxColourPickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_cRulerBackgroundStartColour,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1),
				wxCLRP_SHOW_LABEL);
		m_pBackgroundEndColourPicker =
			new wxColourPickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_cRulerBackgroundEndColour,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1),
				wxCLRP_SHOW_LABEL);

		// Create ruler background image picker.
		m_pBackgroundImageText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background image:"));
		m_pBackgroundImagePicker =
			new wxFilePickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_sRulerBackgroundImagePath);

		m_pScaleAndMarkersStaticBox =
			new wxStaticBox(
				m_pRulerPanel, wxID_ANY, wxString("Scale and markers"));

		// Create ruler scale colour picker.
		m_pRulerScaleColourText =
			new wxStaticText(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				wxString("Ruler scale colour:"));
		m_pRulerScaleColourPicker =
			new wxColourPickerCtrl(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				pMainFrame->m_cRulerScaleColour,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1));

		// Create ruler first and second marker colour.
		m_pRulerFirstMarkerColourText =
			new wxStaticText(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				wxString("Ruler first marker colour:"));
		m_pFirstMarkerColourPicker =
			new wxColourPickerCtrl(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				pMainFrame->m_cFirstMarkerColour,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1));

		m_pRulerSecondMarkerColourText =
			new wxStaticText(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				wxString("Ruler second marker colour:"));
		m_pSecondMarkerColourPicker =
			new wxColourPickerCtrl(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				pMainFrame->m_cSecondMarkerColour,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1));

		// Create wxStaticBox for Special options.
		m_pSpecialOptionsStaticBox =
			new wxStaticBox(
				m_pRulerPanel, wxID_ANY, wxString("Special options"));

		// Create new ruler transparency checkbox.
		m_pRulerTransparencyCheckBox =
			new wxCheckBox(
				m_pSpecialOptionsStaticBox, ID_RulerTransparency,
				wxString("Ruler transparency"));
		m_pRulerTransparencyCheckBox->SetValue(
			pMainFrame->m_bRulerTransparency);

		// Create new ruler transparency slider.
		m_pRulerTransparencyText =
			new wxStaticText(
				m_pSpecialOptionsStaticBox, wxID_ANY,
				wxString("Ruler transparency value:"));

		m_pRulerTransparencySlider =
			new wxSlider(
				m_pSpecialOptionsStaticBox, wxID_ANY,
				static_cast<int>(pMainFrame->m_iRulerTransparencyValue), 0, 255,
				wxDefaultPosition, wxSize(GetClientSize().GetWidth(), -1),
				wxSL_HORIZONTAL | wxSL_VALUE_LABEL);

		// At the end of ruler page creation, enable proper items depending on
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

		///////////////////////////////////////////////////////////////////////
		// Units of measurement page.
		//

		// Create Calibrate static box.
		m_pCalibrateStaticBox =
			new wxStaticBox(m_pUOMPanel, wxID_ANY, wxString("Calibrate"));

		///////////////////////////////////////////////////////////////////////

		// Add created notebook pages to notebook.
		m_pNotebook->AddPage(m_pRulerPanel, wxString("Ruler"));
		m_pNotebook->AddPage(m_pUOMPanel, wxString("Units of measurement"));

		// Add BottomPanel and OKButton.
		m_pBottomPanel = new wxPanel(this, wxID_ANY);
		m_pOKButton = new wxButton(m_pBottomPanel, wxID_OK, wxString("&OK"));
	}

	void COptionsDialog::SetupSizers()
	{
		///////////////////////////////////////////////////////////////////////
		// Ruler page.
		//

		// Create wxBoxSizer and apply it on all components inside
		// m_pBackgroundStaticBox.
		wxBoxSizer* pBackgroundBoxSizer = new wxBoxSizer(wxVERTICAL);

		pBackgroundBoxSizer->AddSpacer(20);
		pBackgroundBoxSizer->Add(
			m_pBackgroundTypeText, 0, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundTypeChoice, 1, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->AddSpacer(5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundColourText, 0, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundColourPicker, 1, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->AddSpacer(5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundStartEndColourText, 0, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundStartColourPicker, 1, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundEndColourPicker, 1, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->AddSpacer(5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundImageText, 0, wxEXPAND | wxALL, 5);
		pBackgroundBoxSizer->Add(
			m_pBackgroundImagePicker, 1, wxEXPAND | wxALL, 5);

		m_pBackgroundStaticBox->SetSizerAndFit(pBackgroundBoxSizer);

		// Create wxBoxSizer and apply it on all components inside
		// m_pRulerStaticBox.
		wxBoxSizer* pScaleAndMarkersBoxSizer = new wxBoxSizer(wxVERTICAL);

		pScaleAndMarkersBoxSizer->AddSpacer(20);
		pScaleAndMarkersBoxSizer->Add(
			m_pRulerScaleColourText, 0, wxEXPAND | wxALL, 5);
		pScaleAndMarkersBoxSizer->Add(
			m_pRulerScaleColourPicker, 1, wxEXPAND | wxALL, 5);
		pScaleAndMarkersBoxSizer->AddSpacer(5);
		pScaleAndMarkersBoxSizer->Add(
			m_pRulerFirstMarkerColourText, 0, wxEXPAND | wxALL, 5);
		pScaleAndMarkersBoxSizer->Add(
			m_pFirstMarkerColourPicker, 1, wxEXPAND | wxALL, 5);
		pScaleAndMarkersBoxSizer->Add(
			m_pRulerSecondMarkerColourText, 0, wxEXPAND | wxALL, 5);
		pScaleAndMarkersBoxSizer->Add(
			m_pSecondMarkerColourPicker, 1, wxEXPAND | wxALL, 5);

		m_pScaleAndMarkersStaticBox->SetSizerAndFit(pScaleAndMarkersBoxSizer);

		// Create new wxBoxSizer and apply it on all components inside
		// m_pSpecialOptionsStaticBox.
		wxBoxSizer* pSpecialOptionsBoxSizer = new wxBoxSizer(wxVERTICAL);

		pSpecialOptionsBoxSizer->AddSpacer(20);
		pSpecialOptionsBoxSizer->Add(
			m_pRulerTransparencyCheckBox, 0, wxEXPAND | wxALL, 5);
		pSpecialOptionsBoxSizer->Add(
			m_pRulerTransparencyText, 0, wxEXPAND | wxALL, 5);
		pSpecialOptionsBoxSizer->Add(
			m_pRulerTransparencySlider, 1, wxEXPAND | wxALL, 5);

		m_pSpecialOptionsStaticBox->SetSizerAndFit(pSpecialOptionsBoxSizer);

		// Create wxBoxSizer and fit all static boxes on pRulerPanel.
		wxBoxSizer* pStaticBoxSizer = new wxBoxSizer(wxVERTICAL);

		pStaticBoxSizer->Add(
			m_pBackgroundStaticBox, 1, wxEXPAND || wxALL, 5);
		pStaticBoxSizer->Add(
			m_pScaleAndMarkersStaticBox, 1, wxEXPAND || wxALL, 5);
		pStaticBoxSizer->Add(
			m_pSpecialOptionsStaticBox, 1, wxEXPAND || wxALL, 5);

		m_pRulerPanel->SetSizerAndFit(pStaticBoxSizer);

		///////////////////////////////////////////////////////////////////////
		// Unit of measurement page.
		//

		// Create wxBoxSizer and fit all static boxes on pUOMPanel.
		wxBoxSizer* pStaticBoxSizer2 = new wxBoxSizer(wxVERTICAL);

		pStaticBoxSizer2->Add(m_pCalibrateStaticBox, 1, wxEXPAND || wxALL, 5);

		m_pUOMPanel->SetSizerAndFit(pStaticBoxSizer2);

		///////////////////////////////////////////////////////////////////////

		// Create wxBoxSizer and fit our Notebook and Bottom panel.
		wxBoxSizer* pBoxSizer = new wxBoxSizer(wxVERTICAL);

		pBoxSizer->Add(m_pNotebook, 1, wxEXPAND | wxALL, 5);
		pBoxSizer->Add(m_pBottomPanel, 0, wxEXPAND | wxALL, 5);

		SetSizerAndFit(pBoxSizer);

		// Centre OK button.
		m_pOKButton->Centre();
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
