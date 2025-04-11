/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#include "WRUtilities.h"

namespace WinRuler
{
	std::vector<wxSize> g_vPixelPerInch( { wxSize(0, 0) } );

	bool AreSame(double a, double b, double epsilon)
	{
		return (std::fabs(a - b) < epsilon);
	}

	void SetPixelPerInch(unsigned int DisplayNo, wxSize& NewPPIValue)
	{
		// If NewValue is different than current PixelPerInch value, then
		// change it.
		if ((g_vPixelPerInch[DisplayNo].x != NewPPIValue.x) &&
			(g_vPixelPerInch[DisplayNo].y != NewPPIValue.y))
			g_vPixelPerInch[DisplayNo] = NewPPIValue;
	}

	wxSize& GetPixelPerInch(unsigned int DisplayNo)
	{
		// Return current PixelPerInch value.
		return g_vPixelPerInch[DisplayNo];
	}

	int InchesToPixelsHorizontal(unsigned int DisplayNo, double AInchDistance)
	{
		// Calculate how many pixels are in specified distance (in inches) and
		// return result.
		return static_cast<int>(
			std::trunc(GetPixelPerInch(DisplayNo).GetX() * AInchDistance));
	}

	int InchesToPixelsVertical(unsigned int DisplayNo, double AInchDistance)
	{
		// Calculate how many pixels are in specified distance (in inches) and
		// return result.
		return static_cast<int>(
			std::trunc(GetPixelPerInch(DisplayNo).GetY() * AInchDistance));
	}

	double PixelsToInchesHorizontal(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculate how many inches are in specified pixel distance and return
		// result.
		return static_cast<double>(
			static_cast<double>(APixelDistance) /
			static_cast<double>(GetPixelPerInch(DisplayNo).GetX()));
	}

	double PixelsToInchesVertical(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculate how many inches are in specified pixel distance and return
		// result.
		return static_cast<double>(
			static_cast<double>(APixelDistance) /
			static_cast<double>(GetPixelPerInch(DisplayNo).GetY()));
	}

	int CentimetresToPixelsHorizontal(
		unsigned int DisplayNo, double ACentimetreDistance)
	{
		// Calculate how many pixels are in specified distance (in centimetres)
		// and return result.
		return static_cast<int>(
			std::trunc(GetPixelPerInch(DisplayNo).GetX() / 2.54) * 
			ACentimetreDistance);
	}

	int CentimetresToPixelsVertical(
		unsigned int DisplayNo, double ACentimetreDistance)
	{
		// Calculate how many pixels are in specified distance (in centimetres)
		// and return result.
		return static_cast<int>(
			std::trunc(GetPixelPerInch(DisplayNo).GetY() / 2.54) * 
			ACentimetreDistance);
	}

	double PixelsToCentimetresHorizontal(
		unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many centimetres are in specified pixel distance and
		// return result.
		return static_cast<double>(
			static_cast<double>(APixelDistance) /
			static_cast<double>(
				static_cast<double>(
					GetPixelPerInch(DisplayNo).GetX() / 2.54)));
	}

	double PixelsToCentimetresVertical(
		unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many centimetres are in specified pixel distance and
		// return result.
		return static_cast<double>(
			static_cast<double>(APixelDistance) /
			static_cast<double>(
				static_cast<double>(
					GetPixelPerInch(DisplayNo).GetY() / 2.54)));
	}

	int PicasToPixelsHorizontal(unsigned int DisplayNo, double APicasDistance)
	{
		// Calculates how many pixels are in specified distance (in picas) and
		// return result.
		return static_cast<int>(
			(GetPixelPerInch(DisplayNo).GetX() / 6.0) * 
			APicasDistance);
	}

	int PicasToPixelsVertical(unsigned int DisplayNo, double APicasDistance)
	{
		// Calculates how many pixels are in specified distance (in picas) and
		// return result.
		return static_cast<int>(
			(GetPixelPerInch(DisplayNo).GetY() / 6.0) * APicasDistance);
	}

	double PixelsToPicasHorizontal(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many picas are in specified pixel distance and return
		// result.
		return static_cast<double>(
			static_cast<double>(APixelDistance) /
			static_cast<double>(
				static_cast<double>(GetPixelPerInch(DisplayNo).GetX() / 6.0)));
	}

	double PixelsToPicasVertical(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many picas are in specified pixel distance and return
		// result.
		return static_cast<double>(
			static_cast<double>(APixelDistance) /
			static_cast<double>(
				static_cast<double>(GetPixelPerInch(DisplayNo).GetY() / 6.0)));
	}

	wxPoint ParsePosition(const wxString& PositionString)
	{
		int x = 0, y = 0;

		// Use sscanf for value parsing.
		if (std::sscanf(PositionString.c_str(), "%d:%d", &x, &y) == 2)
		{
			// Return our parsed position as wxPoint.
			return wxPoint(x, y);
		}
		else
		{
			// Handle bad format.
			wxLogError(
				"Invalid format for position string: ""%s""", PositionString);

			return wxPoint(0, 0);
		}
	}

#if (defined(_WIN32) || defined(WIN32))	// If platform is Windows.
	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		std::vector<WindowInfo>* WindowsVec =
			reinterpret_cast<std::vector<WindowInfo>*>(lParam);

		// We are checking only visible window.
		if (IsWindowVisible(hwnd))
		{
			RECT Rect;
			if (GetWindowRect(hwnd, &Rect))
			{
				WindowsVec->emplace_back(WindowInfo{ Rect, hwnd });
			}
		}

		// Continue enumeration.
		return TRUE;
	}

	std::vector<WindowInfo> GetAllWindows()
	{
		std::vector<WindowInfo> WindowsVec;

		EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&WindowsVec));

		return WindowsVec;
	}
#endif
} // end namespace WinRuler
