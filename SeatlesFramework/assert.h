#pragma once

#include <Windows.h>

namespace SeatlesFramework
{
	//	HRESULTの値に応じて追加情報を付加してassertする
	void throwAssertIfFailed(HRESULT result, const char* comment);
}