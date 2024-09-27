/**
 * Copyright © 2024 Piotr Domanski
 * Licensed under the MIT license.
 **/

#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <wx/wx.h>

namespace WinRuler
{
	/*-------------------------------------------------------------------------
	  Mathematical routines.
	-------------------------------------------------------------------------*/

	/**
	 * Portable comparison function for double float numbers.
	 *
	 * @param a		First double float number that will be compared.
	 * @param b		Second double float number that will be compared.
	 *
	 * @return Returns true if a and b numbers are almost equal, false otherwise.
	 **/
	bool AreSame(double a, double b, double epsilon = std::numeric_limits<double>::epsilon());

	/*-------------------------------------------------------------------------
	  Unit of measurement conversion routines.
	-------------------------------------------------------------------------*/

	extern std::vector<wxSize> g_vPixelPerInch;

	/**
	 * Sets new PixelPerInch value.
	 *
	 * @param NewValue	New value that will be stored as current PixelPerInch
	 *					value.
	 **/
	void SetPixelPerInch(unsigned int DisplayNo, wxSize& NewPPIValue);

	/**
	 * Gets PixelPerInch value.
	 *
	 * @return	Returns current PixelPerInch value.
	 **/
	wxSize& GetPixelPerInch(unsigned int DisplayNo);

	/**
	 * Calculates conversion from inches to pixels (horizontal version).
	 *
	 * @param AInchDistance		Inch distance that will be calculated to
	 *							pixels.
	 *
	 * @return	Returns result of inches to pixels calculation.
	 **/
	int InchesToPixelsHorizontal(unsigned int DisplayNo, double AInchDistance);

	/**
	 * Calculates conversion from inches to pixels (vertical version).
	 *
	 * @param AInchDistance		Inch distance that will be calculated to
	 *							pixels.
	 *
	 * @return	Returns result of inches to pixels calculation.
	 **/
	int InchesToPixelsVertical(unsigned int DisplayNo, double AInchDistance);

	/**
	 * Calculates conversion from pixels to inches (horizontal version).
	 *
	 * @param APixelDistance	Pixel distance that will be calculated to
	 *							inches.
	 *
	 * @return	Returns result of pixels to inches calculation.
	 **/
	double PixelsToInchesHorizontal(unsigned int DisplayNo, int APixelDistance);

	/**
	 * Calculates conversion from pixels to inches (vertical version).
	 *
	 * @param APixelDistance	Pixel distance that will be calculated to
	 *							inches.
	 *
	 * @return	Returns result of pixels to inches calculation.
	 **/
	double PixelsToInchesVertical(unsigned int DisplayNo, int APixelDistance);

	/**
	 * Calculates conversion form centimetres to pixels (horizontal version).
	 *
	 * @param ACentimetreDistance	Centimetre distans that will be calculated to
	 *								pixels.
	 *
	 * @return	Returns result of centimetres to pixels calculation.
	 **/
	int CentimetresToPixelsHorizontal(unsigned int DisplayNo, double ACentimetreDistance);

	/**
	 * Calculates conversion form centimetres to pixels (vertical version).
	 *
	 * @param ACentimetreDistance	Centimetre distans that will be calculated to
	 *								pixels.
	 *
	 * @return	Returns result of centimetres to pixels calculation.
	 **/
	int CentimetresToPixelsVertical(unsigned int DisplayNo, double ACentimetreDistance);

	/**
	 * Calculates conversion from pixels to centimetres (horizontal version).
	 *
	 * @param APixelDistance	Pixel distance that will be calculated to
	 *							centimetres.
	 * @return	Returns result of pixels to centimetres calculation.
	 **/
	double PixelsToCentimetresHorizontal(unsigned int DisplayNo, int APixelDistance);

	/**
	 * Calculates conversion from pixels to centimetres (vertical version).
	 *
	 * @param APixelDistance	Pixel distance that will be calculated to
	 *							centimetres.
	 * @return	Returns result of pixels to centimetres calculation.
	 **/
	double PixelsToCentimetresVertical(unsigned int DisplayNo, int APixelDistance);

	/**
	 * Calculates conversion from picas to pixels (horizontal version).
	 *
	 * @param APicasDistance	Picas distance that will be calculated to pixels.
	 *
	 * @return	Returns result of picas to pixels calculation.
	 **/
	int PicasToPixelsHorizontal(unsigned int DisplayNo, double APicasDistance);

	/**
	 * Calculates conversion from picas to pixels (vertical version).
	 *
	 * @param APicasDistance	Picas distance that will be calculated to pixels.
	 *
	 * @return	Returns result of picas to pixels calculation.
	 **/
	int PicasToPixelsVertical(unsigned int DisplayNo, double APicasDistance);

	/**
	 * Calculates conversion from pixels to picas (horizontal version).
	 *
	 * @param APixelDistance	Pixel distance that will be calculated to picas.
	 *
	 * @return	Returns result of pixels to picas calculation.
	 **/
	double PixelsToPicasHorizontal(unsigned int DisplayNo, int APixelDistance);

	/**
	 * Calculates conversion from pixels to picas (vertical version).
	 *
	 * @param APixelDistance	Pixel distance that will be calculated to picas.
	 *
	 * @return	Returns result of pixels to picas calculation.
	 **/
	double PixelsToPicasVertical(unsigned int DisplayNo, int APixelDistance);
}
