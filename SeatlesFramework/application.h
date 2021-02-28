#pragma once
#include "singletonBase.h"

namespace SeatlesFramework
{
	class Window;

	/// <summary>
	/// アプリケーション全体を管理するシングルトンクラス
	/// </summary>
	class Application:public SingletonBase<Application>
	{
	public:
		friend class SingletonBase<Application>;

		/// <summary>
		/// 更新処理
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