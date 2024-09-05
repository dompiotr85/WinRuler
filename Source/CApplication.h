/**
 * Copyright © 2024 Piotr Domañski
 * Licensed under the MIT license.
 **/

#pragma once

#include <wx/wx.h>
#include <CMainFrame.h>

namespace WinRuler
{
	/**
	 * Application class definition. 
	 **/
	class CApplication :
		public wxApp
	{
	public:
		/**
		 * This function is called when application is initialized and returns
		 * true if initialization process was successful.
		 **/
		bool OnInit() override;

		/**
		 * This function is called when application is closed.
		 **/
		int OnExit() override;

		/**
		 * Override FilterEvent method for our mouse events pre-processing.
		 **/
		int FilterEvent(wxEvent& Event);
	public:
		// Pointer to CMainFrame instance.
		CMainFrame* m_pMainFrame;

		// Pointer to wxIcon.
		wxIcon* m_pIcon;

		wxLog* m_pLogger;
	};
} // end namespace WinRuler
