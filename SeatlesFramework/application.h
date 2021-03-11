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

		///	アクセサ
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