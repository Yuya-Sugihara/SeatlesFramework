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
	//	定数バッファ作成
	auto device = DirectXSystem::instance()->getDevice();
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//	マップできるように
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(mWorldMatrix) + 0xff) & ~0xff);//	定数バッファのアライメントは256バイトである必要がある
	device->CreateCommittedResource
	(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mpConstantBuffer)
	);

	//	初期値で初期化
	XMMATRIX* mapMatrix = nullptr;
	auto result = mpConstantBuffer->Map(0, nullptr, (void**)&mapMatrix);
	throwAssertIfFailed(result, "コンスタントバッファからのマップ処理に失敗しました。");
	*mapMatrix = mWorldMatrix;
	
	//	ディスクリプターヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	//	コンスタントバッファ用
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = device->CreateDescriptorHeap
	(
		&descHeapDesc,
		IID_PPV_ARGS(&mpDescriptorHeap)
	);
	throwAssertIfFailed(result, "Transformのディスクリプターヒープ作成に失敗しました。");

	//	ビュー作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = mpConstantBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(mpConstantBuffer->GetDesc().Width);
	device->CreateConstantBufferView(&cbvDesc, mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

void Transform::update()
{}