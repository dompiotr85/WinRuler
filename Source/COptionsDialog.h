/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/clrpicker.h>
#include <wx/filepicker.h>

enum EOptionsIDs
{
	ID_BackgroundTypeChoice = 40,
	ID_RulerTransparency = 41
};

namespace WinRuler
{
	class COptionsDialog :
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
		 *					Default: "Options"
		 * @param Pos		Dialog position. Default: wxDefaultPosition
		 * @param Size		Dialog size. Default: wxSize(700, 560)
		 * @param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL
		 **/
		COptionsDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("Options"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxDefaultSize,
			long Style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~COptionsDialog();

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
		 * OnBackgroundTypeChoiceChanged() method event.
		 *
		 * @param Event		Reference to wxCommandEvent instance.
		 **/
		void OnBackgroundTypeChoiceChanged(wxCommandEvent& Event);

		/**
		 * OnRulerTransparencyCheckBoxClicked() method event.
		 * 
		 * @param Event		Reference to wxCommandEvent instance.
		 **/
		void OnRulerTransparencyCheckBoxClicked(wxCommandEvent& Event);
	public:
		// Bottom panel.
		wxPanel* m_pBottomPanel;

		// Bottom OK button.
		wxButton* m_pOKButton;

		// Notebook.
		wxNotebook* m_pNotebook;

		// Background type choice.
		wxChoice* m_pBackgroundTypeChoice;

		// Background colours static texts.
		wxStaticText* m_pBackgroundColourText;
		wxStaticText* m_pBackgroundStartEndColourText;
		wxStaticText* m_pBackgroundImageText;

		// Background colours.
		wxColourPickerCtrl* m_pBackgroundColourPicker;
		wxColourPickerCtrl* m_pBackgroundStartColourPicker;
		wxColourPickerCtrl* m_pBackgroundEndColourPicker;

		// Background image.
		wxFilePickerCtrl* m_pBackgroundImagePicker;

		// Ruler scale colour.
		wxColourPickerCtrl* m_pRulerScaleColourPicker;

		// First and second marker colour.
		wxColourPickerCtrl* m_pFirstMarkerColourPicker;
		wxColourPickerCtrl* m_pSecondMarkerColourPicker;

		// Ruler transparency.
		wxCheckBox* m_pRulerTransparencyCheckBox;

		// Ruler transparency text.
		wxStaticText* m_pRulerTransparencyText;

		// Ruler transparency value.
		wxSlider* m_pRulerTransparencySlider;
	};
} // end namespace WinRuler
