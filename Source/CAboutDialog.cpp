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
        wxDELETE(m_pContentPanel);
    }

    void CAboutDialog::Init()
    {
        // Set all class instances as NULL.
        m_pContentPanel = NULL;
        m_pHeaderStaticBitmap = NULL;
        m_pCopyrightStaticText = NULL;
        m_pVersionStaticText = NULL;
        m_pLicenseButton = NULL;
        m_pLicenseDialog = NULL;
        m_pCloseButton = NULL;
    }

    void CAboutDialog::CreateControls()
    {
        // Create m_pContentPanel.
        m_pContentPanel = 
            new wxPanel(
                    this, wxID_ANY, wxDefaultPosition,
                    wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL);

        // Create static bitmap on top.
        m_pHeaderStaticBitmap = 
            new wxStaticBitmap(
                m_pContentPanel,
                wxID_ANY,
                wxBitmap("../../../../Resources/About.png", wxBITMAP_TYPE_PNG),
                wxPoint(0, 0), wxSize(600, 300), 0);

        // Create static texts.
        m_pVersionStaticText =
            new wxStaticText(
                    m_pContentPanel, wxID_ANY,
                    wxString("Version 0.0.1"), wxPoint(15, 300));
        m_pCopyrightStaticText =
            new wxStaticText(
                    m_pContentPanel, wxID_ANY,
                    wxString("Copyright © 2024 Piotr Domanski and WinRuler programmers team."),
                    wxPoint(15, 320));

        // Create License button.
        m_pLicenseButton =
            new wxButton(
                    m_pContentPanel, ID_LICENSE_BUTTON, "&License",
                    wxPoint(20, 350), wxSize(60, 30));

        // Create Close button.
        m_pCloseButton =
            new wxButton(
                    m_pContentPanel, wxID_OK, "&Close",
                    wxPoint(500, 350), wxSize(60, 30));
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