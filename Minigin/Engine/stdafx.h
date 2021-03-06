// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <tchar.h>
#include <memory> // smart pointers
#include <vector>


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "Logger.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(a) if(a != nullptr) {delete a; a = nullptr;}
#endif



////Define "Debug" To Show Render Colliders
//#ifndef Debug
//#define Debug
//#endif

////Define "LOG_CONSOLE" To SetPath logs possible to the console
#define LOG_CONSOLE

////Define "LOG_FILE" To make logs possible to a file
#define LOG_FILE


// reference additional headers your program requires here
