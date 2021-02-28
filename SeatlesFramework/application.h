#pragma once
#include "singletonBase.h"

namespace SeatlesFramework
{
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

	protected:

		Application();
		Application(const Application&);
		~Application() override;

	private:

		Window* mpWindow;
	};
}