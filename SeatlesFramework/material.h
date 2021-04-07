#pragma once

#include"directX.h"
#include <vector>

using namespace std;

namespace seatlesFramework
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
			Texture();
			virtual ~Texture();
			
			void setup();
			ID3D12DescriptorHeap* getDescriptorHeap()const { return mpDescriptorHeap; }
		
		protected:
			virtual void setTexture() = 0;
			virtual void setResourceDesc() = 0;
			virtual void writeToSubResource() = 0;
			virtual void setHeapProperty();
			virtual void createTextureBuffer();
			virtual void createDescriptorHeap();
			virtual void createShaderResourceView();

			D3D12_RESOURCE_DESC mResourceDesc;
			D3D12_HEAP_PROPERTIES mHeapProperty;
			D3D12_DESCRIPTOR_HEAP_DESC mDescHeapDesc;
			D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc;

			ID3D12Resource* mpTextureBuffer;
			ID3D12DescriptorHeap* mpDescriptorHeap;
		};

		class RandomTexture :public Texture
		{
		public: 
			struct TexRGBA
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};

			RandomTexture();

		protected:
			void setTexture() override;
			void setResourceDesc() override;
			void writeToSubResource() override;

		private:
			int mTextureWidth;
			int mTextureHeight;

			vector<TexRGBA> mTextureData;
		};

		class ImageTexture :public Texture
		{
		public: 
			ImageTexture(const wchar_t* fileName);
		
		protected:
			void setTexture() override;
			void setResourceDesc() override;
			void writeToSubResource() override;
			void createShaderResourceView() override;

		private: 
			const wchar_t* mFileName;
			ScratchImage mScratchImage;
			TexMetadata mMetaData;
		};
	}
}