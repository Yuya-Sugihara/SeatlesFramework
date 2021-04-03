#include "shape.h"
#include "utility.h"
#include "assert.h"
#include "directX.h"

#include <iostream>


using namespace SeatlesFramework::render;

Shape::Shape():
	mpVertexBuffer(nullptr)
{
	mVertices[0] = { -1.0f, -1.0f, 0.0f };
	mVertices[1] = { -1.0f,  1.0f, 0.0f };
	mVertices[2] = {  1.0f, -1.0f, 0.0f };

	D3D12_HEAP_PROPERTIES heapProp = {};
	//	map����K�v������̂�CPU����A�N�Z�X�ł���悤��
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	�y�[�W���O�ݒ�@�f�t�H���g
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	�������v�[���ݒ�@�f�t�H���g
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(mVertices);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	���_�o�b�t�@�[�쐬
	auto device = DirectXSystem::instance()->getDevice();
	HRESULT result = device->CreateCommittedResource
	(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mpVertexBuffer)
	);
	throwAssertIfFailed(result, "���_�o�b�t�@�[�̍쐬�Ɏ��s���܂����B");

	//	�}�b�v
	XMFLOAT3* vertMap = nullptr;
	result = mpVertexBuffer->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(mVertices), std::end(mVertices), vertMap);
	mpVertexBuffer->Unmap(0, nullptr);

	//	���_�r���[�̍쐬
	mVertexBufferView = {};
	mVertexBufferView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.SizeInBytes = sizeof(mVertices);
	mVertexBufferView.StrideInBytes = sizeof(mVertices[0]);
}

Shape::~Shape()
{
	SAFE_RELEASE(mpVertexBuffer);
}