#pragma once

#include <iostream>

namespace SeatlesFramework
{

#ifdef _DEBUG

	/// <summary>
	/// デバッグ情報を出力する
	/// </summary>
	void debugOutPut(const char* format, ...);

	/// <summary>
	/// メモリリークに関するダンプを出力する
	/// </summary>
	void dumpMemoryLeakReport();

#endif

}
