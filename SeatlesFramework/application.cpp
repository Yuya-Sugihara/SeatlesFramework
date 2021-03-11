#include "application.h"
#include "window.h"
#include "directXSystem.h"
#include "utility.h"

#include <tchar.h>

using namespace SeatlesFramework;

int Application::mMainWindowWidth = 720;
int Application::mMainWindowHeight = 480;

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
{}

Application::Application(const Application&):
	mpWindow(nullptr)
{}

Application::~Application()
{}

void Application::onInitialize()
{
	// ���C���E�B���h�E�쐬
	mpWindow = new Window(_T("SeatlesFramework"), mMainWindowWidth, mMainWindowHeight);
	
	// �V���O���g���C���X�^���X�쐬
	createSingletons();

	//	�V���O���g���C���X�^���X������
	initializeSingletons();
}

void Application::onDestroy()
{
	// �V���O���g����j��
	destroySingletons();

	SAFE_DELETE(mpWindow);
}

/// <summary>
/// �g�p����V���O���g���C���X�^���X���쐬����
/// </summary>
void Application::createSingletons()
{
	render::DirectXSystem::instance();
}

/// <summary>
/// �g�p����V���O���g��������������
/// </summary>
void Application::initializeSingletons()
{
	render::DirectXSystem::instance()->initialize();
}

/// <summary>
/// �g�p���Ă����V���O���g�������ׂĔj������
/// </summary>
void Application::destroySingletons()
{
	render::DirectXSystem::instance()->destroy();
}