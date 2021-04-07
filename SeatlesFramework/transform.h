#pragma once
#include "directX.h"

namespace seatlesFramework
{
	/// <summary>
	/// Position,Rotation,Scale�̒l���Ǘ�����
	/// </summary>
	class Transform
	{
	public:
		Transform();
		~Transform();

		void update();

	private: 
		XMMATRIX mWorldMatrix;

	};
}