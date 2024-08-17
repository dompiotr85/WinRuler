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

#include "WRUtilities.h"

namespace WinRuler
{
	std::vector<wxSize> g_vPixelPerInch = { wxSize(0, 0) };

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
		return (int) (std::trunc(GetPixelPerInch(DisplayNo).GetX() * AInchDistance));
	}

	int InchesToPixelsVertical(unsigned int DisplayNo, double AInchDistance)
	{
		// Calculate how many pixels are in specified distance (in inches) and
		// return result.
		return (int) (std::trunc(GetPixelPerInch(DisplayNo).GetY() * AInchDistance));
	}

	double PixelsToInchesHorizontal(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculate how many inches are in specified pixel distance and return
		// result.
		return (double) ((double) APixelDistance / (double) GetPixelPerInch(DisplayNo).GetX());
	}

	double PixelsToInchesVertical(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculate how many inches are in specified pixel distance and return
		// result.
		return (double) ((double) APixelDistance / (double) GetPixelPerInch(DisplayNo).GetY());
	}

	int CentimetresToPixelsHorizontal(unsigned int DisplayNo, double ACentimetreDistance)
	{
		// Calculate how many pixels are in specified distance (in centimetres)
		// and return result.
		return (int) (std::trunc(GetPixelPerInch(DisplayNo).GetX() / 2.54) * ACentimetreDistance);
	}

	int CentimetresToPixelsVertical(unsigned int DisplayNo, double ACentimetreDistance)
	{
		// Calculate how many pixels are in specified distance (in centimetres)
		// and return result.
		return (int) (std::trunc(GetPixelPerInch(DisplayNo).GetY() / 2.54) * ACentimetreDistance);
	}

	double PixelsToCentimetresHorizontal(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many centimetres are in specified pixel distance and
		// return result.
		return (double) ((double) APixelDistance / (double) ((double) GetPixelPerInch(DisplayNo).GetX() / 2.54));
	}

	double PixelsToCentimetresVertical(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many centimetres are in specified pixel distance and
		// return result.
		return (double) ((double) APixelDistance / (double) ((double) GetPixelPerInch(DisplayNo).GetY() / 2.54));
	}

	int PicasToPixelsHorizontal(unsigned int DisplayNo, double APicasDistance)
	{
		// Calculates how many pixels are in specified distance (in picas) and
		// return result.
		return (int) ((GetPixelPerInch(DisplayNo).GetX() / 6.0) * APicasDistance);
	}

	int PicasToPixelsVertical(unsigned int DisplayNo, double APicasDistance)
	{
		// Calculates how many pixels are in specified distance (in picas) and
		// return result.
		return (int) ((GetPixelPerInch(DisplayNo).GetY() / 6.0) * APicasDistance);
	}

	double PixelsToPicasHorizontal(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many picas are in specified pixel distance and return
		// result.
		return (double) ((double) APixelDistance / (double) ((double) GetPixelPerInch(DisplayNo).GetX() / 6.0));
	}

	double PixelsToPicasVertical(unsigned int DisplayNo, int APixelDistance)
	{
		// Calculates how many picas are in specified pixel distance and return
		// result.
		return (double) ((double) APixelDistance / (double) ((double) GetPixelPerInch(DisplayNo).GetY() / 6.0));
	}
} // end namespace WinRuler
