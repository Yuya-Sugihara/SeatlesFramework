#include "assert.h"

#include "debugUtility.h"
#include <cassert>
#include <stdio.h>

void SeatlesFramework::throwAssertIfFailed(HRESULT result, const char* comment)
{
	if (SUCCEEDED(result))
		return;

	//	Tips:  _com_error���o���ꍇ�͕ϐ��̌^���m�F����
	//			�ϐ��̌^���Ⴄ�ꍇ�ł�S_OK���o�邱�Ƃ�����
	Trace::traceLog("------------------------------------------------");

	char assertComment[128];
	/// �v�m�F�@�ϐ��̌^
	if (result == E_ACCESSDENIED)
	{
		sprintf_s(assertComment, "E_ACCESSDENIED: %s", comment);	
		Trace::traceLog(assertComment);
		Trace::traceLog("------------------------------------------------");

		assert(false && "HRESULT:  E_ACCESSDENIED.");
	}
	else
	{
		sprintf_s(assertComment, "FAILED: %s", comment);
		Trace::traceLog(assertComment);
		Trace::traceLog("------------------------------------------------");

		assert(false && "HRESULT:  FAILED.");
	}

	
}
