#include "window.h"

#include <tchar.h>

namespace SeatlesFramework
{
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Window::Window(const wchar_t* applicationName, int width, int height):
		mWindowHandle(nullptr),
		mWindowWidth(width),
		mWindowHeight(height)
	{
		init(applicationName);
	}
	
	/// <summary>
	/// �E�B���h�E�̏���������
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
		/// �E�B���h�E�T�C�Y��␳����
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

		/// �E�B���h�E�쐬
		mWindowHandle = CreateWindow
		(
			applicationName,	///	�N���X��
			applicationName,	///	�^�C�g���o�[�̕���
			WS_OVERLAPPEDWINDOW,		///	�^�C�g���o�[�Ƌ��E������	
			CW_USEDEFAULT,		///	�\��X���W��OS�ɔC����
			CW_USEDEFAULT,		///	�\��X���W��OS�ɔC����
			windowRect.right - windowRect.left,	///	��
			windowRect.bottom - windowRect.top,	///	����
			nullptr,	///	�e�E�B���h�E�n���h��
			nullptr,	///	���j���[�n���h��
			mWindowDesc.hInstance,	///	�Ăяo���A�v���P�[�V�����̃n���h��
			nullptr	///	�ǉ��p�����[�^
		);

		/// �E�B���h�E�\��
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
	/// ���b�Z�[�W����
	/// </summary>
	LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_CREATE:			///		����
			case WM_MOVE:				///		�ړ�
			case WM_SIZE:				///		�T�C�Y�ύX��
			case WM_CLOSE:			///		����
			case WM_QUIT:				///		PostQuitMessage() ���s��
			case WM_KEYDOWN:		///		��V�X�e���L�[�������ꂽ��
			case WM_KEYUP:			///		��V�X�e���L�[���オ������
			case WM_MOUSEMOVE:	///		�}�E�X����������
				break;
			case WM_DESTROY:		///		�j��
				PostQuitMessage(0);
				return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}