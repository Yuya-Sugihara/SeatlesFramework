#pragma once

#include <Windows.h>

namespace seatlesFramework
{
	/// <summary>
	/// window�Ɋւ���N���X
	/// </summary>
	class Window
	{
	public: 

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Window(const wchar_t* applicationName,int windowWidth,int windowHeight);

		/// <summary>
		/// �E�B���h�E�X�V�����@�E�B���h�E���I�������Ƃ�true��Ԃ�
		/// </summary>
		bool update();

		/// �A�N�Z�T
		const HWND getWindowHandle()const { return mWindowHandle; }
		const int getWindowWidth() const { return mWindowWidth; }
		const int getWindowHeight() const { return mWindowHeight; }

	private: 

		/// <summary>
		/// �E�B���h�E�̏���������
		/// </summary>
		HRESULT init(const wchar_t* applicationName);

		WNDCLASSEX mWindowDesc;
		HWND mWindowHandle;

		int mWindowWidth;
		int mWindowHeight;
	};

	/// <summary>
	/// ���b�Z�[�W����
	/// </summary>
	LRESULT windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}