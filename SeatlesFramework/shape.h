#pragma once

#include"directXSystem.h"
#include"directX.h"
#include <vector>

using namespace DirectX;
using namespace std;

namespace SeatlesFramework
{
	namespace render
	{
		class Shape
		{
		public:
			Shape();
			virtual ~Shape();

			virtual void draw(ID3D12GraphicsCommandList* pCommandList) = 0;
		protected:
			virtual void initialize();
			vector<XMFLOAT3> mVertices;
			ID3D12Resource* mpVertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
		private:
		};

		class Triangle :public Shape
		{
		public:
			Triangle(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3);
			virtual ~Triangle();

			void initialize() override;
			void draw(ID3D12GraphicsCommandList* pCommandList) override;
		};
	}
}