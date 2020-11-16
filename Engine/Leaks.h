#pragma once
#ifdef _DEBUG
#define MYDEBUG_NEW new(_NORMAL_BLOCK, __FILE__,__LINE__)
#endif
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define new MYDEBUG_NEW
#endif

//This header must be included on every .cpp in order to get all information