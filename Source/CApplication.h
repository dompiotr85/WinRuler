/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>

#include <CMainFrame.h>

namespace WinRuler
{
	/**
	 * CApplication class definition.
	 **/
	class CApplication :
		public wxApp
	{
	public:
		/**
		 * This function is called when application is initialized and returns
		 * true if initialization process was successful.
		 * 
		 * \return	Returns true if initialization process was successful,
		 *			otherwise false.
		 **/
		bool OnInit() override;

		/**
		 * This function is called when application is closed.
		 * 
		 * \return	Returns 0 if application is closed successfully, otherwise
		 *			error code is returned.
		 **/
		int OnExit() override;

		/**
		 * Override FilterEvent method for our mouse events pre-processing.
		 * 
		 * \param Event		Reference to wxEvent object.
		 **/
		int FilterEvent(wxEvent& Event);

		/**
		 * Detects that our application is executed for the first time.
		 *
		 * \return	Returns true if our application is executed for the first
		 *			time, otherwise false.
		 *
		 * \note	This function determines that our application is executed
		 *			for the first time by checking that application database
		 *			file exists or not. If database file don't exists, it means
		 *			that our application is executed for the first time.
		 **/
		bool ApplicationExecutedForTheFirstTime();
	public:
		//! Pointer to CMainFrame instance.
		CMainFrame* m_pMainFrame;

		//! Pointer to wxIcon.
		wxIcon* m_pIcon;

		//! Pointer to wxLog.
		wxLog* m_pLogger;
	};
} // end namespace WinRuler
