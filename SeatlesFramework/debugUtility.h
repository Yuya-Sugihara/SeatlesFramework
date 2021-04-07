#pragma once

#include"directX.h"
#include <iostream>
#include <Windows.h>

namespace seatlesFramework
{
	class Trace
	{
	public:
		/// <summary>
		/// �f�o�b�O�E�B���h�E�Ƀg���[�X���O���o�͂���
		/// ����������_DEBUG�ł������Ă���̂ŁA�֐��Ăяo�����Ɉ͂ޕK�v�͂Ȃ�
		/// </summary>
		void static traceLog(const char* format, ...);

		/// <summary>
		/// �G���[Blob�Ɋւ���g���[�X���o�͂���
		/// </summary>
		void static traceErrorBlobLog(ID3D10Blob* errorBlob);
	};

#ifdef _DEBUG

	/// <summary>
	/// ���������[�N�Ɋւ���_���v���o�͂���
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
