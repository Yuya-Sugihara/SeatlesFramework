#pragma once

namespace seatlesFramework
{
	/// <summary>
	/// �R���|�[�l���g�̊��N���X
	/// </summary>
	class Component
	{
	public: 
		Component();
		virtual ~Component();

		//	�R���|�[�l���g�̏���������
		virtual void awake();

		//	�R���|�[�l���g�̍X�V����
		virtual void update();
	};
}