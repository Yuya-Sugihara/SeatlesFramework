#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC

#include "debugUtility.h"
#include <stdlib.h>
#include <cstdarg>
#include <crtdbg.h>
#include <string>
#include <Windows.h>

using namespace seatlesFramework;

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

void Trace::traceErrorBlobLog(ID3DBlob* errorBlob)
{
#if _DEBUG
	if (errorBlob == nullptr)
		return;

	std::string errorStr;
	errorStr.resize(errorBlob->GetBufferSize());
	std::copy_n
	( 
		(char*)errorBlob->GetBufferPointer(),
		errorBlob->GetBufferSize(),
		errorStr.begin()
	);

	traceLog(errorStr.c_str());
#endif
}

/// <summary>
/// メモリリークに関するダンプを出力する
/// </summary>
void seatlesFramework::dumpMemoryLeakReport()
{
	_CrtDumpMemoryLeaks();
}


#endif