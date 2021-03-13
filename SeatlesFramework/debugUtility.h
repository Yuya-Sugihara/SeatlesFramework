#pragma once

#include <iostream>
#include <Windows.h>

namespace SeatlesFramework
{
	class Trace
	{
	public:
		/// <summary>
		/// デバッグウィンドウにトレースログを出力する
		/// 内部処理を_DEBUGでくくっているので、関数呼び出し時に囲む必要はない
		/// </summary>
		void static traceLog(const char* format, ...);

		//void static traceLog(const TCHAR* comment);
	};

#ifdef _DEBUG

	/// <summary>
	/// メモリリークに関するダンプを出力する
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
