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
	//	mapする必要があるのでCPUからアクセスできるように
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	ページング設定　デフォルト
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	メモリプール設定　デフォルト
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
	Vertex* vertMap = nullptr;
	result = mpVertexBuffer->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(mVertices), std::end(mVertices), vertMap);
	mpVertexBuffer->Unmap(0, nullptr);
	for (UINT i = 0; i < mVertices.size(); i++)
	{
		Trace::traceLog("頂点: (%f,%f,%f)",vertMap[i].pos.x, vertMap[i].pos.y, vertMap[i].pos.z);
	}

	//	頂点ビューの作成
	mVertexBufferView = {};
	mVertexBufferView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.SizeInBytes = sizeof(mVertices[0]) * mVertices.size();
	mVertexBufferView.StrideInBytes = sizeof(mVertices[0]);
}

Triangle::Triangle(Vertex vertex1, Vertex vertex2, Vertex vertex3):
	Shape()
{
	//	座標設定
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
	//	トポロジーセット
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0,1, &mVertexBufferView);

	//	描画命令セット(頂点数、インスタンス数、頂点データのオフセット、インスタンスのオフセット)
	pCommandList->DrawInstanced(mVertices.size(), 1, 0, 0);
}

Rectangle::Rectangle(Vertex vertex1, Vertex vertex2, Vertex vertex3, Vertex vertex4):
	Shape(),
	mpIndexBuffer(nullptr),
	mIndexBufferView{},
	mIndices{ 0, 1, 2, 2, 1, 3 }
{
	//	座標設定
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

	//	インデックスバッファー作成
	D3D12_HEAP_PROPERTIES heapProp = {};
	//	mapする必要があるのでCPUからアクセスできるように
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//	ページング設定　デフォルト
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	メモリプール設定　デフォルト
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

	//	頂点バッファー作成
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
	throwAssertIfFailed(result, "インデックスバッファーの作成に失敗しました。");

	//	マップ
	unsigned short* indexMap = nullptr;
	result = mpIndexBuffer->Map(0, nullptr, (void**)&indexMap);
	std::copy(std::begin(mIndices), std::end(mIndices), indexMap);
	mpIndexBuffer->Unmap(0, nullptr);

	//	インデックスバッファービューの作成
	mIndexBufferView.BufferLocation = mpIndexBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	mIndexBufferView.SizeInBytes = sizeof(mIndices);
}

void Rectangle::draw(ID3D12GraphicsCommandList* pCommandList)
{
	//	トポロジーセット
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	//	頂点バッファセット（スロット番号、登録するバッファ数、描画に使用する頂点バッファビュー）
	pCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	//	インデックスバッファセット
	pCommandList->IASetIndexBuffer(&mIndexBufferView);

	//	描画命令セット(頂点数、インスタンス数、頂点データのオフセット、インスタンスのオフセット)
	//pCommandList->DrawInstanced(4, 1, 0, 0);
	//pCommandList->DrawInstanced(mVertices.size(), 1, 0, 0);
	//pCommandList->DrawIndexedInstanced(_countof(mIndices), 1, 0, 0, 0);
	pCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	//	インデックスバッファーにして四角形が描画されなくなった
}