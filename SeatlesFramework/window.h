#pragma once

#include <Windows.h>

namespace SeatlesFramework
{
	/// <summary>
	/// windowに関するクラス
	/// </summary>
	class Window
	{
	public: 

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Window(const wchar_t* applicationName,int windowWidth,int windowHeight);

		/// <summary>
		/// ウィンドウ更新処理　ウィンドウが終了したときtrueを返す
		/// </summary>
		bool update();

	private: 

		/// <summary>
		/// ウィンドウの初期化処理
		/// </summary>
		HRESULT init(const wchar_t* applicationName, int windowWidth, int windowHeight);

		WNDCLASSEX mWindowDesc;

	};

	/// <summary>
	/// メッセージ処理
	/// </summary>
	LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}