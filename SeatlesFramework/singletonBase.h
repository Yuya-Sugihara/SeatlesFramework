#pragma once
#include "utility.h"

namespace SeatlesFramework
{
	/// <summary>
	/// シングルトンクラスの基底テンプレート
	/// </summary>
	template<class T>
	class SingletonBase
	{
	public:

		/// <summary>
		/// インスタンス取得
		/// </summary>
		static T* instance();

		/// <summary>
		/// 初期化処理の実行
		/// </summary>
		void initialize();

		/// <summary>
		/// 破棄処理の実行 
		/// </summary>
		void destroy();

	protected:
		SingletonBase();
		SingletonBase(const SingletonBase<T>&);
		virtual ~SingletonBase();

		virtual void onInitialize();
		virtual void onDestroy();

	private:
		static T* mpInstance;
	};

	template<class T>
	T* SingletonBase<T>::mpInstance = nullptr;

	template<class T>
	T* SingletonBase<T>::instance()
	{
		if (mpInstance == nullptr)
		{
			mpInstance = new T();
		}

		return mpInstance;
	}

	template<class T>
	void SingletonBase<T>::initialize()
	{
		onInitialize();
	}

	template<class T>
	void SingletonBase<T>::destroy()
	{
		onDestroy();

		SAFE_DELETE(mpInstance);
	}

	template<class T>
	SingletonBase<T>::SingletonBase()
	{}

	template<class T>
	SingletonBase<T>::SingletonBase(const SingletonBase<T>&)
	{}

	template<class T>
	SingletonBase<T>::~SingletonBase()
	{}

	template<class T>
	void SingletonBase<T>::onInitialize()
	{}

	template<class T>
	void SingletonBase<T>::onDestroy()
	{}
}
