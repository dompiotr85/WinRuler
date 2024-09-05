/**
 * Copyright © 2024 Piotr Domañski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace WinRuler
{
	/**
	 * NewRulerLengthDialog class definition.
	 **/
	class CNewRulerLengthDialog :
		public wxDialog
	{
		DECLARE_EVENT_TABLE()
	public:
		/**
		 * Constructor with default parameters.
		 *
		 * @param Parent	Pointer to parent instance.
		 * @param Id		Window Id. Default: wxID_ANY
		 * @param Title		Reference to dialog title string.
		 *					Default: "Choose new ruler's length"
		 * @param Pos		Dialog position. Default: wxDefaultPosition
		 * @param Size		Dialog size. Default: wxSize(200, 150)
		 * @param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL
		 **/
		CNewRulerLengthDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("Choose new ruler's length"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxDefaultSize,
			long Style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~CNewRulerLengthDialog();

		/**
		 * Initialize about dialog.
		 **/
		void Init();

		/**
		 * Creates all controls for this about dialog.
		 **/
		void CreateControls();
	private:
		/**
		 * OnClose() method event.
		 *
		 * @param Event		Reference to wxCloseEvent instance.
		 **/
		void OnClose(wxCloseEvent& Event);

		/**
		 * OnOKButtonClicked() method event.
		 *
		 * @param Event		Reference to wxCommandEvent instance.
		 **/
		void OnOKButtonClicked(wxCommandEvent& Event);
	private:
		// Content panel.
		wxPanel* m_pContentPanel;

		// Choose static text.
		wxStaticText* m_pChooseStaticText;

		// Spin control.
		wxSpinCtrl* m_pSpinCtrl;

		// OK button.
		wxButton* m_pOKButton;
	};
} // end namespace WinRuler
