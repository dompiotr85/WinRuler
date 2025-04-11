/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include "CLicenseDialog.h"

namespace WinRuler
{
	/**
	 * CAboutDialog class implementation.
	 **/
	class CAboutDialog :
		public wxDialog
	{
		DECLARE_EVENT_TABLE()
	public:
		/**
		 * Constructor with default parameters.
		 *
		 * \param Parent	Pointer to parent instance.
		 * \param Id		Window Id. Default: wxID_ANY
		 * \param Title		Reference to dialog title string.
		 *					Default: "About WinRuler"
		 * \param Pos		Dialog position. Default: wxDefaultPosition
		 * \param Size		Dialog size. Default: wxSize(600, 440)
		 * \param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL
		 **/
		CAboutDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("About WinRuler"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxSize(600, 440),
			long Style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~CAboutDialog();

		/**
		 * Initializes about dialog with default values.
		 **/
		void Init();

		/**
		 * Creates all controls and adds them to the dialog.
		 **/
		void CreateControls();

		/**
		 * Sets up the layout of the dialog using sizers.
		 **/
		void SetupSizers();
	private:
		/**
		 * OnClose() method event.
		 *
		 * \param Event		Reference to wxCloseEvent object.
		 **/
		void OnClose(wxCloseEvent& Event);

		/**
		 * OnLicenseButtonClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent object.
		 **/
		void OnLicenseButtonClicked(wxCommandEvent& Event);
	private:
		//! Header static bitmap.
		wxStaticBitmap* m_pHeaderStaticBitmap;

		//! Version static text.
		wxStaticText* m_pVersionStaticText;

		//! Copyright static text.
		wxStaticText* m_pCopyrightStaticText;

		//! Bottom panel.
		wxPanel* m_pBottomPanel;

		//! License button.
		wxButton* m_pLicenseButton;

		//! CLicenseDialog instance.
		CLicenseDialog* m_pLicenseDialog;

		//! Close button.
		wxButton* m_pCloseButton;
	};
} // end namespace WinRuler
