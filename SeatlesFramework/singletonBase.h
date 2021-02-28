#pragma once
#include "utility.h"

namespace SeatlesFramework
{
	/// <summary>
	/// �V���O���g���N���X�̊��e���v���[�g
	/// </summary>
	template<class T>
	class SingletonBase
	{
	public:

		/// <summary>
		/// �C���X�^���X�擾
		/// </summary>
		static T* instance();

		/// <summary>
		/// �j�������̎��s 
		/// </summary>
		void destroy();

	protected:

		SingletonBase();
		SingletonBase(const SingletonBase<T>&);
		virtual ~SingletonBase();

	private:
		static T* mpInstance;
	};

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
	void SingletonBase<T>::destroy()
	{
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
}
