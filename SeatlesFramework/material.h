#pragma once

#include"directX.h"
#include <vector>

using namespace DirectX;
using namespace std;

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

		class Texture
		{
		public:
			struct TexRGBA
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};

			Texture();
			~Texture();

			//	ランダムなテクスチャーデータをセットする
			void setRandomTexture();
			ID3D12DescriptorHeap* getDescriptorHeap()const { return mpDescriptorHeap; }
		private:
			int mTextureWidth;
			int mTextureHeight;

			vector<TexRGBA> mTextureData;
			ID3D12Resource* mpTextureBuffer;
			ID3D12DescriptorHeap* mpDescriptorHeap;
		};
	}
}