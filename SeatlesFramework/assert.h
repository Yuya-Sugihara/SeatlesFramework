#pragma once

#include <Windows.h>

namespace SeatlesFramework
{
	//	HRESULT�̒l�ɉ����Ēǉ�����t������assert����
	void throwAssertIfFailed(HRESULT result, const char* comment);
}