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
	//	�e�N�X�`���쐬
	setRandomTexture();

	//	writeToSubResource()�œ]�����邽�߂̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp = {};

	//	����Ȑݒ�
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;

	//	���C�g�o�b�N
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;

	//	�]����CPU�����璼�ڍs��
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//	�P��A�_�v�^�[
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

	//	���\�[�X�ݒ�
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

	//	�e�N�X�`���o�b�t�@�쐬
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

	//	�f�[�^�]��
	result = mpTextureBuffer->WriteToSubresource
	(
		0,
		nullptr,
		mTextureData.data(),
		sizeof(TexRGBA) * mTextureWidth,
		sizeof(TexRGBA) * mTextureData.size()
	);

	//	�f�B�X�N���v�^�[�q�[�v��Desc�쐬
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	//	�V�F�[�_�[���猩����
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	//	�V�F�[�_�[���\�[�X�r���[�p
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//	�f�B�X�N���v�^�[�q�[�v�쐬
	result = device->CreateDescriptorHeap
	(
		&descHeapDesc,
		IID_PPV_ARGS(&mpDescriptorHeap)
	);

	//	�V�F�[�_�[���\�[�X�r���[��Desc�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	//	RBGA(0.0f ~ 1.0f)�ɐ��K��
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//	�V�F�[�_�[���\�[�X�r���[�쐬
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