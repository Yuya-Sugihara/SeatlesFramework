#pragma once

#include <iostream>

namespace SeatlesFramework
{
	/// <summary>
	/// �f�o�b�O�E�B���h�E�Ƀg���[�X���O���o�͂���
	/// ����������_DEBUG�ł������Ă���̂ŁA�֐��Ăяo�����Ɉ͂ޕK�v�͂Ȃ�
	/// </summary>
	void traceLog(const char* format, ...);

#ifdef _DEBUG

	/// <summary>
	/// ���������[�N�Ɋւ���_���v���o�͂���
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
