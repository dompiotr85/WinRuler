/**
 * Copyright © 2024-2025 Piotr Domanski
 * Licensed under the MIT license.
 **/

#ifndef _WRPLATFORM_H_
#define _WRPLATFORM_H_

#if (defined(_WIN32) || defined(_WIN64) || defined(WIN32) ||	\
	defined(WIN64) || defined(CYGWIN))
 // If platform is all supported Windows versions.
# define WR_WINDOWS
#elif (defined(__unix__))
 // If platform is Linux, BSD, or any other Unix-like system.
# define WR_LINUX
#elif (defined(__APPLE__) || defined(__MACH__))
// If platform is macOS.
# define WR_MACOS
#else
// If platform is not supported.
# error "Unsupported platform!"
#endif

#endif // _WRPLATFORM_H_
