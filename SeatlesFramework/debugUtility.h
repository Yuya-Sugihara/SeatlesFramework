#pragma once

#include <iostream>

namespace SeatlesFramework
{
	/// <summary>
	/// デバッグウィンドウにトレースログを出力する
	/// 内部処理を_DEBUGでくくっているので、関数呼び出し時に囲む必要はない
	/// </summary>
	void traceLog(const char* format, ...);

#ifdef _DEBUG

	/// <summary>
	/// メモリリークに関するダンプを出力する
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
