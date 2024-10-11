/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "CNewRulerLengthDialog.h"
#include "CMainFrame.h"

namespace WinRuler
{
    BEGIN_EVENT_TABLE(CNewRulerLengthDialog, wxDialog)
    EVT_CLOSE(CNewRulerLengthDialog::OnClose)
    EVT_BUTTON(wxID_OK, CNewRulerLengthDialog::OnOKButtonClicked)
    END_EVENT_TABLE()

    CNewRulerLengthDialog::CNewRulerLengthDialog(
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

        // Centre dialog.
        Centre();
    }

    CNewRulerLengthDialog::~CNewRulerLengthDialog()
    {
        // Release all instances from heap.
        wxDELETE(m_pOKButton);
        wxDELETE(m_pSpinCtrl);
        wxDELETE(m_pChooseStaticText);
    }

    void CNewRulerLengthDialog::Init()
    {
        // Set all class instances as NULL.
        m_pChooseStaticText = NULL;
        m_pSpinCtrl = NULL;
        m_pOKButton = NULL;
    }

    void CNewRulerLengthDialog::CreateControls()
    {
        // Retrieve our MainFrame.
        CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

        // Set client size.
        SetClientSize(400, 200);

        // Create Choose static text.
        m_pChooseStaticText =
            new wxStaticText(
                this, wxID_ANY,
                wxString("Please enter the new length of the ruler (in pixels):"));

        // Create Spin control.
        m_pSpinCtrl =
            new wxSpinCtrl(
                this, wxID_ANY,
                wxString::Format(wxT("%d"), (int) pMainFrame->m_iRulerLength),
                wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
                pMainFrame->m_iRulerMinimumLengthLimit, 2000);

        // Create OK button.
        m_pOKButton = new wxButton(this, wxID_OK, "&OK");
    }

    void CNewRulerLengthDialog::SetupSizers()
    {
        // Create wxBoxSizer and fit all components.
        wxBoxSizer* pBoxSizer = new wxBoxSizer(wxVERTICAL);

        pBoxSizer->Add(m_pChooseStaticText, 0, wxEXPAND | wxALL, 5);
        pBoxSizer->Add(m_pSpinCtrl, 1, wxEXPAND | wxALL, 5);
        pBoxSizer->AddSpacer(8);
        pBoxSizer->Add(m_pOKButton, 0, wxEXPAND | wxALL, 5);

        SetSizerAndFit(pBoxSizer);
    }

    void CNewRulerLengthDialog::OnOKButtonClicked(wxCommandEvent& WXUNUSED(Event))
    {
        // Retrieve CMainFrame instance.
        CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

        // Change ruler's length.
        pMainFrame->ChangeRulerLength(m_pSpinCtrl->GetValue());

        // Close this dialog.
        Close(true);
    }

    void CNewRulerLengthDialog::OnClose(wxCloseEvent& Event)
    {
        // Destroy class.
        Destroy();
    }
} // end namespace WinRuler
