#pragma once

namespace SeatlesFramework
{

	class Window;

	/// <summary>
	/// アプリケーション全体を管理するシングルトンクラス
	/// </summary>
	class Application
	{
	public:

		/// <summary>
		/// インスタンス取得
		/// </summary>
		static Application* instance();

		/// <summary>
		/// 初期化処理
		/// </summary>
		void init();

		/// <summary>
		/// 更新処理
		/// </summary>
		void update();

		/// <summary>
		/// 破棄処理
		/// </summary>
		void destroy();

	private:

		Application();

		Application(Application&);

		static Application* mpInstance;
		Window* mpWindow;
	};
}