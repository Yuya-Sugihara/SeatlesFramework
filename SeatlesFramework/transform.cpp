#include "transform.h"
#include "utility.h"
#include "directXSystem.h"
#include "assert.h"

using namespace seatlesFramework;
using namespace render;

Transform::Transform():
	mWorldMatrix(XMMatrixIdentity()),
	mpConstantBuffer(nullptr),
	mpDescriptorHeap(nullptr)
{}

Transform::~Transform()
{
	SAFE_RELEASE(mpConstantBuffer);
	SAFE_RELEASE(mpDescriptorHeap);
}

void Transform::awake()
{
	//	�萔�o�b�t�@�쐬
	auto device = DirectXSystem::instance()->getDevice();
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//	�}�b�v�ł���悤��
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(mWorldMatrix) + 0xff) & ~0xff);//	�萔�o�b�t�@�̃A���C�����g��256�o�C�g�ł���K�v������
	device->CreateCommittedResource
	(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mpConstantBuffer)
	);

	//	�����l�ŏ�����
	XMMATRIX* mapMatrix = nullptr;
	auto result = mpConstantBuffer->Map(0, nullptr, (void**)&mapMatrix);
	throwAssertIfFailed(result, "�R���X�^���g�o�b�t�@����̃}�b�v�����Ɏ��s���܂����B");
	*mapMatrix = mWorldMatrix;
	
	//	�f�B�X�N���v�^�[�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	//	�R���X�^���g�o�b�t�@�p
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = device->CreateDescriptorHeap
	(
		&descHeapDesc,
		IID_PPV_ARGS(&mpDescriptorHeap)
	);
	throwAssertIfFailed(result, "Transform�̃f�B�X�N���v�^�[�q�[�v�쐬�Ɏ��s���܂����B");

	//	�r���[�쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = mpConstantBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(mpConstantBuffer->GetDesc().Width);
	device->CreateConstantBufferView(&cbvDesc, mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

void Transform::update()
{}