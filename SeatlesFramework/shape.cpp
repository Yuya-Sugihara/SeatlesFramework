#include "shape.h"
#include "utility.h"
#include "assert.h"
#include "directX.h"
#include "debugUtility.h"

#include <iostream>


using namespace SeatlesFramework::render;

Shape::Shape():
	mpVertexBuffer(nullptr),
	mVertexBufferView{},
	mVertices{}
{}

Shape::~Shape()
{
	SAFE_RELEASE(mpVertexBuffer);
}

void Shape::initialize()
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	//	map����K�v������̂�CPU����A�N�Z�X�ł���悤��
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	�y�[�W���O�ݒ�@�f�t�H���g
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	�������v�[���ݒ�@�f�t�H���g
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = (UINT64)mVertices.size() * sizeof(mVertices[0]);
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
	Vertex* vertMap = nullptr;
	result = mpVertexBuffer->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(mVertices), std::end(mVertices), vertMap);
	mpVertexBuffer->Unmap(0, nullptr);
	for (UINT i = 0; i < mVertices.size(); i++)
	{
		Trace::traceLog("���_: (%f,%f,%f)",vertMap[i].pos.x, vertMap[i].pos.y, vertMap[i].pos.z);
	}

	//	���_�r���[�̍쐬
	mVertexBufferView = {};
	mVertexBufferView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.SizeInBytes = sizeof(mVertices[0]) * mVertices.size();
	mVertexBufferView.StrideInBytes = sizeof(mVertices[0]);
}

Triangle::Triangle(Vertex vertex1, Vertex vertex2, Vertex vertex3):
	Shape()
{
	//	���W�ݒ�
	mVertices = { vertex1,vertex2,vertex3 };

	initialize();
}

Triangle::~Triangle()
{}

void Triangle::initialize()
{
	Shape::initialize();
}

void Triangle::draw(ID3D12GraphicsCommandList* pCommandList)
{
	//	�g�|���W�[�Z�b�g
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0,1, &mVertexBufferView);

	//	�`�施�߃Z�b�g(���_���A�C���X�^���X���A���_�f�[�^�̃I�t�Z�b�g�A�C���X�^���X�̃I�t�Z�b�g)
	pCommandList->DrawInstanced(mVertices.size(), 1, 0, 0);
}

Rectangle::Rectangle(Vertex vertex1, Vertex vertex2, Vertex vertex3, Vertex vertex4):
	Shape(),
	mpIndexBuffer(nullptr),
	mIndexBufferView{},
	mIndices{ 0, 1, 2, 2, 1, 3 }
{
	//	���W�ݒ�
	mVertices = { vertex1,vertex2,vertex3,vertex4 };

	initialize();
}

Rectangle::~Rectangle()
{
	SAFE_RELEASE(mpIndexBuffer);
}

void Rectangle::initialize()
{
	Shape::initialize();

	//	�C���f�b�N�X�o�b�t�@�[�쐬
	D3D12_HEAP_PROPERTIES heapProp = {};
	//	map����K�v������̂�CPU����A�N�Z�X�ł���悤��
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	�y�[�W���O�ݒ�@�f�t�H���g
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	�������v�[���ݒ�@�f�t�H���g
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(mIndices);
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
		IID_PPV_ARGS(&mpIndexBuffer)
	);
	throwAssertIfFailed(result, "�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s���܂����B");

	//	�}�b�v
	unsigned short* indexMap = nullptr;
	result = mpIndexBuffer->Map(0, nullptr, (void**)&indexMap);
	std::copy(std::begin(mIndices), std::end(mIndices), indexMap);
	mpIndexBuffer->Unmap(0, nullptr);

	//	�C���f�b�N�X�o�b�t�@�[�r���[�̍쐬
	mIndexBufferView.BufferLocation = mpIndexBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	mIndexBufferView.SizeInBytes = sizeof(mIndices);
}

void Rectangle::draw(ID3D12GraphicsCommandList* pCommandList)
{
	//	�g�|���W�[�Z�b�g
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	//	���_�o�b�t�@�Z�b�g�i�X���b�g�ԍ��A�o�^����o�b�t�@���A�`��Ɏg�p���钸�_�o�b�t�@�r���[�j
	pCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	//	�C���f�b�N�X�o�b�t�@�Z�b�g
	pCommandList->IASetIndexBuffer(&mIndexBufferView);

	//	�`�施�߃Z�b�g(���_���A�C���X�^���X���A���_�f�[�^�̃I�t�Z�b�g�A�C���X�^���X�̃I�t�Z�b�g)
	//pCommandList->DrawInstanced(4, 1, 0, 0);
	//pCommandList->DrawInstanced(mVertices.size(), 1, 0, 0);
	//pCommandList->DrawIndexedInstanced(_countof(mIndices), 1, 0, 0, 0);
	pCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	//	�C���f�b�N�X�o�b�t�@�[�ɂ��Ďl�p�`���`�悳��Ȃ��Ȃ���
}