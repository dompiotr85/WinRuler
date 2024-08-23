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

#include "COptionsDialog.h"
#include "CMainFrame.h"

namespace WinRuler
{
	BEGIN_EVENT_TABLE(COptionsDialog, wxDialog)
	EVT_CLOSE(COptionsDialog::OnClose)
	EVT_CHOICE(ID_BackgroundTypeChoice, COptionsDialog::OnBackgroundTypeChoiceChanged)
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
		wxDELETE(m_pBackgroundImagePicker);
		wxDELETE(m_pBackgroundColour);
		wxDELETE(m_pBackgroundStartColour);
		wxDELETE(m_pBackgroundEndColour);
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
		m_pBackgroundColour = NULL;
		m_pBackgroundStartColour = NULL;
		m_pBackgroundEndColour = NULL;
		m_pBackgroundImagePicker = NULL;
	}

	void COptionsDialog::CreateControls()
	{
		// Retrieve pointer to CMainFrame.
		CMainFrame* pMainFrame = (CMainFrame*)this->GetParent();

		// Set client size.
		SetClientSize(wxSize(700, 560));

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

		// Create Ruler BackgroundTypeChoice.
		wxArrayString Choices;
		Choices.Add(wxString("Solid"));
		Choices.Add(wxString("Gradient"));
		Choices.Add(wxString("Image"));

		wxStaticText* pBackgroundTypeText =
			new wxStaticText(pRulerPanel, wxID_ANY, wxString("Ruler background type:"));
		m_pBackgroundTypeChoice =
			new wxChoice(pRulerPanel, ID_BackgroundTypeChoice, wxDefaultPosition, wxDefaultSize, Choices);
		// Select current Ruler Background Type.
		m_pBackgroundTypeChoice->Select(pMainFrame->m_eRulerBackgroundType);

		// Create Ruler Background Colours.
		m_pBackgroundColourText =
			new wxStaticText(pRulerPanel, wxID_ANY, wxString("Ruler background colour:"));
		m_pBackgroundColour =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerBackgroundColour,
				wxDefaultPosition, wxDefaultSize, wxCLRP_SHOW_LABEL);

		// Create Ruler Background Start and End Colour.
		m_pBackgroundStartEndColourText =
			new wxStaticText(pRulerPanel, wxID_ANY, wxString("Ruler background start and end colour:"));
		m_pBackgroundStartColour =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerBackgroundStartColour,
				wxDefaultPosition, wxDefaultSize, wxCLRP_SHOW_LABEL);
		m_pBackgroundEndColour =
			new wxColourPickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_cRulerBackgroundEndColour,
				wxDefaultPosition, wxDefaultSize, wxCLRP_SHOW_LABEL);

		// Create Ruler Background Image.
		m_pBackgroundImageText =
			new wxStaticText(
				pRulerPanel, wxID_ANY, wxString("Ruler background image:"));
		m_pBackgroundImagePicker =
			new wxFilePickerCtrl(
				pRulerPanel, wxID_ANY, pMainFrame->m_sRulerBackgroundImage);

		// Create new wxBoxSizer and apply this sizer to our items.
		wxBoxSizer* pBoxSizer3 = new wxBoxSizer(wxVERTICAL);
		pBoxSizer3->Add(pBackgroundTypeText, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->Add(m_pBackgroundTypeChoice, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->AddSpacer(5);
		pBoxSizer3->Add(m_pBackgroundColourText, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->Add(m_pBackgroundColour, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->AddSpacer(5);
		pBoxSizer3->Add(m_pBackgroundStartEndColourText, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->Add(m_pBackgroundStartColour, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->Add(m_pBackgroundEndColour, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->AddSpacer(5);
		pBoxSizer3->Add(m_pBackgroundImageText, 0, wxEXPAND | wxALL, 5);
		pBoxSizer3->Add(m_pBackgroundImagePicker, 0, wxEXPAND | wxALL, 5);

		pRulerPanel->SetSizerAndFit(pBoxSizer3);

		// At end of Ruler page creation, enable proper items depending on
		// current settings.
		switch (pMainFrame->m_eRulerBackgroundType)
		{
		case CMainFrame::btSolid:
			m_pBackgroundColourText->Enable(true);
			m_pBackgroundColour->Enable(true);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColour->Enable(false);
			m_pBackgroundEndColour->Enable(false);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btGradient:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColour->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColour->Enable(true);
			m_pBackgroundEndColour->Enable(true);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btImage:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColour->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColour->Enable(false);
			m_pBackgroundEndColour->Enable(false);
			m_pBackgroundImageText->Enable(true);
			m_pBackgroundImagePicker->Enable(true);

			break;
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
		wxBoxSizer* pBoxSizer2 = new wxBoxSizer(wxVERTICAL);

		pBoxSizer2->Add(m_pNotebook, 1, wxEXPAND | wxALL, 5);
		pBoxSizer2->Add(m_pBottomPanel, 0, wxEXPAND | wxALL, 5);

		SetSizerAndFit(pBoxSizer2);
	}

	void COptionsDialog::OnBackgroundTypeChoiceChanged(wxCommandEvent& Event)
	{
		switch (Event.GetSelection())
		{
		case CMainFrame::btSolid:
			m_pBackgroundColourText->Enable(true);
			m_pBackgroundColour->Enable(true);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColour->Enable(false);
			m_pBackgroundEndColour->Enable(false);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btGradient:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColour->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColour->Enable(true);
			m_pBackgroundEndColour->Enable(true);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case CMainFrame::btImage:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColour->Enable(false);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColour->Enable(false);
			m_pBackgroundEndColour->Enable(false);
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
