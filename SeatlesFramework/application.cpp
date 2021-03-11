#include "application.h"
#include "window.h"
#include "directXSystem.h"
#include "utility.h"

#include <tchar.h>

using namespace SeatlesFramework;

int Application::mMainWindowWidth = 720;
int Application::mMainWindowHeight = 480;

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
{}

Application::Application(const Application&):
	mpWindow(nullptr)
{}

Application::~Application()
{}

void Application::onInitialize()
{
	// メインウィンドウ作成
	mpWindow = new Window(_T("SeatlesFramework"), mMainWindowWidth, mMainWindowHeight);
	
	// シングルトンインスタンス作成
	createSingletons();

	//	シングルトンインスタンス初期化
	initializeSingletons();
}

void Application::onDestroy()
{
	// シングルトンを破棄
	destroySingletons();

	SAFE_DELETE(mpWindow);
}

/// <summary>
/// 使用するシングルトンインスタンスを作成する
/// </summary>
void Application::createSingletons()
{
	render::DirectXSystem::instance();
}

/// <summary>
/// 使用するシングルトンを初期化する
/// </summary>
void Application::initializeSingletons()
{
	render::DirectXSystem::instance()->initialize();
}

/// <summary>
/// 使用していたシングルトンをすべて破棄する
/// </summary>
void Application::destroySingletons()
{
	render::DirectXSystem::instance()->destroy();
}