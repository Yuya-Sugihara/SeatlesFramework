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
	/// �f�o�b�O�����o�͂���
	/// </summary>
	void debugOutPut(const char* format, ...)
	{
		va_list valist;
		va_start(valist, format);
		vprintf(format, valist);
		va_end(valist);
	}

	/// <summary>
	/// ���������[�N�Ɋւ���_���v���o�͂���
	/// </summary>
	void dumpMemoryLeakReport()
	{
		_CrtDumpMemoryLeaks();
	}
}

#endif