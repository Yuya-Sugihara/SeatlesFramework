#pragma once

namespace SeatlesFramework
{

	class Window;

	/// <summary>
	/// �A�v���P�[�V�����S�̂��Ǘ�����V���O���g���N���X
	/// </summary>
	class Application
	{
	public:

		/// <summary>
		/// �C���X�^���X�擾
		/// </summary>
		static Application* instance();

		/// <summary>
		/// ����������
		/// </summary>
		void init();

		/// <summary>
		/// �X�V����
		/// </summary>
		void update();

		/// <summary>
		/// �j������
		/// </summary>
		void destroy();

	private:

		Application();

		Application(Application&);

		static Application* mpInstance;
		Window* mpWindow;
	};
}