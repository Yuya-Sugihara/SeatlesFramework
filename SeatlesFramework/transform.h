#pragma once
#include "directX.h"

namespace seatlesFramework
{
	/// <summary>
	/// Position,Rotation,Scale‚Ì’l‚ğŠÇ—‚·‚é
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