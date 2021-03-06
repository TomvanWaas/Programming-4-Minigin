#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <tchar.h>
#include <memory> // smart pointers
#include <vector>


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Logger.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(a) if(a != nullptr) {delete a; a = nullptr;}
#endif


