/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "CAboutDialog.h"

#define ID_LICENSE_BUTTON 2500

namespace WinRuler
{
    BEGIN_EVENT_TABLE(CAboutDialog, wxDialog)
    EVT_BUTTON(ID_LICENSE_BUTTON, CAboutDialog::OnLicenseButtonClicked)
    EVT_CLOSE(CAboutDialog::OnClose)
    END_EVENT_TABLE()

    CAboutDialog::CAboutDialog(
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

    CAboutDialog::~CAboutDialog()
    {
        // Release all instances from heap.
        wxDELETE(m_pCloseButton);
        wxDELETE(m_pLicenseDialog);
        wxDELETE(m_pLicenseButton);
        wxDELETE(m_pVersionStaticText);
        wxDELETE(m_pCopyrightStaticText);
        wxDELETE(m_pHeaderStaticBitmap);
    }

    void CAboutDialog::Init()
    {
        // Set all class instances as NULL.
        m_pHeaderStaticBitmap = NULL;
        m_pCopyrightStaticText = NULL;
        m_pVersionStaticText = NULL;
        m_pLicenseButton = NULL;
        m_pLicenseDialog = NULL;
        m_pCloseButton = NULL;
    }

    void CAboutDialog::CreateControls()
    {
        // Set client size.
        SetClientSize(wxSize(600, 500));

        // Create static bitmap on top.
        m_pHeaderStaticBitmap =
            new wxStaticBitmap(
                this, wxID_ANY,
                wxBitmap("../../../../Resources/About.png", wxBITMAP_TYPE_PNG),
                wxDefaultPosition, wxSize(600, 300), 0);

        // Create static texts.
        m_pVersionStaticText =
            new wxStaticText(
                    this, wxID_ANY,
                    wxString("Version 0.0.1"));
        m_pCopyrightStaticText =
            new wxStaticText(
                    this, wxID_ANY,
                    wxString("Copyright © 2024 Piotr Domanski and WinRuler programmers team."));

        // Create bottom panel.
        wxPanel* pBottomPanel =
            new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));

        // Create License button.
        m_pLicenseButton =
            new wxButton(pBottomPanel, ID_LICENSE_BUTTON, "&License");

        // Create Close button.
        m_pCloseButton =
            new wxButton(pBottomPanel, wxID_OK, "&Close");

        // Create 2 wxBoxSizer and set layout.
        wxBoxSizer* pSizer1 = new wxBoxSizer(wxVERTICAL);

        pSizer1->Add(m_pHeaderStaticBitmap, 0, wxEXPAND | wxALL, 5);
        pSizer1->Add(m_pVersionStaticText, 0, wxEXPAND | wxALL, 5);
        pSizer1->Add(m_pCopyrightStaticText, 0, wxEXPAND | wxALL, 5);
        pSizer1->Add(pBottomPanel, 0, wxEXPAND | wxALL, 5);

        SetSizerAndFit(pSizer1);

        wxBoxSizer* pSizer2 = new wxBoxSizer(wxHORIZONTAL);

        pSizer2->Add(m_pLicenseButton, 0, wxEXPAND | wxALL, 5);
        pSizer2->AddSpacer(425);
        pSizer2->Add(m_pCloseButton, 0, wxEXPAND | wxALL, 5);

        pBottomPanel->SetSizerAndFit(pSizer2);
    }

    void CAboutDialog::OnLicenseButtonClicked(wxCommandEvent& WXUNUSED(Event))
    {
        // Create new CAboutDialog instance.
        m_pLicenseDialog = new CLicenseDialog(this);

        // Call CLicenseDialog::ShowModal() method.
        m_pLicenseDialog->ShowModal();

        // Release created CLicenseDialog instance.
        wxDELETE(m_pLicenseDialog);
    }

    void CAboutDialog::OnClose(wxCloseEvent& Event)
    {
        // Destroy class.
        Destroy();
    }
} // end namespace WinRuler
