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

        // Centre dialog.
        Centre();
    }

    CNewRulerLengthDialog::~CNewRulerLengthDialog()
    {
        // Release all instances from heap.
        wxDELETE(m_pOKButton);
        wxDELETE(m_pSpinCtrl);
        wxDELETE(m_pChooseStaticText);
        wxDELETE(m_pContentPanel);
    }

    void CNewRulerLengthDialog::Init()
    {
        // Set all class instances as NULL.
        m_pContentPanel = NULL;
        m_pChooseStaticText = NULL;
        m_pSpinCtrl = NULL;
        m_pOKButton = NULL;
    }

    void CNewRulerLengthDialog::CreateControls()
    {
        // Retrieve our CAboutDialog.
        CNewRulerLengthDialog* pDialog = this;

        // Retrieve our MainFrame.
        CMainFrame* pMainFrame = (CMainFrame*) pDialog->GetParent();

        // Create m_pContentPanel.
        m_pContentPanel =
            new wxPanel(
                pDialog, wxID_ANY, wxDefaultPosition,
                wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL);

        // Create Choose static text.
        m_pChooseStaticText =
            new wxStaticText(
                m_pContentPanel, wxID_ANY,
                wxString("Enter new length of the ruler:"),
                wxPoint(10, 10));

        // Create Spin control.
        m_pSpinCtrl =
            new wxSpinCtrl(
                m_pContentPanel, wxID_ANY,
                wxString::Format(wxT("%d"), (int) pMainFrame->m_iRulerLength),
                wxPoint(10, 40), wxSize(m_pChooseStaticText->GetSize().GetWidth(), -1), wxSP_ARROW_KEYS, 100, 2000);

        // Create OK button.
        m_pOKButton =
            new wxButton(
                m_pContentPanel, wxID_OK, "&OK",
                wxPoint(60, 70), wxSize(60, 30));
    }

    void CNewRulerLengthDialog::OnOKButtonClicked(wxCommandEvent& WXUNUSED(Event))
    {
        // Retrieve CMainFrame instance.
        CMainFrame* pMainFrame = (CMainFrame*) this->GetParent();

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
