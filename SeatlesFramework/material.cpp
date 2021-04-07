#include "material.h"
#include "directXSystem.h"
#include "assert.h"

using namespace seatlesFramework;
using namespace render;

Texture::Texture():
	mResourceDesc{},
	mHeapProperty{},
	mDescHeapDesc{},
	mSrvDesc{},
	mpTextureBuffer(nullptr),
	mpDescriptorHeap(nullptr)
{}

Texture::~Texture()
{
	SAFE_RELEASE(mpTextureBuffer);
	SAFE_RELEASE(mpDescriptorHeap);
}

void Texture::setup()
{
	//	テクスチャデータセット
	setTexture();

	//	リソースDescセット
	setResourceDesc();

	//	ヒーププロパティセット
	setHeapProperty();

	//	テクスチャバッファ作成
	createTextureBuffer();

	//	転送
	writeToSubResource();

	//	ディスクリプターヒープ作成
	createDescriptorHeap();

	//	シェーダーリソースビュー作成
	createShaderResourceView();
}

void Texture::setHeapProperty()
{
	//	writeToSubResource()で転送するためのヒープ設定
	//	特殊な設定
	mHeapProperty.Type = D3D12_HEAP_TYPE_CUSTOM;

	//	ライトバック
	mHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;

	//	転送はCPU側から直接行う
	mHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//	単一アダプター
	mHeapProperty.CreationNodeMask = 0;
	mHeapProperty.VisibleNodeMask = 0;
}

void Texture::createTextureBuffer()
{
	//	テクスチャバッファ作成
	auto device = DirectXSystem::instance()->getDevice();
	auto result = device->CreateCommittedResource
	(
		&mHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&mResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&mpTextureBuffer)
	);
	throwAssertIfFailed(result, "テクスチャバッファの作成に失敗しました。");
}

void Texture::createDescriptorHeap()
{
	//	ディスクリプターヒープのDesc作成
	//	シェーダーから見える
	mDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	mDescHeapDesc.NodeMask = 0;
	mDescHeapDesc.NumDescriptors = 1;
	//	シェーダーリソースビュー用
	mDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//	ディスクリプターヒープ作成
	auto device = DirectXSystem::instance()->getDevice();
	auto result = device->CreateDescriptorHeap
	(
		&mDescHeapDesc,
		IID_PPV_ARGS(&mpDescriptorHeap)
	);
}

void Texture::createShaderResourceView()
{
	//	シェーダーリソースビューのDesc作成
	//	RBGA(0.0f ~ 1.0f)に正規化
	mSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mSrvDesc.Texture2D.MipLevels = 1;

	//	シェーダーリソースビュー作成
	auto device = DirectXSystem::instance()->getDevice();
	device->CreateShaderResourceView
	(
		mpTextureBuffer,
		&mSrvDesc,
		mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

RandomTexture::RandomTexture() :
	Texture(),
	mTextureWidth(256),
	mTextureHeight(256),
	mTextureData{}
{}

void RandomTexture::setTexture() 
{
	mTextureData.resize(mTextureWidth * mTextureHeight);
	for (auto& data : mTextureData)
	{
		data.r = rand() % 256;
		data.g = rand() % 256;
		data.b = rand() % 256;
		data.a = 1;
	}
}

void RandomTexture::setResourceDesc()
{
	//	リソース設定
	mResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	mResourceDesc.Width = mTextureWidth;
	mResourceDesc.Height = mTextureHeight;
	mResourceDesc.DepthOrArraySize = 1;
	mResourceDesc.SampleDesc.Count = 1;
	mResourceDesc.SampleDesc.Quality = 0;
	mResourceDesc.MipLevels = 1;
	mResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	mResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	mResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
}

void RandomTexture::writeToSubResource()
{
	//	データ転送
	auto result = mpTextureBuffer->WriteToSubresource
	(
		0,
		nullptr,
		mTextureData.data(),
		sizeof(TexRGBA) * mTextureWidth,
		sizeof(TexRGBA) * mTextureData.size()
	);

}


ImageTexture::ImageTexture(const wchar_t* fileName):
	Texture(),
	mFileName(fileName),
	mScratchImage{},
	mMetaData{}
{}

void ImageTexture::setTexture()
{
	//	テクスチャロード
	auto result = LoadFromWICFile(mFileName, WIC_FLAGS_NONE, &mMetaData, mScratchImage);
	throwAssertIfFailed(result, "テクスチャのロードに失敗しました。");
}

void ImageTexture::setResourceDesc()
{
	//	リソース設定
	mResourceDesc.Format = mMetaData.format;
	mResourceDesc.Width = mMetaData.width;
	mResourceDesc.Height = mMetaData.height;
	mResourceDesc.DepthOrArraySize = static_cast<UINT16>(mMetaData.arraySize);
	mResourceDesc.SampleDesc.Count = 1;
	mResourceDesc.SampleDesc.Quality = 0;
	mResourceDesc.MipLevels = static_cast<UINT16>(mMetaData.mipLevels);
	mResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(mMetaData.dimension);
	mResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	mResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
}

void ImageTexture::writeToSubResource()
{
	auto image = mScratchImage.GetImage(0,0,0);

	//	データ転送
	auto result = mpTextureBuffer->WriteToSubresource
	(
		0,
		nullptr,
		image->pixels,
		image->rowPitch,
		image->slicePitch
	);
}

void ImageTexture::createShaderResourceView()
{
	//	シェーダーリソースビューのDesc作成
	mSrvDesc.Format = mMetaData.format;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mSrvDesc.Texture2D.MipLevels = 1;

	//	シェーダーリソースビュー作成
	auto device = DirectXSystem::instance()->getDevice();
	device->CreateShaderResourceView
	(
		mpTextureBuffer,
		&mSrvDesc,
		mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);
}