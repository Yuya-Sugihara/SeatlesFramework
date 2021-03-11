#include "assert.h"

#include "debugUtility.h"
#include <cassert>
#include <stdio.h>

void SeatlesFramework::throwAssertIfFailed(HRESULT result, const char* comment)
{
	if (result == S_OK)
		return;

	traceLog("------------------------------------------------");

	char assertComment[128];
	/// 要確認　変数の型
	if (result == E_ACCESSDENIED)
	{
		sprintf_s(assertComment, "E_ACCESSDENIED: %s", comment);	
		traceLog(assertComment);
		traceLog("------------------------------------------------");

		assert(false && "HRESULT:  E_ACCESSDENIED.");
	}
	else
	{
		sprintf_s(assertComment, "FAILED: %s", comment);
		traceLog(assertComment);
		traceLog("------------------------------------------------");

		assert(false && "HRESULT:  FAILED.");
	}

	
}
