/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include "WRPlatform.h"
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/clrpicker.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>

/**
 * Enumeration of options ids.
 **/
enum EOptionsIDs
{
	ID_BackgroundTypeChoice = 40,
	ID_RulerTransparency = 41,
	ID_CalibrateUnitOfMeasurementTypeChoice = 42,
	ID_VerticalRulerIncreaseButton = 43,
	ID_VerticalRulerDecreaseButton = 44,
	ID_HorizontalRulerIncreaseButton = 45,
	ID_HorizontalRulerDecreaseButton = 46,
	ID_SnapToEdgesOfScreen = 47,
	ID_SnapToOtherWindows = 48
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
		 * \param Parent	Pointer to parent instance.
		 * \param Id		Window Id. Default: wxID_ANY
		 * \param Title		Reference to dialog title string.
		 *					Default: "Options"
		 * \param Pos		Dialog position. Default: wxDefaultPosition
		 * \param Size		Dialog size. Default: wxSize(700, 560)
		 * \param Style		Dialog style.
		 *					Default: wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | 
		 *					wxTAB_TRAVERSAL
		 **/
		COptionsDialog(
			wxWindow* Parent, wxWindowID Id = wxID_ANY,
			const wxString& Title = wxString("Options"),
			const wxPoint& Pos = wxDefaultPosition,
			const wxSize& Size = wxDefaultSize,
			long Style = 
				wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxTAB_TRAVERSAL);

		/**
		 * Default destructor.
		 **/
		~COptionsDialog();

		/**
		 * Initialize options dialog.
		 **/
		void Init();

		/**
		 * Creates all controls for this options dialog.
		 **/
		void CreateControls();

		/**
		 * Setup sizers.
		 **/
		void SetupSizers();

		/**
		 * Called by wxWidgets when the panel needs to be redrawn.
		 **/
		void VRulerPanel_OnPaintEvent(wxPaintEvent& Event);
		void HRulerPanel_OnPaintEvent(wxPaintEvent& Event);

		/**
		 * Alternatively, you can use a clientDC to paint on the panel at any
		 * time. Using this generally does not free you from catching paint
		 * events, since it is possible that e.g. the window manager throws
		 * away your drawing when the window comes to the background, and
		 * expects you will redraw it when the window comes back (by sending
		 * a paint event).
		 *
		 * In most cases, this will not be needed at all; simply handling paint
		 * events and calling Refresh() when a refresh is needed will do the
		 * job.
		 **/
		void VRulerPanel_PaintNow();
		void HRulerPanel_PaintNow();

		/**
		 * Here we do the actual rendering. I put it in a separate method so
		 * that it can work no matter what type of DC (e.g. wxPaintDC or
		 * wxClientDC) is used.
		 **/
		void VRulerPanel_Render(wxDC& dc);
		void HRulerPanel_Render(wxDC& dc);
	private:
		/**
		 * Creates controls for the ruler page.
		 **/
		void CreateRulerPageControls();

		/**
		 * Creates controls for the calibration page.
		 **/
		void CreateCalibrationPageControls();

		/**
		 * Creates controls for the additional features page.
		 **/
		void CreateAdditionalFeaturesPageControls();

		/**
		 * Setup ruler page sizers.
		 **/
		void SetupRulerPageSizers();

		/**
		 * Setup calibration page sizers.
		 **/
		void SetupCalibrationPageSizers();

		/**
		 * Setup additional features page sizers.
		 **/
		void SetupAdditionalFeaturesPageSizers();

		/**
		 * OnClose() method event.
		 *
		 * \param Event		Reference to wxCloseEvent instance.
		 **/
		void OnClose(wxCloseEvent& Event);

		/**
		 * OnBackgroundTypeChoiceChanged() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnBackgroundTypeChoiceChanged(wxCommandEvent& Event);

		/**
		 * OnCalibrateUnitOfMeasurementTypeChoiceChanged() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnCalibrateUnitOfMeasurementTypeChoiceChanged(
			wxCommandEvent& Event);

		/**
		 * OnRulerTransparencyCheckBoxClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnRulerTransparencyCheckBoxClicked(wxCommandEvent& Event);

		/**
		 * OnVerticalRulerIncreaseButtonClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnVerticalRulerIncreaseButtonClicked(wxCommandEvent& Event);

		/**
		 * OnVerticalRulerDecreaseButtonClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnVerticalRulerDecreaseButtonClicked(wxCommandEvent& Event);

		/**
		 * OnHorizontalRulerIncreaseButtonClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnHorizontalRulerIncreaseButtonClicked(wxCommandEvent& Event);

		/**
		 * OnHorizontalRulerDecreaseButtonClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnHorizontalRulerDecreaseButtonClicked(wxCommandEvent& Event);

#ifdef WR_WINDOWS // If platform is Windows.
		/**
		 * OnSnapToEdgesOfScreenCheckBoxClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnSnapToEdgesOfScreenCheckBoxClicked(wxCommandEvent& Event);

		/**
		 * OnSnapToOtherWindowsCheckBoxClicked() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 **/
		void OnSnapToOtherWindowsCheckBoxClicked(wxCommandEvent& Event);
#endif
	public:
		//! Ruler panel.
		wxPanel* m_pRulerPanel;

