#pragma once
#include "directX.h"

namespace seatlesFramework
{
	/// <summary>
	/// Position,Rotation,Scaleの値を管理する
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