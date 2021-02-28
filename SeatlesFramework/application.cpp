#include "application.h"
#include "window.h"
#include "directXSystem.h"
#include "utility.h"

#include <tchar.h>

namespace SeatlesFramework
{
	/// フィールド初期化
	Application* Application::mpInstance = nullptr;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Application::update()
	{
		while (!mpWindow->update())
		{

		}
	}

	Application::Application():
		mpWindow(nullptr)
	{
		mpWindow = new Window(_T("SeatlesFramework"), 720, 480);
		mpDirectXSystem = render::DirectXSystem::instance();
	}

	Application::Application(const Application&):
		mpWindow(nullptr)
	{}

	Application::~Application()
	{
		SAFE_DELETE(mpWindow);
		mpDirectXSystem->destroy();
	}
}