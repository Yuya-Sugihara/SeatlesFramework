#pragma once

#include <Windows.h>

namespace seatlesFramework
{
	//	HRESULT�̒l�ɉ����Ēǉ�����t������assert����
	void throwAssertIfFailed(HRESULT result, const char* comment);
}