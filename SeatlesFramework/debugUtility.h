#pragma once

#include <iostream>

namespace SeatlesFramework
{

#ifdef _DEBUG

	/// <summary>
	/// �f�o�b�O�����o�͂���
	/// </summary>
	void debugOutPut(const char* format, ...);

	/// <summary>
	/// ���������[�N�Ɋւ���_���v���o�͂���
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
