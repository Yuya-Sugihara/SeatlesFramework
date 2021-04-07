#pragma once

#include <Windows.h>

namespace seatlesFramework
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

		/// アクセサ
		const HWND getWindowHandle()const { return mWindowHandle; }
		const int getWindowWidth() const { return mWindowWidth; }
		const int getWindowHeight() const { return mWindowHeight; }

	private: 

		/// <summary>
		/// ウィンドウの初期化処理
		/// </summary>
		HRESULT init(const wchar_t* applicationName);

		WNDCLASSEX mWindowDesc;
		HWND mWindowHandle;

		int mWindowWidth;
		int mWindowHeight;
	};

	/// <summary>
	/// メッセージ処理
	/// </summary>
	LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}