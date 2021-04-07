#pragma once

#include <Windows.h>

namespace seatlesFramework
{
	//	HRESULT‚Ì’l‚É‰‚¶‚Ä’Ç‰Áî•ñ‚ğ•t‰Á‚µ‚Äassert‚·‚é
	void throwAssertIfFailed(HRESULT result, const char* comment);
}