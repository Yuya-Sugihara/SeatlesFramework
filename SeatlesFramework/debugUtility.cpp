#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC

#include "debugUtility.h"
#include <stdlib.h>
#include <cstdarg>
#include <crtdbg.h>
#include <string>
#include <Windows.h>

using namespace SeatlesFramework;
using namespace std;

void Trace::traceLog(const char* format, ...)
{
#if _DEBUG
	char log[256];
	va_list ap;

	va_start(ap,format);
	vsprintf_s(log, format, ap);
	strcat_s(log, "\n");
	OutputDebugStringA(log);
	va_end(ap);
#endif
}

//void Trace::traceLog(const TCHAR* comment)
//{
//#if _DEBUG
//
//	OutputDebugString(comment);
//
//#endif
//}

/// <summary>
/// メモリリークに関するダンプを出力する
/// </summary>
void SeatlesFramework::dumpMemoryLeakReport()
{
	_CrtDumpMemoryLeaks();
}


#endif