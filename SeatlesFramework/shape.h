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

			const D3D12_VERTEX_BUFFER_VIEW* getVertexBufferView() const { return &mVertexBufferView; }
		private:
			ID3D12Resource* mpVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			XMFLOAT3 mVertices[3];

		};
	}
}