#pragma once

#include <iostream>
#include <Windows.h>

namespace SeatlesFramework
{
	class Trace
	{
	public:
		/// <summary>
		/// �f�o�b�O�E�B���h�E�Ƀg���[�X���O���o�͂���
		/// ����������_DEBUG�ł������Ă���̂ŁA�֐��Ăяo�����Ɉ͂ޕK�v�͂Ȃ�
		/// </summary>
		void static traceLog(const char* format, ...);

		//void static traceLog(const TCHAR* comment);
	};

#ifdef _DEBUG

	/// <summary>
	/// ���������[�N�Ɋւ���_���v���o�͂���
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
