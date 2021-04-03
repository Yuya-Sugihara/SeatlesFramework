#include "assert.h"

#include "debugUtility.h"
#include <cassert>
#include <stdio.h>

void SeatlesFramework::throwAssertIfFailed(HRESULT result, const char* comment)
{
	if (SUCCEEDED(result))
		return;

	//	Tips:  _com_errorが出た場合は変数の型を確認する
	//			変数の型が違う場合でもS_OKが出ることがある
	Trace::traceLog("------------------------------------------------");

	char assertComment[128];
	/// 要確認　変数の型
	if (result == E_ACCESSDENIED)
	{
		sprintf_s(assertComment, "E_ACCESSDENIED: %s", comment);	
		Trace::traceLog(assertComment);
		Trace::traceLog("------------------------------------------------");

		assert(false && "HRESULT:  E_ACCESSDENIED.");
	}
	else if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
	{
		sprintf_s(assertComment, "FILE_NOT_FOUND: %s", comment);
		Trace::traceLog(assertComment);
		Trace::traceLog("------------------------------------------------");

		assert(false && "HRESULT:  FILE_NOT_FOUND.");
	}
	else
	{
		sprintf_s(assertComment, "FAILED: %s", comment);
		Trace::traceLog(assertComment);
		Trace::traceLog("------------------------------------------------");

		assert(false && "HRESULT:  FAILED.");
	}

	
}
