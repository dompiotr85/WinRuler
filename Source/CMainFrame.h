/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <map>
#include <wx/wx.h>
#include <sqlite3.h>

#include "WRUtilities.h"
#include "CDrawPanel.h"
#include "CAboutDialog.h"
#include "CNewRulerLengthDialog.h"
#include "COptionsDialog.h"

enum HT_Pos
{
	HT_client,
	HT_left,
	HT_right,
	HT_top,
	HT_topLeft,
	HT_topRight,
	HT_bottom,
	HT_bottomLeft,
	HT_bottomRight
};

enum EMenuIDs
{
	ID_RULER_POSITION = 10,
	ID_RULER_POSITION_SCALE_ON_LEFT = 11,
	ID_RULER_POSITION_SCALE_ON_RIGHT = 12,
	ID_RULER_POSITION_SCALE_ON_TOP = 13,
	ID_RULER_POSITION_SCALE_ON_BOTTOM = 14,
	ID_PIXELS_AS_UNIT = 15,
	ID_CENTIMETRES_AS_UNIT = 16,
	ID_INCHES_AS_UNIT = 17,
	ID_PICAS_AS_UNIT = 18,
	ID_ALWAYS_ON_TOP = 19,
	ID_NEW_RULER_LENGTH = 20,
	ID_OPTIONS = 21
};

