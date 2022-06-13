// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Includes SDKDDKVer.h
#include <SDKDDKVer.h>		

 // Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN            


// Windows Header Files:
#include <Windows.h>
#include <d3d11.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <codecvt>
#include <unordered_map>
#include <queue>
#include <chrono>


#include "DbgAssert.h"
