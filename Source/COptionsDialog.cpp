/**
 * Copyright   2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "COptionsDialog.h"
#include "CMainFrame.h"

namespace WinRuler
{
	BEGIN_EVENT_TABLE(COptionsDialog, wxDialog)

	EVT_CLOSE(COptionsDialog::OnClose)
	EVT_CHOICE(
		ID_BackgroundTypeChoice, 
		COptionsDialog::OnBackgroundTypeChoiceChanged)
	EVT_CHECKBOX(
		ID_RulerTransparency, 
		COptionsDialog::OnRulerTransparencyCheckBoxClicked)
	EVT_CHOICE(
		ID_CalibrateUnitOfMeasurementTypeChoice, 
		COptionsDialog::OnCalibrateUnitOfMeasurementTypeChoiceChanged)
	EVT_BUTTON(
		ID_VerticalRulerIncreaseButton, 
		COptionsDialog::OnVerticalRulerIncreaseButtonClicked)
	EVT_BUTTON(
		ID_VerticalRulerDecreaseButton, 
		COptionsDialog::OnVerticalRulerDecreaseButtonClicked)
	EVT_BUTTON(
		ID_HorizontalRulerIncreaseButton, 
		COptionsDialog::OnHorizontalRulerIncreaseButtonClicked)
	EVT_BUTTON(
		ID_HorizontalRulerDecreaseButton, 
		COptionsDialog::OnHorizontalRulerDecreaseButtonClicked)
#ifdef WR_WINDOWS // If platform is Windows.
	EVT_CHECKBOX(
		ID_SnapToEdgesOfScreen, 
		COptionsDialog::OnSnapToEdgesOfScreenCheckBoxClicked)
	EVT_CHECKBOX(
		ID_SnapToOtherWindows, 
		COptionsDialog::OnSnapToOtherWindowsCheckBoxClicked)
#endif

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

		// Setup sizers.
		SetupSizers();

		// Centre dialog.
		Centre();
	}

	COptionsDialog::~COptionsDialog()
	{
		// Release all instances from heap. The release order is important.
		wxDELETE(m_pHPPIStaticText);
		wxDELETE(m_pVPPIStaticText);
		wxDELETE(m_pVRulerPanel);
		wxDELETE(m_pV_IncButton);
		wxDELETE(m_pV_DecButton);
		wxDELETE(m_pHRulerPanel);
		wxDELETE(m_pH_IncButton);
		wxDELETE(m_pH_DecButton);
		wxDELETE(m_pCalibrateInfoText);
		wxDELETE(m_pCalibrateStaticBox);

		wxDELETE(m_pRulerTransparencyCheckBox);
		wxDELETE(m_pRulerTransparencyText);
		wxDELETE(m_pRulerTransparencySlider);
		wxDELETE(m_pSpecialOptionsStaticBox);

		wxDELETE(m_pRulerScaleColourText);
		wxDELETE(m_pRulerScaleColourPicker);
		wxDELETE(m_pRulerFirstMarkerColourText);
		wxDELETE(m_pRulerSecondMarkerColourText);
		wxDELETE(m_pFirstMarkerColourPicker);
		wxDELETE(m_pSecondMarkerColourPicker);
		wxDELETE(m_pScaleAndMarkersStaticBox);

		wxDELETE(m_pBackgroundTypeText);
		wxDELETE(m_pBackgroundTypeChoice);
		wxDELETE(m_pBackgroundColourText);
		wxDELETE(m_pBackgroundStartEndColourText);
		wxDELETE(m_pBackgroundImageText);
		wxDELETE(m_pBackgroundColourPicker);
		wxDELETE(m_pBackgroundStartColourPicker);
		wxDELETE(m_pBackgroundEndColourPicker);
		wxDELETE(m_pBackgroundImagePicker);
		wxDELETE(m_pBackgroundStaticBox);

		wxDELETE(m_pOKButton);
		wxDELETE(m_pBottomPanel);

		//wxDELETE(m_pRulerPanel);
		//wxDELETE(m_pCalibrationPanel);

		wxDELETE(m_pNotebook);
	}

	void COptionsDialog::Init()
	{
		m_pRulerPanel = NULL;
		m_pCalibrationPanel = NULL;
		m_pAdditionalFeaturesPanel = NULL;
		m_pBottomPanel = NULL;
		m_pOKButton = NULL;
		m_pNotebook = NULL;
		m_pBackgroundStaticBox = NULL;
		m_pBackgroundTypeText = NULL;
		m_pBackgroundTypeChoice = NULL;
		m_pBackgroundColourText = NULL;
		m_pBackgroundStartEndColourText = NULL;
		m_pBackgroundImageText = NULL;
		m_pBackgroundColourPicker = NULL;
		m_pBackgroundStartColourPicker = NULL;
		m_pBackgroundEndColourPicker = NULL;
		m_pBackgroundImagePicker = NULL;
		m_pScaleAndMarkersStaticBox = NULL;
		m_pRulerScaleColourText = NULL;
		m_pRulerScaleColourPicker = NULL;
		m_pRulerFirstMarkerColourText = NULL;
		m_pRulerSecondMarkerColourText = NULL;
		m_pFirstMarkerColourPicker = NULL;
		m_pSecondMarkerColourPicker = NULL;
		m_pSpecialOptionsStaticBox = NULL;
		m_pRulerTransparencyCheckBox = NULL;
		m_pRulerTransparencyText = NULL;
		m_pRulerTransparencySlider = NULL;
		m_pCalibrateStaticBox = NULL;
		m_pCalibrateInfoText = NULL;
		m_pVRulerPanel = NULL;
		m_pV_IncButton = NULL;
		m_pV_DecButton = NULL;
		m_pHRulerPanel = NULL;
		m_pH_IncButton = NULL;
		m_pH_DecButton = NULL;
		m_pVPPIStaticText = NULL;
		m_pHPPIStaticText = NULL;
	}

	void COptionsDialog::CreateControls()
	{
		// Set client size.
		// @todo	Right now code below is a dirty hack. 
#ifdef WR_LINUX // If platform is Linux.
		int cWidth = 700;
		int cHeight = 860;
#elif defined(WR_WINDOWS) // If platform is Windows.
		int cWidth = 700;
		int cHeight = 740;
#elif defined(WR_MACOS) // If platform is macOS.
        int cWidth = 700;
        int cHeight = 860;
#else
# warning "Unknown platform!"
		int cWidth = 700;
		int cHeight = 740;
#endif

        SetClientSize(wxSize(cWidth, cHeight));

		// Create Notebook.
		m_pNotebook =
			new wxNotebook(
				this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
		m_pNotebook->SetMaxClientSize(wxSize(cWidth, cHeight));

		// Create Notebook pages.
		m_pRulerPanel = new wxPanel(m_pNotebook, wxID_ANY);
		m_pCalibrationPanel = new wxPanel(m_pNotebook, wxID_ANY);
#ifdef WR_WINDOWS // If platform is Windows.
		m_pAdditionalFeaturesPanel = new wxPanel(m_pNotebook, wxID_ANY);
#endif

		// Creates Ruler page controls.
		CreateRulerPageControls();

		// Creates Calibration page controls.
		CreateCalibrationPageControls();

		// Creates Additional features page controls.
		CreateAdditionalFeaturesPageControls();

		// Add created notebook pages to notebook.
		m_pNotebook->AddPage(m_pRulerPanel, wxString("Ruler"));
		m_pNotebook->AddPage(m_pCalibrationPanel, wxString("Calibration"));
#ifdef WR_WINDOWS // If platform is Windows.
		m_pNotebook->AddPage(
            m_pAdditionalFeaturesPanel, wxString("Additional features"));
#endif

		// Add BottomPanel and OKButton.
		m_pBottomPanel = new wxPanel(this, wxID_ANY);
		m_pOKButton = new wxButton(m_pBottomPanel, wxID_OK, wxString("&OK"));
	}

	void COptionsDialog::SetupSizers()
	{
		// Setup ruler page sizers.
		SetupRulerPageSizers();

		// Setup calibration page sizers.
		SetupCalibrationPageSizers();


		// Setup additional features page sizers.
		SetupAdditionalFeaturesPageSizers();

		// Create wxBoxSizer and fit our Notebook and Bottom panel.
		wxBoxSizer* pBoxSizer = new wxBoxSizer(wxVERTICAL);

		wxSizerFlags flags1 =
			wxSizerFlags().Proportion(0).Expand().Border(wxALL, 5);
		wxSizerFlags flags2 =
			wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5);

		pBoxSizer->Add(m_pNotebook, flags2);
		pBoxSizer->Add(m_pBottomPanel, flags1);

		SetSizerAndFit(pBoxSizer);

		// Centre OK button.
		m_pOKButton->Centre();
	}

	void COptionsDialog::VRulerPanel_OnPaintEvent(wxPaintEvent& Event)
	{
		// Create wxPaintDC.
		wxPaintDC dc(m_pVRulerPanel);

		// Draw on created DC.
		VRulerPanel_Render(dc);
	}

	void COptionsDialog::HRulerPanel_OnPaintEvent(wxPaintEvent& Event)
	{
		// Create wxPaintDC.
		wxPaintDC dc(m_pHRulerPanel);

		// Draw on created DC.
		HRulerPanel_Render(dc);
	}

	void COptionsDialog::VRulerPanel_PaintNow()
	{
		// Create wxPaintDC.
		wxClientDC dc(m_pVRulerPanel);

		// Draw on created DC.
		VRulerPanel_Render(dc);
	}

	void COptionsDialog::HRulerPanel_PaintNow()
	{
		// Create wxPaintDC.
		wxClientDC dc(m_pHRulerPanel);

		// Draw on created DC.
		HRulerPanel_Render(dc);
	}

	void COptionsDialog::VRulerPanel_Render(wxDC& dc)
	{
		// Retrieve pointer to CMainFrame class.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Retrieve surface size.
		wxSize size(m_pVRulerPanel->GetClientSize());
		wxRect surfaceRect(0, 0, size.GetWidth(), size.GetHeight());

		// Draw ruler's surface on specified display context.
		pMainFrame->m_pDrawPanel->DrawRulerSurface(
			dc, surfaceRect,
			ERulerPosition::rpLeft,
			pMainFrame->m_eRulerBackgroundType,
			pMainFrame->m_cRulerBackgroundColour,
			pMainFrame->m_cRulerBackgroundStartColour,
			pMainFrame->m_cRulerBackgroundEndColour,
			pMainFrame->m_RulerBackgroundBitmapLeftH,
			pMainFrame->m_RulerBackgroundBitmapMiddleH,
			pMainFrame->m_RulerBackgroundBitmapRightH,
			pMainFrame->m_RulerBackgroundBitmapTopV,
			pMainFrame->m_RulerBackgroundBitmapMiddleV,
			pMainFrame->m_RulerBackgroundBitmapBottomV);

		// Draw ruler's scale.
		int nSelection =
			m_pCalibrateUnitOfMeasurementTypeChoice->GetSelection();
		ERulerUnits rulerUnits = ERulerUnits::ruCentimetres;

		// If selection is valid, set ruler units to selected one.
		if (nSelection != wxNOT_FOUND)
		{
			switch (nSelection)
			{
			case 0:	// Centimetres.
				rulerUnits = ERulerUnits::ruCentimetres;

				break;
			case 1:	// Inches.
				rulerUnits = ERulerUnits::ruInches;

				break;
			}
		}
		else // If selection is invalid, set ruler units to default one.
			rulerUnits = ERulerUnits::ruCentimetres;

		pMainFrame->m_pDrawPanel->DrawRulerScale(
			dc, surfaceRect, pMainFrame->m_cRulerScaleColour,
			ERulerPosition::rpLeft, rulerUnits);

		// Draw ruler's markers.
		pMainFrame->m_pDrawPanel->DrawRulerMarkers(
			dc, surfaceRect,
			ERulerPosition::rpLeft,
			pMainFrame->m_eRulerUnits,
			pMainFrame->m_cRulerScaleColour,
			pMainFrame->m_cFirstMarkerColour,
			pMainFrame->m_cSecondMarkerColour,
			-1, -1);
	}

	void COptionsDialog::HRulerPanel_Render(wxDC& dc)
	{
		// Retrieve pointer to CMainFrame class.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Retrieve surface size.
		wxSize size(m_pHRulerPanel->GetClientSize());
		wxRect surfaceRect(0, 0, size.GetWidth(), size.GetHeight());

		// Draw ruler's surface.
		pMainFrame->m_pDrawPanel->DrawRulerSurface(
			dc, surfaceRect,
			ERulerPosition::rpTop,
			pMainFrame->m_eRulerBackgroundType,
			pMainFrame->m_cRulerBackgroundColour,
			pMainFrame->m_cRulerBackgroundStartColour,
			pMainFrame->m_cRulerBackgroundEndColour,
			pMainFrame->m_RulerBackgroundBitmapLeftH,
			pMainFrame->m_RulerBackgroundBitmapMiddleH,
			pMainFrame->m_RulerBackgroundBitmapRightH,
			pMainFrame->m_RulerBackgroundBitmapTopV,
			pMainFrame->m_RulerBackgroundBitmapMiddleV,
			pMainFrame->m_RulerBackgroundBitmapBottomV);

		// Draw ruler's scale.
		int nSelection =
			m_pCalibrateUnitOfMeasurementTypeChoice->GetSelection();
		ERulerUnits rulerUnits = ERulerUnits::ruCentimetres;

		// If selection is valid, set ruler units to selected one.
		if (nSelection != wxNOT_FOUND)
		{
			switch (nSelection)
			{
			case 0:	// Centimetres.
				rulerUnits = ERulerUnits::ruCentimetres;

				break;
			case 1:	// Inches.
				rulerUnits = ERulerUnits::ruInches;

				break;
			}
		}
		else // If selection is invalid, set ruler units to default one.
			rulerUnits = ERulerUnits::ruCentimetres;

		pMainFrame->m_pDrawPanel->DrawRulerScale(
			dc, surfaceRect, pMainFrame->m_cRulerScaleColour,
			ERulerPosition::rpTop, rulerUnits);

		// Draw ruler's markers.
		pMainFrame->m_pDrawPanel->DrawRulerMarkers(
			dc, surfaceRect,
			ERulerPosition::rpTop,
			pMainFrame->m_eRulerUnits,
			pMainFrame->m_cRulerScaleColour,
			pMainFrame->m_cFirstMarkerColour,
			pMainFrame->m_cSecondMarkerColour,
			-1, -1);
	}

	void COptionsDialog::CreateRulerPageControls()
	{
		// Retrieve pointer to CMainFrame class.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Create ruler background StaticBox.
		m_pBackgroundStaticBox =
			new wxStaticBox(
				m_pRulerPanel, wxID_ANY, wxString("Background"));

		// Create ruler background type choice.
		wxArrayString Choices;
		Choices.Add(wxString("Solid"));
		Choices.Add(wxString("Gradient"));
		Choices.Add(wxString("Image"));

		m_pBackgroundTypeText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background type:"));
		m_pBackgroundTypeChoice =
			new wxChoice(
				m_pBackgroundStaticBox, ID_BackgroundTypeChoice,
				wxDefaultPosition, wxDefaultSize,
				Choices);

		// Select current ruler background type.
		m_pBackgroundTypeChoice->Select(pMainFrame->m_eRulerBackgroundType);

		// Create ruler background colours picker.
		m_pBackgroundColourText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background colour:"));
		m_pBackgroundColourPicker =
			new wxColourPickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_cRulerBackgroundColour,
				wxDefaultPosition, wxDefaultSize,
				wxCLRP_SHOW_LABEL);

		// Create ruler background start and end colour picker.
		m_pBackgroundStartEndColourText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background start and end colour:"));
		m_pBackgroundStartColourPicker =
			new wxColourPickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_cRulerBackgroundStartColour,
				wxDefaultPosition, wxDefaultSize,
				wxCLRP_SHOW_LABEL);
		m_pBackgroundEndColourPicker =
			new wxColourPickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_cRulerBackgroundEndColour,
				wxDefaultPosition, wxDefaultSize,
				wxCLRP_SHOW_LABEL);

		// Create ruler background image picker.
		m_pBackgroundImageText =
			new wxStaticText(
				m_pBackgroundStaticBox, wxID_ANY,
				wxString("Ruler background image:"));
		m_pBackgroundImagePicker =
			new wxFilePickerCtrl(
				m_pBackgroundStaticBox, wxID_ANY,
				pMainFrame->m_sRulerBackgroundImagePath);

		m_pScaleAndMarkersStaticBox =
			new wxStaticBox(
				m_pRulerPanel, wxID_ANY, wxString("Scale and markers"));

		// Create ruler scale colour picker.
		m_pRulerScaleColourText =
			new wxStaticText(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				wxString("Ruler scale colour:"));
		m_pRulerScaleColourPicker =
			new wxColourPickerCtrl(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				pMainFrame->m_cRulerScaleColour);

		// Create ruler first and second marker colour.
		m_pRulerFirstMarkerColourText =
			new wxStaticText(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				wxString("Ruler first marker colour:"));
		m_pFirstMarkerColourPicker =
			new wxColourPickerCtrl(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				pMainFrame->m_cFirstMarkerColour);

		m_pRulerSecondMarkerColourText =
			new wxStaticText(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				wxString("Ruler second marker colour:"));
		m_pSecondMarkerColourPicker =
			new wxColourPickerCtrl(
				m_pScaleAndMarkersStaticBox, wxID_ANY,
				pMainFrame->m_cSecondMarkerColour);

		// Create wxStaticBox for Special options.
		m_pSpecialOptionsStaticBox =
			new wxStaticBox(
				m_pRulerPanel, wxID_ANY, wxString("Special options"));

		// Create new ruler transparency check box.
		m_pRulerTransparencyCheckBox =
			new wxCheckBox(
				m_pSpecialOptionsStaticBox, ID_RulerTransparency,
				wxString("Ruler transparency"));
		m_pRulerTransparencyCheckBox->SetValue(
			pMainFrame->m_bRulerTransparency);

		// Create new ruler transparency slider.
		m_pRulerTransparencyText =
			new wxStaticText(
				m_pSpecialOptionsStaticBox, wxID_ANY,
				wxString("Ruler transparency value:"));

		m_pRulerTransparencySlider =
			new wxSlider(
				m_pSpecialOptionsStaticBox, wxID_ANY,
				static_cast<int>(pMainFrame->m_iRulerTransparencyValue),
				0, 255, wxDefaultPosition, wxDefaultSize,
				wxSL_HORIZONTAL | wxSL_VALUE_LABEL);

		// At the end of ruler page creation, enable proper items depending on
		// current settings.
		switch (pMainFrame->m_eRulerBackgroundType)
		{
		case btSolid:
			m_pBackgroundColourText->Enable(true);
			m_pBackgroundColourPicker->Enable(true);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case btGradient:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColourPicker->Enable(true);
			m_pBackgroundEndColourPicker->Enable(true);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case btImage:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(true);
			m_pBackgroundImagePicker->Enable(true);

			break;
		}

		if (m_pRulerTransparencyCheckBox->IsChecked())
		{
			m_pRulerTransparencyText->Enable(true);
			m_pRulerTransparencySlider->Enable(true);
		}
		else
		{
			m_pRulerTransparencyText->Enable(false);
			m_pRulerTransparencySlider->Enable(false);
		}
	}

	void COptionsDialog::CreateCalibrationPageControls()
	{
		// Create Calibrate static box.
		m_pCalibrateStaticBox =
			new wxStaticBox(
				m_pCalibrationPanel, wxID_ANY, wxString("Calibrate"));

		// Create Calibrate information text.
		m_pCalibrateInfoText =
			new wxStaticText(
				m_pCalibrateStaticBox, wxID_ANY,
				wxString(
					"WinRuler is based on a operating system factor that "
					"provides the vertical and horizontal pixels per inch "
					"(PPI). In some computer configurations (often in "
					"laptops), this factor indicates a default value of 96 "
					"pixels per inch, which is not the correct one. This "
					"value is not accurate, giving an incorrect scale in our "
					"screen ruler. Manual calibration of the PPI value for "
					"the height and width of the screen solves this problem. "
					"This calibration is set only for WinRuler application. "
					"To properly calibrate WinRuler, please put a real ruler "
					"on your computer screen close to the displayed scale and "
					"calibrate the scale on the screen by pressing the "
					"""Increase"" or ""Decrease"" button. Note that you must "
					"calibrate horizontal and vertical PPI values. After "
					"proper calibration, the ruler should indicate the "
					"correct distances."));

		// Create unit of measurement type choice.
		wxArrayString Choices;
		Choices.Add(wxString("Centimetres"));
		Choices.Add(wxString("Inches"));

		m_pCalibrateUnitOfMeasurementTypeText =
			new wxStaticText(
					m_pCalibrateStaticBox, wxID_ANY,
					wxString("Unit of measurement type:"));

		m_pCalibrateUnitOfMeasurementTypeChoice =
			new wxChoice(
					m_pCalibrateStaticBox,
					ID_CalibrateUnitOfMeasurementTypeChoice,
					wxDefaultPosition, wxDefaultSize, Choices);

		// Select current unit of measurement type.
		m_pCalibrateUnitOfMeasurementTypeChoice->Select(0);



		// Create vertical and horizontal panels that will display rulers for
		// calibration. Create also Increase and Decrease buttons for vertical
		// and horizontal ruler.
		m_pVRulerPanel =
			new wxPanel(
				m_pCalibrateStaticBox, wxID_ANY,
				wxDefaultPosition, wxSize(60, 200));
		m_pVRulerPanel->Bind(
			wxEVT_PAINT, &COptionsDialog::VRulerPanel_OnPaintEvent, this);

		m_pV_IncButton =
			new wxButton(
				m_pCalibrateStaticBox, ID_VerticalRulerIncreaseButton,
				wxString("&Increase"));
		m_pV_DecButton =
			new wxButton(
				m_pCalibrateStaticBox, ID_VerticalRulerDecreaseButton,
				wxString("&Decrease"));

		m_pHRulerPanel =
			new wxPanel(
				m_pCalibrateStaticBox, wxID_ANY,
				wxDefaultPosition, wxSize(200, 60));
		m_pHRulerPanel->Bind(
			wxEVT_PAINT, &COptionsDialog::HRulerPanel_OnPaintEvent, this);

		m_pH_IncButton =
			new wxButton(
				m_pCalibrateStaticBox, ID_HorizontalRulerIncreaseButton,
				wxString("&Increase"));
		m_pH_DecButton =
			new wxButton(
				m_pCalibrateStaticBox, ID_HorizontalRulerDecreaseButton,
				wxString("&Decrease"));

		// Create vertical and horizontal PPI static texts.
		m_pVPPIStaticText =
			new wxStaticText(
				m_pCalibrateStaticBox, wxID_ANY,
				wxString::Format(
					"Vertical PPI: %d", g_vPixelPerInch[0].GetY()));
		m_pHPPIStaticText =
			new wxStaticText(
				m_pCalibrateStaticBox, wxID_ANY,
				wxString::Format(
					"Horizontal PPI: %d", g_vPixelPerInch[0].GetX()));
	}

	void COptionsDialog::CreateAdditionalFeaturesPageControls()
	{
		// Retrieve pointer to CMainFrame class.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

#ifdef WR_WINDOWS // If platform is Windows.
		// Create snap to edges of the screen static box.
		m_pSnapToEdgesOfScreenStaticBox =
			new wxStaticBox(
				m_pAdditionalFeaturesPanel, wxID_ANY,
				wxString("Snap ruler to edges of the screen"));

		// Create snap to edges of the screen check box.
		m_pSnapToEdgesOfScreenCheckBox =
			new wxCheckBox(
				m_pSnapToEdgesOfScreenStaticBox, ID_SnapToEdgesOfScreen,
				wxString("Enable snapping to edges of the screen"));
		m_pSnapToEdgesOfScreenCheckBox->SetValue(
			pMainFrame->m_bSnapToEdgesOfScreen);

		// Create snap to edges of the screen static text.
		m_pSnapToEdgesOfScreenStaticText =
			new wxStaticText(
				m_pSnapToEdgesOfScreenStaticBox, wxID_ANY,
				wxString("Snap to edges of the screen distance:"));

		// If snap to edges of the screen is enabled, enable also
		// our static text. Otherwise disable it.
		m_pSnapToEdgesOfScreenStaticText->Enable(
			pMainFrame->m_bSnapToEdgesOfScreen);

		// Create snap to edges of the screen spin control.
		m_pSnapToEdgesOfScreenSpinCtrl =
			new wxSpinCtrl(
				m_pSnapToEdgesOfScreenStaticBox, wxID_ANY,
				wxString::Format(
					"%d", pMainFrame->m_iSnapToEdgesOfScreenDistance),
				wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 10);

		// If snap to edges of the screen is enabled, enable also our spin
		// control. Otherwise disable it.
		m_pSnapToEdgesOfScreenSpinCtrl->Enable(
			pMainFrame->m_bSnapToEdgesOfScreen);

		// Create snap to other windows static box.
		m_pSnapToOtherWindowsStaticBox =
			new wxStaticBox(
				m_pAdditionalFeaturesPanel, wxID_ANY,
				wxString("Snap ruler to other windows"));

		// Create snap to other windows check box.
		m_pSnapToOtherWindowsCheckBox =
			new wxCheckBox(
				m_pSnapToOtherWindowsStaticBox, ID_SnapToOtherWindows,
				wxString("Enable snapping to other windows"));
		m_pSnapToOtherWindowsCheckBox->SetValue(
			pMainFrame->m_bSnapToOtherWindows);

		// Create snap to other windows static text.
		m_pSnapToOtherWindowsStaticText =
			new wxStaticText(
				m_pSnapToOtherWindowsStaticBox, wxID_ANY,
				wxString("Snap to other window distance:"));

		// If snap to other windows is enabled, enable also our
		// static box. Otherwise disable it.
		m_pSnapToOtherWindowsStaticText->Enable(
			pMainFrame->m_bSnapToOtherWindows);

		// Create snap to other windows spin control.
		m_pSnapToOtherWindowsSpinCtrl =
			new wxSpinCtrl(
				m_pSnapToOtherWindowsStaticBox, wxID_ANY,
				wxString::Format(
					"%d", pMainFrame->m_iSnapToOtherWindowsDistance),
				wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 10);

		// If snap to other windows is enabled, enable also our spin control.
		// Otherwise disable it.
		m_pSnapToOtherWindowsSpinCtrl->Enable(
			pMainFrame->m_bSnapToOtherWindows);
#endif
	}

	void COptionsDialog::SetupRulerPageSizers()
	{
		// Create wxBoxSizer and apply it on all components inside
		// m_pBackgroundStaticBox.
		wxBoxSizer* pBackgroundBoxSizer = new wxBoxSizer(wxVERTICAL);

		wxSizerFlags flags1 =
			wxSizerFlags().Proportion(0).Expand().Border(wxALL, 5);
		wxSizerFlags flags2 =
			wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5);

		pBackgroundBoxSizer->AddSpacer(20);
		pBackgroundBoxSizer->Add(m_pBackgroundTypeText, flags1);
		pBackgroundBoxSizer->Add(m_pBackgroundTypeChoice, flags2);
		pBackgroundBoxSizer->AddSpacer(5);
		pBackgroundBoxSizer->Add(m_pBackgroundColourText, flags1);
		pBackgroundBoxSizer->Add(m_pBackgroundColourPicker, flags2);
		pBackgroundBoxSizer->AddSpacer(5);
		pBackgroundBoxSizer->Add(m_pBackgroundStartEndColourText, flags1);
		pBackgroundBoxSizer->Add(m_pBackgroundStartColourPicker, flags2);
		pBackgroundBoxSizer->Add(m_pBackgroundEndColourPicker, flags2);
		pBackgroundBoxSizer->AddSpacer(5);
		pBackgroundBoxSizer->Add(m_pBackgroundImageText, flags1);
		pBackgroundBoxSizer->Add(m_pBackgroundImagePicker, flags2);
		pBackgroundBoxSizer->AddSpacer(20);

		m_pBackgroundStaticBox->SetSizerAndFit(pBackgroundBoxSizer);

		// Create wxBoxSizer and apply it on all components inside
		// m_pRulerStaticBox.
		wxBoxSizer* pScaleAndMarkersBoxSizer = new wxBoxSizer(wxVERTICAL);

		pScaleAndMarkersBoxSizer->AddSpacer(20);
		pScaleAndMarkersBoxSizer->Add(m_pRulerScaleColourText, flags1);
		pScaleAndMarkersBoxSizer->Add(m_pRulerScaleColourPicker, flags2);
		pScaleAndMarkersBoxSizer->AddSpacer(5);
		pScaleAndMarkersBoxSizer->Add(m_pRulerFirstMarkerColourText, flags1);
		pScaleAndMarkersBoxSizer->Add(m_pFirstMarkerColourPicker, flags2);
		pScaleAndMarkersBoxSizer->Add(m_pRulerSecondMarkerColourText, flags1);
		pScaleAndMarkersBoxSizer->Add(m_pSecondMarkerColourPicker, flags2);
		pScaleAndMarkersBoxSizer->AddSpacer(20);

		m_pScaleAndMarkersStaticBox->SetSizerAndFit(pScaleAndMarkersBoxSizer);

		// Create new wxBoxSizer and apply it on all components inside
		// m_pSpecialOptionsStaticBox.
		wxBoxSizer* pSpecialOptionsBoxSizer = new wxBoxSizer(wxVERTICAL);

		pSpecialOptionsBoxSizer->AddSpacer(20);
		pSpecialOptionsBoxSizer->Add(m_pRulerTransparencyCheckBox, flags1);
		pSpecialOptionsBoxSizer->Add(m_pRulerTransparencyText, flags1);
		pSpecialOptionsBoxSizer->Add(m_pRulerTransparencySlider, flags2);
		pSpecialOptionsBoxSizer->AddSpacer(20);

		m_pSpecialOptionsStaticBox->SetSizerAndFit(pSpecialOptionsBoxSizer);

		// Create wxBoxSizer and fit all static boxes on pRulerPanel.
		wxBoxSizer* pStaticBoxSizer = new wxBoxSizer(wxVERTICAL);

		pStaticBoxSizer->Add(m_pBackgroundStaticBox, flags2);
		pStaticBoxSizer->Add(m_pScaleAndMarkersStaticBox, flags2);
		pStaticBoxSizer->Add(m_pSpecialOptionsStaticBox, flags2);

		m_pRulerPanel->SetSizerAndFit(pStaticBoxSizer);
	}

	void COptionsDialog::SetupCalibrationPageSizers()
	{
		// Create wxBoxSizer and fit all calibrate components.
		wxBoxSizer* pCalibrateBoxSizer = new wxBoxSizer(wxVERTICAL);

		wxSizerFlags flags1 =
			wxSizerFlags().Proportion(0).Expand().Border(wxALL, 5);
		wxSizerFlags flags2 =
			wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5);

		pCalibrateBoxSizer->AddSpacer(20);
		pCalibrateBoxSizer->Add(m_pCalibrateInfoText, flags2);
		pCalibrateBoxSizer->AddSpacer(5);
		pCalibrateBoxSizer->Add(m_pCalibrateUnitOfMeasurementTypeText, flags1);
		pCalibrateBoxSizer->Add(
			m_pCalibrateUnitOfMeasurementTypeChoice, flags1);

		wxBoxSizer* pVerticalSizer = new wxBoxSizer(wxVERTICAL);

		wxSizerFlags flags3 = wxSizerFlags().Border(wxALL, 5);

		pVerticalSizer->Add(m_pV_IncButton, flags3);
		pVerticalSizer->Add(m_pVRulerPanel, flags3);
		pVerticalSizer->Add(m_pV_DecButton, flags3);

		wxBoxSizer* pHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

		pHorizontalSizer->Add(m_pH_IncButton, flags3);
		pHorizontalSizer->Add(m_pHRulerPanel, flags3);
		pHorizontalSizer->Add(m_pH_DecButton, flags3);

		wxBoxSizer* pRulersSizer = new wxBoxSizer(wxHORIZONTAL);

		pRulersSizer->AddSpacer(30);
		pRulersSizer->Add(pVerticalSizer, flags2);
		pRulersSizer->AddSpacer(30);
		pRulersSizer->Add(pHorizontalSizer, flags2);
		pRulersSizer->AddSpacer(30);

		pCalibrateBoxSizer->Add(pRulersSizer, flags2);
		pCalibrateBoxSizer->AddSpacer(20);

		wxBoxSizer* pInfoSizer = new wxBoxSizer(wxHORIZONTAL);

		pInfoSizer->Add(m_pVPPIStaticText, flags2);
		pInfoSizer->Add(m_pHPPIStaticText, flags2);

		pCalibrateBoxSizer->Add(pInfoSizer, flags2);

		m_pCalibrateStaticBox->SetSizerAndFit(pCalibrateBoxSizer);

		// Create wxBoxSizer and fit all static boxes on pCalibrationPanel.
		wxBoxSizer* pStaticBoxSizer2 = new wxBoxSizer(wxVERTICAL);

		pStaticBoxSizer2->Add(m_pCalibrateStaticBox, flags2);

		m_pCalibrationPanel->SetSizerAndFit(pStaticBoxSizer2);
	}

	void COptionsDialog::SetupAdditionalFeaturesPageSizers()
	{
#ifdef WR_WINDOWS // If platform is Windows.
		wxBoxSizer* pSnappingBoxSizer = new wxBoxSizer(wxVERTICAL);

		wxSizerFlags flags =
			wxSizerFlags().Proportion(0).Expand().Border(wxALL, 5);

		pSnappingBoxSizer->Add(m_pSnapToEdgesOfScreenStaticBox, flags);
		pSnappingBoxSizer->Add(m_pSnapToOtherWindowsStaticBox, flags);

		m_pAdditionalFeaturesPanel->SetSizerAndFit(pSnappingBoxSizer);

		wxBoxSizer* pSnapToEdgesBoxSizer = new wxBoxSizer(wxVERTICAL);

		pSnapToEdgesBoxSizer->AddSpacer(20);
		pSnapToEdgesBoxSizer->Add(m_pSnapToEdgesOfScreenCheckBox, flags);
		pSnapToEdgesBoxSizer->Add(m_pSnapToEdgesOfScreenStaticText, flags);
		pSnapToEdgesBoxSizer->Add(m_pSnapToEdgesOfScreenSpinCtrl, flags);

		m_pSnapToEdgesOfScreenStaticBox->SetSizerAndFit(pSnapToEdgesBoxSizer);

		wxBoxSizer* pSnapToOtherWindowsBoxSizer = new wxBoxSizer(wxVERTICAL);

		pSnapToOtherWindowsBoxSizer->AddSpacer(20);
		pSnapToOtherWindowsBoxSizer->Add(
			m_pSnapToOtherWindowsCheckBox, flags);
		pSnapToOtherWindowsBoxSizer->Add(
			m_pSnapToOtherWindowsStaticText, flags);
		pSnapToOtherWindowsBoxSizer->Add(
			m_pSnapToOtherWindowsSpinCtrl, flags);

		m_pSnapToOtherWindowsStaticBox->SetSizerAndFit(
			pSnapToOtherWindowsBoxSizer);
#endif
	}

	void COptionsDialog::OnRulerTransparencyCheckBoxClicked(
        wxCommandEvent& Event)
	{
		// Retrieve our check box by static casting of Event object.
		wxCheckBox* pCheckBox =
            static_cast<wxCheckBox*>(Event.GetEventObject());

		// If our check box is checked, enable controls bellow. Otherwise
		// disable controls below.
		m_pRulerTransparencyText->Enable(pCheckBox->IsChecked());
		m_pRulerTransparencySlider->Enable(pCheckBox->IsChecked());
	}

	void COptionsDialog::OnVerticalRulerIncreaseButtonClicked(
        wxCommandEvent& Event)
	{
		// Increment vertical PPI value by 1.
		g_vPixelPerInch[0].y++;

		// Update m_pVPPIStaticText label text.
		m_pVPPIStaticText->SetLabel(
			wxString::Format("Vertical PPI: %d", g_vPixelPerInch[0].GetY()));

		// Redraw vertical ruler panel.
		m_pVRulerPanel->Refresh();
	}

	void COptionsDialog::OnVerticalRulerDecreaseButtonClicked(
        wxCommandEvent& Event)
	{
		// Decrement vertical PPI value by 1.
		g_vPixelPerInch[0].y--;

		// Update m_pVPPIStaticText label text.
		m_pVPPIStaticText->SetLabel(
			wxString::Format("Vertical PPI: %d", g_vPixelPerInch[0].GetY()));

		// Redraw vertical ruler panel.
		m_pVRulerPanel->Refresh();
	}

	void COptionsDialog::OnHorizontalRulerIncreaseButtonClicked(
        wxCommandEvent& Event)
	{
		// Increment horizontal PPI value by 1.
		g_vPixelPerInch[0].x++;

		// Update m_pHPPIStaticText label text.
		m_pHPPIStaticText->SetLabel(
			wxString::Format("Horizontal PPI: %d", g_vPixelPerInch[0].GetX()));

		// Redraw horizontal ruler panel.
		m_pHRulerPanel->Refresh();
	}

	void COptionsDialog::OnHorizontalRulerDecreaseButtonClicked(
        wxCommandEvent& Event)
	{
		// Decrement horizontal PPI value by 1.
		g_vPixelPerInch[0].x--;

		// Update m_pHPPIStaticText label text.
		m_pHPPIStaticText->SetLabel(
			wxString::Format("Horizontal PPI: %d", g_vPixelPerInch[0].GetX()));

		// Redraw horizontal ruler panel.
		m_pHRulerPanel->Refresh();
	}

#ifdef WR_WINDOWS // If platform is Windows.
	void COptionsDialog::OnSnapToEdgesOfScreenCheckBoxClicked(
        wxCommandEvent& Event)
	{
		// Retrieve our MainFrame.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Retrieve our check box by static casting of Event object.
		wxCheckBox* pCheckBox =
            static_cast<wxCheckBox*>(Event.GetEventObject());

		// If our check box is checked, enable controls below. Otherwise
		// disable controls below.
		m_pSnapToEdgesOfScreenStaticText->Enable(pCheckBox->IsChecked());
		m_pSnapToEdgesOfScreenSpinCtrl->Enable(pCheckBox->IsChecked());

		// Store snap to edges of the screen enable state.
		pMainFrame->m_bSnapToEdgesOfScreen = pCheckBox->IsChecked();
	}

	void COptionsDialog::OnSnapToOtherWindowsCheckBoxClicked(
        wxCommandEvent& Event)
	{
		// Retrieve CMainFrame pointer.
		CMainFrame* pMainFrame = static_cast<CMainFrame*>(this->GetParent());

		// Retrieve our check box by static casting of Event object.
		wxCheckBox* pCheckBox =
            static_cast<wxCheckBox*>(Event.GetEventObject());

		// If our check box is checked, enable controls below. Otherwise
		// disable controls below.
		m_pSnapToOtherWindowsStaticText->Enable(pCheckBox->IsChecked());
		m_pSnapToOtherWindowsSpinCtrl->Enable(pCheckBox->IsChecked());

		// Store snap to other windows enable state.
		pMainFrame->m_bSnapToOtherWindows = pCheckBox->IsChecked();
	}
#endif

	void COptionsDialog::OnBackgroundTypeChoiceChanged(wxCommandEvent& Event)
	{
		// Depending on selected background type, enable or disable proper
		// controls.
		switch (Event.GetSelection())
		{
		case btSolid:
			m_pBackgroundColourText->Enable(true);
			m_pBackgroundColourPicker->Enable(true);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case btGradient:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(true);
			m_pBackgroundStartColourPicker->Enable(true);
			m_pBackgroundEndColourPicker->Enable(true);
			m_pBackgroundImageText->Enable(false);
			m_pBackgroundImagePicker->Enable(false);

			break;
		case btImage:
			m_pBackgroundColourText->Enable(false);
			m_pBackgroundColourPicker->Enable(false);
			m_pBackgroundStartEndColourText->Enable(false);
			m_pBackgroundStartColourPicker->Enable(false);
			m_pBackgroundEndColourPicker->Enable(false);
			m_pBackgroundImageText->Enable(true);
			m_pBackgroundImagePicker->Enable(true);

			break;
		}
	}

	void COptionsDialog::OnCalibrateUnitOfMeasurementTypeChoiceChanged(
		wxCommandEvent& Event)
	{
		// When this event is triggered, we need to redraw both vertical and
		// horizontal ruler panels.
		m_pVRulerPanel->Refresh();
		m_pHRulerPanel->Refresh();
	}

	void COptionsDialog::OnClose(wxCloseEvent& Event)
	{
		// Destroy class.
		Destroy();
	}
} // end namespace WinRuler
