#include "application.h"
#include "window.h"
#include "utility.h"

#include <tchar.h>

namespace SeatlesFramework
{
	/// �t�B�[���h������
	Application* Application::mpInstance = nullptr;

	/// <summary>
	/// �X�V����
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
	}

	Application::Application(const Application&):
		mpWindow(nullptr)
	{}

	Application::~Application()
	{
		SAFE_DELETE(mpWindow);
	}
}