/**
 * Name:		StdAfx.h
 * Purpose:
 * Author:		King
 * Company:
 * Created:		04/05/2011 21:11:11
 * Copyright:
 */

#pragma once

#include "config.h"

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <WindowsX.h>
#include <ShellAPI.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <strsafe.h>

#include "defines.h"

#include "base/ktypes.h"
#include "base/kobject.h"
#include "base/kstdpath.h"
#include "base/kassert.h"
#include "base/klog.h"
#include "base/kerrno.h"
#include "base/kdefines.h"
#include "base/kmemory.h"


#define AUTHOR		TEXT("King")
#define APP_NAME	TEXT("Assistant")
