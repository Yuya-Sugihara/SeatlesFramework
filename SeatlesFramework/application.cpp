#include "application.h"
#include "window.h"
#include "utility.h"

#include <tchar.h>

namespace SeatlesFramework
{
	/// �t�B�[���h������
	Application* Application::mpInstance = nullptr;

	/// <summary>
	/// �C���X�^���X�擾
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
	/// ����������
	/// </summary>
	void Application::init()
	{
		mpWindow = new Window(_T("SeatlesFramework"), 720, 480);
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	void Application::update()
	{
		while (!mpWindow->update())
		{

		}
	}

	/// <summary>
	///	 �j������
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