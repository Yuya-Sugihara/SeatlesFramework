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
		/// デバッグウィンドウにトレースログを出力する
		/// 内部処理を_DEBUGでくくっているので、関数呼び出し時に囲む必要はない
		/// </summary>
		void static traceLog(const char* format, ...);

		/// <summary>
		/// エラーBlobに関するトレースを出力する
		/// </summary>
		void static traceErrorBlobLog(ID3D10Blob* errorBlob);
	};

#ifdef _DEBUG

	/// <summary>
	/// メモリリークに関するダンプを出力する
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