		//! Calibration panel.
		wxPanel* m_pCalibrationPanel;

		//! Additional features panel.
		wxPanel* m_pAdditionalFeaturesPanel;

		//! Bottom panel.
		wxPanel* m_pBottomPanel;

		//! Bottom OK button.
		wxButton* m_pOKButton;

		//! Notebook.
		wxNotebook* m_pNotebook;

		//! Background static box.
		wxStaticBox* m_pBackgroundStaticBox;

		wxStaticText* m_pBackgroundTypeText;

		//! Background type choice.
		wxChoice* m_pBackgroundTypeChoice;

		//! Background colours static texts.
		wxStaticText* m_pBackgroundColourText;
		wxStaticText* m_pBackgroundStartEndColourText;
		wxStaticText* m_pBackgroundImageText;

		//! Background colours.
		wxColourPickerCtrl* m_pBackgroundColourPicker;
		wxColourPickerCtrl* m_pBackgroundStartColourPicker;
		wxColourPickerCtrl* m_pBackgroundEndColourPicker;

		//! Background image.
		wxFilePickerCtrl* m_pBackgroundImagePicker;

		//! Scale and markers static box.
		wxStaticBox* m_pScaleAndMarkersStaticBox;

		//! Ruler scale colour text.
		wxStaticText* m_pRulerScaleColourText;

		//! Ruler scale colour.
		wxColourPickerCtrl* m_pRulerScaleColourPicker;

		//! First and second marker colour text.
		wxStaticText* m_pRulerFirstMarkerColourText;
		wxStaticText* m_pRulerSecondMarkerColourText;

		//! First and second marker colour.
		wxColourPickerCtrl* m_pFirstMarkerColourPicker;
		wxColourPickerCtrl* m_pSecondMarkerColourPicker;

		//! Special options static box.
		wxStaticBox* m_pSpecialOptionsStaticBox;

		//! Ruler transparency.
		wxCheckBox* m_pRulerTransparencyCheckBox;

		//! Ruler transparency text.
		wxStaticText* m_pRulerTransparencyText;

		//! Ruler transparency value.
		wxSlider* m_pRulerTransparencySlider;

		//! Calibrate static box.
		wxStaticBox* m_pCalibrateStaticBox;

		//! Calibrate info text.
		wxStaticText* m_pCalibrateInfoText;

		//! Calibrate unit of measurement type text.
		wxStaticText* m_pCalibrateUnitOfMeasurementTypeText;

		//! Calibrate unit of measurement type choice.
		wxChoice* m_pCalibrateUnitOfMeasurementTypeChoice;

		//! Vertical and horizontal panels.
		wxPanel* m_pVRulerPanel;
		wxPanel* m_pHRulerPanel;

		//! Vertical increse & decrese buttons.
		wxButton* m_pV_IncButton;
		wxButton* m_pV_DecButton;

		//! Horizontal increse & decrese buttons.
		wxButton* m_pH_IncButton;
		wxButton* m_pH_DecButton;

		//! Horizontal and vertical PPI static text.
		wxStaticText* m_pVPPIStaticText;
		wxStaticText* m_pHPPIStaticText;

		//! Snapping to edges of the screen static box.
		wxStaticBox* m_pSnapToEdgesOfScreenStaticBox;

		//! Snapping to edges of the screen check box.
		wxCheckBox* m_pSnapToEdgesOfScreenCheckBox;

		//! Snapping to edges of the screen distance static text.
		wxStaticText* m_pSnapToEdgesOfScreenStaticText;

		//! Snapping to edges of the screen distance spin control.
		wxSpinCtrl* m_pSnapToEdgesOfScreenSpinCtrl;

		//! Snapping to other windows static box.
		wxStaticBox* m_pSnapToOtherWindowsStaticBox;

		//! Snapping to other windows check box.
		wxCheckBox* m_pSnapToOtherWindowsCheckBox;

		//! Snapping to other windows static text.
		wxStaticText* m_pSnapToOtherWindowsStaticText;

		//! Snapping to other windows spin control.
		wxSpinCtrl* m_pSnapToOtherWindowsSpinCtrl;
	};
} // end namespace WinRuler
