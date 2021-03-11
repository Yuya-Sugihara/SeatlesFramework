#pragma once

#include "singletonBase.h"

namespace SeatlesFramework
{
	namespace render
	{
		class DirectXSystem;
	}

	class Window;

	/// <summary>
	/// �A�v���P�[�V�����S�̂��Ǘ�����V���O���g���N���X
	/// </summary>
	class Application:public SingletonBase<Application>
	{
	public:
		friend class SingletonBase<Application>;

		/// <summary>
		/// �X�V����
		/// </summary>
		void update();

		///	�A�N�Z�T
		const Window* getWindow()const { return mpWindow; }

	protected:
		Application();
		Application(const Application&);
		~Application() override;

		void onInitialize() override;
		void onDestroy() override;

	private:
		void createSingletons();
		void initializeSingletons();
		void destroySingletons();

		static int mMainWindowWidth;
		static int mMainWindowHeight;

		Window* mpWindow;
	};
}