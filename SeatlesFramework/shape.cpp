#include "shape.h"
#include "utility.h"
#include "assert.h"
#include "directX.h"

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
	//	mapする必要があるのでCPUからアクセスできるように
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	ページング設定　デフォルト
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	メモリプール設定　デフォルト
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = (int)(mVertices.size() * sizeof(mVertices[0]));
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	頂点バッファー作成
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
	throwAssertIfFailed(result, "頂点バッファーの作成に失敗しました。");

	//	マップ
	XMFLOAT3* vertMap = nullptr;
	result = mpVertexBuffer->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(mVertices), std::end(mVertices), vertMap);
	mpVertexBuffer->Unmap(0, nullptr);

	//	頂点ビューの作成
	mVertexBufferView = {};
	mVertexBufferView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.SizeInBytes = sizeof(mVertices[0]) * mVertices.size();
	mVertexBufferView.StrideInBytes = sizeof(mVertices[0]);
}

Triangle::Triangle(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3):
	Shape()
{
	//	座標設定
	mVertices = { pos1,pos2,pos3 };

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
	//	トポロジーセット
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers
	(
		0, //	スロット番号
		1, //	登録するバッファ数
		&mVertexBufferView //	描画に使用する頂点バッファビュー
	);

	//	描画命令セット(頂点数、インスタンス数、頂点データのオフセット、インスタンスのオフセット)
	pCommandList->DrawInstanced(mVertices.size(), 1, 0, 0);
}