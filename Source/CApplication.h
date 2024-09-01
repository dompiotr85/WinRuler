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
