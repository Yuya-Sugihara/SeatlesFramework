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
	//	�e�N�X�`���f�[�^�Z�b�g
	setTexture();

	//	���\�[�XDesc�Z�b�g
	setResourceDesc();

	//	�q�[�v�v���p�e�B�Z�b�g
	setHeapProperty();

	//	�e�N�X�`���o�b�t�@�쐬
	createTextureBuffer();

	//	�]��
	writeToSubResource();

	//	�f�B�X�N���v�^�[�q�[�v�쐬
	createDescriptorHeap();

	//	�V�F�[�_�[���\�[�X�r���[�쐬
	createShaderResourceView();
}

void Texture::setHeapProperty()
{
	//	writeToSubResource()�œ]�����邽�߂̃q�[�v�ݒ�
	//	����Ȑݒ�
	mHeapProperty.Type = D3D12_HEAP_TYPE_CUSTOM;

	//	���C�g�o�b�N
	mHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;

	//	�]����CPU�����璼�ڍs��
	mHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//	�P��A�_�v�^�[
	mHeapProperty.CreationNodeMask = 0;
	mHeapProperty.VisibleNodeMask = 0;
}

void Texture::createTextureBuffer()
{
	//	�e�N�X�`���o�b�t�@�쐬
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
	throwAssertIfFailed(result, "�e�N�X�`���o�b�t�@�̍쐬�Ɏ��s���܂����B");
}

void Texture::createDescriptorHeap()
{
	//	�f�B�X�N���v�^�[�q�[�v��Desc�쐬
	//	�V�F�[�_�[���猩����
	mDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	mDescHeapDesc.NodeMask = 0;
	mDescHeapDesc.NumDescriptors = 1;
	//	�V�F�[�_�[���\�[�X�r���[�p
	mDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//	�f�B�X�N���v�^�[�q�[�v�쐬
	auto device = DirectXSystem::instance()->getDevice();
	auto result = device->CreateDescriptorHeap
	(
		&mDescHeapDesc,
		IID_PPV_ARGS(&mpDescriptorHeap)
	);
}

void Texture::createShaderResourceView()
{
	//	�V�F�[�_�[���\�[�X�r���[��Desc�쐬
	//	RBGA(0.0f ~ 1.0f)�ɐ��K��
	mSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mSrvDesc.Texture2D.MipLevels = 1;

	//	�V�F�[�_�[���\�[�X�r���[�쐬
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
	//	���\�[�X�ݒ�
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
	//	�f�[�^�]��
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
	//	�e�N�X�`�����[�h
	auto result = LoadFromWICFile(mFileName, WIC_FLAGS_NONE, &mMetaData, mScratchImage);
	throwAssertIfFailed(result, "�e�N�X�`���̃��[�h�Ɏ��s���܂����B");
}

void ImageTexture::setResourceDesc()
{
	//	���\�[�X�ݒ�
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

	//	�f�[�^�]��
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
	//	�V�F�[�_�[���\�[�X�r���[��Desc�쐬
	mSrvDesc.Format = mMetaData.format;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mSrvDesc.Texture2D.MipLevels = 1;

	//	�V�F�[�_�[���\�[�X�r���[�쐬
	auto device = DirectXSystem::instance()->getDevice();
	device->CreateShaderResourceView
	(
		mpTextureBuffer,
		&mSrvDesc,
		mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);
}