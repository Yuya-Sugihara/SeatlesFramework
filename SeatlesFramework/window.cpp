#include "window.h"

#include <tchar.h>

namespace SeatlesFramework
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Window::Window(const wchar_t* applicationName, int width, int height):
		mWindowHandle(nullptr),
		mWindowWidth(width),
		mWindowHeight(height)
	{
		init(applicationName);
	}
	
	/// <summary>
	/// ウィンドウの初期化処理
	/// </summary>
	HRESULT Window::init(const wchar_t* applicationName)
	{
		mWindowDesc = {};
		mWindowDesc.cbSize = sizeof(WNDCLASSEX);
		mWindowDesc.lpszClassName = applicationName;
		mWindowDesc.lpfnWndProc = (WNDPROC)windowProcedure;
		mWindowDesc.hInstance = GetModuleHandle(nullptr);

		RegisterClassEx(&mWindowDesc);

		RECT windowRect = { 0, 0, mWindowWidth, mWindowHeight };
		/// ウィンドウサイズを補正する
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

		/// ウィンドウ作成
		mWindowHandle = CreateWindow
		(
			applicationName,	///	クラス名
			applicationName,	///	タイトルバーの文字
			WS_OVERLAPPEDWINDOW,		///	タイトルバーと境界線あり	
			CW_USEDEFAULT,		///	表示X座標はOSに任せる
			CW_USEDEFAULT,		///	表示X座標はOSに任せる
			windowRect.right - windowRect.left,	///	幅
			windowRect.bottom - windowRect.top,	///	高さ
			nullptr,	///	親ウィンドウハンドル
			nullptr,	///	メニューハンドル
			mWindowDesc.hInstance,	///	呼び出しアプリケーションのハンドル
			nullptr	///	追加パラメータ
		);

		/// ウィンドウ表示
		ShowWindow(mWindowHandle, SW_SHOW);

		return S_OK;
	}

	bool Window::update()
	{
		MSG msg = {};
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			UnregisterClass(mWindowDesc.lpszClassName,mWindowDesc.hInstance);
			return true;
		}

		return false;
	}

	/// <summary>
	/// メッセージ処理
	/// </summary>
	LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_CREATE:			///		生成
			case WM_MOVE:				///		移動
			case WM_SIZE:				///		サイズ変更時
			case WM_CLOSE:			///		閉じる
			case WM_QUIT:				///		PostQuitMessage() 実行後
			case WM_KEYDOWN:		///		非システムキーが押された時
			case WM_KEYUP:			///		非システムキーが上がった時
			case WM_MOUSEMOVE:	///		マウスが動いた時
				break;
			case WM_DESTROY:		///		破棄
				PostQuitMessage(0);
				return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}