namespace WinRuler
{
	/**
	 * MainFrame class definition.
	 **/
	class CMainFrame :
		public wxFrame
	{
		DECLARE_EVENT_TABLE()
	private:
		/**
		 * PopupMenu event handling methods.
		 **/
		void OnOptionsClicked(wxCommandEvent& Event);
		void OnNewRulerLengthClicked(wxCommandEvent& Event);
		void OnPixelsAsUnitClicked(wxCommandEvent& Event);
		void OnCentimetresAsUnitClicked(wxCommandEvent& Event);
		void OnInchesAsUnitClicked(wxCommandEvent& Event);
		void OnPicasAsUnitClicked(wxCommandEvent& Event);
		void OnAlwaysOnTopClicked(wxCommandEvent& Event);
		void OnScaleOnLeftClicked(wxCommandEvent& Event);
		void OnScaleOnTopClicked(wxCommandEvent& Event);
		void OnScaleOnRightClicked(wxCommandEvent& Event);
		void OnScaleOnBottomClicked(wxCommandEvent& Event);
		void OnAboutClicked(wxCommandEvent& Event);
		void OnCloseClicked(wxCommandEvent& Event);
	public:
		/**
		 * CMainFrame constructor.
		 *
		 * \param Title		Reference to MainFrame title string.
		 **/
		CMainFrame(const wxString& Title);

		/**
		 * Default destructor.
		 **/
		~CMainFrame();

		/**
		 * Initialize MainFrame.
		 **/
		void Init();

		/**
		 * Creates all controls for CMainFrame.
		 **/
		void CreateControls();

		/**
		 * Setup sizers.
		 **/
		void SetupSizers();

		/**
		 * This method should be called for proper change of ruler's position.
		 *
		 * \param NewPosition		New ruler's position that will be set.
		 **/
		void ChangeRulerPosition(ERulerPosition NewPosition);

		/**
		 * This method should be called for proper change of StayOnTop state.
		 *
		 * \param State		New StayOnTop state.
		 **/
		void StayOnTop(bool State);

		/**
		 * This method should be called for proper change of ruler's unit of
		 * measurement.
		 *
		 * \param NewUnit	New ruler's unit of measurement.
		 **/
		void ChangeRulerUnitOfMeasurement(ERulerUnits NewUnit);

		/**
		 * This method should be called for proper change of the ruler's length.
		 *
		 * \param NewLength		New ruler's length.
		 **/
		void ChangeRulerLength(int NewLength);

		/**
		 * Loads and prepare ruler's images for background image type drawing.
		 *
		 * \return	Returns true if ruler's background image was loaded
		 *			properly, otherwise false.
		 **/
		bool LoadAndPrepareRulerBackgroundImage();

		/**
		 * This method loads all settings of the application from sqlite
		 * database. If application is executed for the first time, there
		 * are no settings in sqlite database so new table and entries will be
		 * create.
		 **/
		bool LoadSettingsFromDatabase(
			const wxString& dbPath, std::map<wxString, wxString>& Settings);

		/**
		 * This method save all settings of the application into sqlite
		 * database.
		 **/
		bool SaveSettingsToDatabase(
			const wxString& dbPath, const std::map<wxString, wxString>& Settings);

		/**
		 * Loads all settings of our application.
		 **/
		void LoadApplicationSettings();

		/**
		 * Saves all settings of our application.
		 **/
		void SaveApplicationSettings();

		/**
		 * Performs snapping to edges of the screen.
		 *
		 * \param Pos		Reference to current position as wxPoint instance.
		 **/
		void SnapToEdges(wxPoint& Pos);

		/**
		 * Performs snapping to other windows.
		 **/
		void SnapToOtherWindows();
	public:
		/**
		 * OnExit() method event.
		 *
		 * \param Event		Reference to wxCommandEvent instance.
		 */
		void OnExit(wxCommandEvent& Event);

		/**
		 * OnClose() method event.
		 *
		 * \param Event		Reference to wxCloseEvent instance.
		 **/
		void OnClose(wxCloseEvent& Event);

		/**
		 * OnMouseEvent() method event.
		 *
		 * \param Event		Reference to wxMouseEvent instance.
		 **/
		void OnMouseEvent(wxMouseEvent& Event);

		/**
		 * OnMove() method event.
		 *
		 * \param Event		Reference to wxMoveEvent instance.
		 **/
		void OnMove(wxMoveEvent& Event);
	protected:
		/**
		 * Performs initialization of border dragging.
		 **/
		void BorderDragInit();

		/**
		 * Performs border hit test and returns hit position.
		 *
		 * \param Pos		Reference to wxPoint instance.
		 *
		 * @return	Returns hit position described as HT_Pos.
		 **/
		int BorderHitTest(const wxPoint& Pos);

		/**
		 * Sets resize cursor for specified hit position.
		 *
		 * \param htPos		Hit position specified.
		 **/
		void SetResizeCursor(int HitPos);

		/**
		 * This method is called when mouse is leaving border specified by
		 * hit position.
		 *
		 * \param hitPos	Hit position specified.
		 **/
		void OnLeaveBorder(int HitPos);

		/**
		 * This method is called when mouse is entering border.
		 **/
		void OnEnterBorder();

		/**
		 * This method is called when
		 **/
		void ResizeSize(const wxPoint& Pos);
	public:
		//! Ruler's scale position.
		ERulerPosition m_eRulerPosition = rpTop;

		//! Ruler's unit of measurement.
		ERulerUnits m_eRulerUnits = ruCentimetres;

		//! Ruler's background type.
		ERulerBackgroundType m_eRulerBackgroundType = btSolid;

		//! Ruler's scale colour.
		wxColour m_cRulerScaleColour = wxColour(0, 0, 0);

		//! Ruler's background colour (used in btSolid background type).
		wxColour m_cRulerBackgroundColour = wxColour(255, 164, 119);

		//! Ruler's background start colour (used in btGradient background
		//! type).
		wxColour m_cRulerBackgroundStartColour = wxColour(255, 196, 119);

		//! Ruler's background end colour (used in btGradient background type).
		wxColour m_cRulerBackgroundEndColour = wxColour(255, 142, 61);

		//! Ruler's background image that act as skin.
		wxString m_sRulerBackgroundImagePath = wxString("");

		//! wxBitmaps for clipped background image. Those bitmaps are used for
		//! drawing ruler when m_eRulerBackgroundType is in btImage mode.
		wxBitmap m_RulerBackgroundBitmapLeftH;
		wxBitmap m_RulerBackgroundBitmapMiddleH;
		wxBitmap m_RulerBackgroundBitmapRightH;
		wxBitmap m_RulerBackgroundBitmapTopV;
		wxBitmap m_RulerBackgroundBitmapMiddleV;
		wxBitmap m_RulerBackgroundBitmapBottomV;

		//! Ruler's length.
		int m_iRulerLength = 800;

		//! Ruler's minimum length limit.
		int m_iRulerMinimumLengthLimit = 100;

		//! Ruler AlwaysOnTop state.
		bool m_bAlwaysOnTop = true;

		//! Ruler transparency state.
		bool m_bRulerTransparency = false;

		//! Ruler transparency value.
		wxByte m_iRulerTransparencyValue = 255;

		//! First marker position.
		int m_iFirstMarkerPosition = 0;

		//! Second marker position.
		int m_iSecondMarkerPosition = -1;

		//! First marker colour.
		wxColour m_cFirstMarkerColour = wxColour(0, 0, 255);

		//! Second marker colour.
		wxColour m_cSecondMarkerColour = wxColour(255, 0, 0);

		//! Snap to edges of the screen state.
		bool m_bSnapToEdgesOfScreen = true;

		//! Snap to edges of visible windows state.
		bool m_bSnapToOtherWindows = true;

		//! Snap to edges of the screen distance value.
		int m_iSnapToEdgesOfScreenDistance = 4;

		//! Snap to edges of visible windows distance value.
		int m_iSnapToOtherWindowsDistance = 4;
	public:
		//! Pointer to CDrawPanel instance, which is used for all drawing of
		//! our ruler.
		CDrawPanel* m_pDrawPanel;

		//! Pointer to CAboutDialog instance.
		CAboutDialog* m_pAboutDialog;

		//! Pointer to CNewRulerLengthDialog instance.
		CNewRulerLengthDialog* m_pNewRulerLengthDialog;

		//! Pointer to COptionsDialog instance.
		COptionsDialog* m_pOptionsDialog;
	public:
		//! Is caption drag starts operation.
		bool m_bCaptionDragStart;

		//! Mouse position when dragging started.
		wxPoint m_ptDragStart;

		//! Default width of the border.
		int	m_iOffsetBorder;

		//! Record the hit position when resizing the frame.
		int m_eBorderDragMode;

		//! Border rectangle values when drag beginning.
		wxRect m_rectBorder;

		//! Direction vector.
		wxPoint m_ptDirection;
	};
} // end namespace WinRuler
