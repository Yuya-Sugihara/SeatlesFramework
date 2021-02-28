#include "application.h"
#include "window.h"
#include "utility.h"

#include <tchar.h>

namespace SeatlesFramework
{
	/// フィールド初期化
	Application* Application::mpInstance = nullptr;

	/// <summary>
	/// インスタンス取得
	/// </summary>
	Application* Application::instance()
	{
		if (Application::mpInstance == nullptr)
		{
			Application::mpInstance = new Application();
		}

		return Application::mpInstance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Application::init()
	{
		mpWindow = new Window(_T("SeatlesFramework"), 720, 480);
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	void Application::update()
	{
		while (!mpWindow->update())
		{

		}
	}

	/// <summary>
	///	 破棄処理
	/// </summary>
	void Application::destroy()
	{
		SAFE_DELETE(mpWindow);
	}

	Application::Application():
		mpWindow(nullptr)
	{}

	Application::Application(Application&):
		mpWindow(nullptr)
	{}

}