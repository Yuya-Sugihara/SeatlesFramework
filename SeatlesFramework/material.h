#pragma once

#include"directX.h"

using namespace DirectX;

namespace SeatlesFramework
{
	namespace render
	{
		struct Vertex
		{
			XMFLOAT3 pos;
			XMFLOAT2 uv;

			Vertex() = default;

			Vertex(const Vertex&) = default;
			Vertex& operator=(const Vertex&) = default;

			Vertex(Vertex&&) = default;
			Vertex& operator=(Vertex&&) = default;

			XM_CONSTEXPR Vertex(XMFLOAT3 _x, XMFLOAT2 _y):
				pos(_x), 
				uv(_y)
			{}
		};
	}
}