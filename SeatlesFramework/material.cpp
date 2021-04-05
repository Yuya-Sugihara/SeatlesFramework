#include "material.h"
#include "directXSystem.h"

using namespace SeatlesFramework;
using namespace render;

Texture::Texture():
	mTextureWidth(256),
	mTextureHeight(256),
	mTextureData{},
	mpTextureBuffer(nullptr),
	mpDescriptorHeap(nullptr)
{
	//	テクスチャ作成
	setRandomTexture();

	//	writeToSubResource()で転送するためのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp = {};

	//	特殊な設定
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;

	//	ライトバック
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;

	//	転送はCPU側から直接行う
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//	単一アダプター
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

	//	リソース設定
	D3D12_RESOURCE_DESC resourceDesc = {};

	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.Width = mTextureWidth;
	resourceDesc.Height = mTextureHeight;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.MipLevels = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//	テクスチャバッファ作成
	auto device = DirectXSystem::instance()->getDevice();
	auto result = device->CreateCommittedResource
	(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&mpTextureBuffer)
	);

	//	データ転送
	result = mpTextureBuffer->WriteToSubresource
	(
		0,
		nullptr,
		mTextureData.data(),
		sizeof(TexRGBA) * mTextureWidth,
		sizeof(TexRGBA) * mTextureData.size()
	);

	//	ディスクリプターヒープのDesc作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	//	シェーダーから見える
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	//	シェーダーリソースビュー用
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//	ディスクリプターヒープ作成
	result = device->CreateDescriptorHeap
	(
		&descHeapDesc,
		IID_PPV_ARGS(&mpDescriptorHeap)
	);

	//	シェーダーリソースビューのDesc作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	//	RBGA(0.0f ~ 1.0f)に正規化
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//	シェーダーリソースビュー作成
	device->CreateShaderResourceView
	(
		mpTextureBuffer,
		&srvDesc,
		mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

Texture::~Texture()
{
	SAFE_RELEASE(mpTextureBuffer);
	SAFE_RELEASE(mpDescriptorHeap);
}

void Texture::setRandomTexture()
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