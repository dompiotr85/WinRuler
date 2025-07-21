/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

namespace WinRuler
{
	/**
	 * CLicenseDialog class definition.
	 **/
    class CLicenseDialog :
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
		 *					Default: "MIT License"
		 * \param Pos		Dialog position. Default: wxDefaultPosition
		 * \param Size		Dialog size. Default: wxSize(500, 320)
		 * \param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX |
		 *					wxTAB_TRAVERSAL
		 **/
		CLicenseDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("MIT License"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxSize(500, 320),
			long Style = 
				wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~CLicenseDialog();

		/**
		 * Initialize about dialog.
		 **/
		void Init();

		/**
		 * Creates all controls for this license dialog.
		 **/
		void CreateControls();

		/**
		 * Setup sizers.
		 **/
		void SetupSizers();
	private:
		/**
		 * OnClose() method event.
		 *
		 * \param Event		Reference to wxCloseEvent instance.
		 **/
		void OnClose(wxCloseEvent& Event);
	public:
		//! Rich text control.
		wxRichTextCtrl* m_pTextCtrl;

		//! Close button.
		wxButton* m_pCloseButton;
    };
} // end namespace WinRuler
