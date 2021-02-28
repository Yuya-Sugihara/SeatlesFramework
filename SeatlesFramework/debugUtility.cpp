#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC

#include "debugUtility.h"
#include <stdlib.h>
#include <cstdarg>
#include <crtdbg.h>

using namespace std;

namespace SeatlesFramework
{

	/// <summary>
	/// デバッグ情報を出力する
	/// </summary>
	void debugOutPut(const char* format, ...)
	{
		va_list valist;
		va_start(valist, format);
		vprintf(format, valist);
		va_end(valist);
	}

	/// <summary>
	/// メモリリークに関するダンプを出力する
	/// </summary>
	void dumpMemoryLeakReport()
	{
		_CrtDumpMemoryLeaks();
	}
}

#endif