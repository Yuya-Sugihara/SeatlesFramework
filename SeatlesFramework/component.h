#pragma once

namespace seatlesFramework
{
	/// <summary>
	/// コンポーネントの基底クラス
	/// </summary>
	class Component
	{
	public: 
		Component();
		virtual ~Component();

		//	コンポーネントの初期化処理
		virtual void awake();

		//	コンポーネントの更新処理
		virtual void update();
	};
}