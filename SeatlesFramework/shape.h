#pragma once

#include"directXSystem.h"
#include"directX.h"

using namespace DirectX;

namespace SeatlesFramework
{
	namespace render
	{
		class Shape
		{
		public:
			Shape();
			~Shape();

		private:

			ID3D12Resource* mpVertexBuffer;

			XMFLOAT3 mVertices[3];

		};
	}
}