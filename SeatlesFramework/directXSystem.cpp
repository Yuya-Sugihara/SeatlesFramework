#include "directXSystem.h"
#include "application.h"
#include "window.h"
#include "shape.h"
#include "assert.h"
#include "debugUtility.h"

#include <cassert>
#include <comdef.h>
#include <vector>

using namespace SeatlesFramework;
using namespace render;
using namespace std;

DirectXSystem::DirectXSystem():
	mpDevice(nullptr),
	mpDxgiFactory(nullptr),
	mpSwapChain(nullptr),
	mpCommandAllocator(nullptr),
	mpCommandList(nullptr),
	mpCommandQueue(nullptr),
	mpRtvDescriptorHeap(nullptr),
	mpPipelineState(nullptr),
	mpRootSignature(nullptr),
	mpShape(nullptr),
	mViewport{},
	mScissorRect{},
	mBackBufferCount(2)
{}

DirectXSystem::DirectXSystem(const DirectXSystem&) :
	mpDevice(nullptr),
	mpDxgiFactory(nullptr),
	mpSwapChain(nullptr),
	mpCommandAllocator(nullptr),
	mpCommandList(nullptr),
	mpCommandQueue(nullptr),
	mpRtvDescriptorHeap(nullptr),
	mpPipelineState(nullptr),
	mpRootSignature(nullptr),
	mpShape(nullptr),
	mViewport{},
	mScissorRect{},
	mBackBufferCount(2)
{}

DirectXSystem::~DirectXSystem()
{}

void DirectXSystem::update()
{
	int backBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();
	ID3D12Resource* pBackBuffer = nullptr;
	HRESULT result = mpSwapChain->GetBuffer(backBufferIndex,IID_PPV_ARGS(&pBackBuffer));

	//	リソースバリアセット
	//	明示的にリソースを何に使用するかをGPUに渡す
	D3D12_RESOURCE_BARRIER barriorDesc = {};
	barriorDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barriorDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barriorDesc.Transition.pResource = pBackBuffer;
	barriorDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barriorDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barriorDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mpCommandList->ResourceBarrier(1, &barriorDesc);

	//	グラフィックスパイプラインセット
	mpCommandList->SetPipelineState(mpPipelineState);

	//	レンダーターゲットビューのセット
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeap = mpRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHeap.ptr += backBufferIndex * mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mpCommandList->OMSetRenderTargets
	(
		1, //			レンダーターゲット数
		&rtvHeap, //	レンダーターゲットハンドルのアドレス
		true, //		複数時に連続しているか
		nullptr //		深度ステンシルバッファービューのハンドル
	);

	//	レンダーターゲットのクリア
	float clearColor[] = { 0.5f,0.16f,0.56f,1.0f };
	mpCommandList->ClearRenderTargetView(rtvHeap, clearColor, 0, nullptr);

	//	ビューポートセット
	mpCommandList->RSSetViewports(1,&mViewport);
	//	シザー矩形セット
	mpCommandList->RSSetScissorRects(1, &mScissorRect);

	//	ルートシグネチャセット
	mpCommandList->SetGraphicsRootSignature(mpRootSignature);

	//	描画セット
	mpShape->draw(mpCommandList);

	barriorDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barriorDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	mpCommandList->ResourceBarrier(1, &barriorDesc);

	mpCommandList->Close();

	//	フェンス作成
	//	GPUの処理終了を知ることができる
	ID3D12Fence* fence = nullptr;
	UINT64 fenceValue = 0;
	result = mpDevice->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//	コマンドキューに追加して実行
	ID3D12CommandList* commandLists[] = { mpCommandList };
	mpCommandQueue->ExecuteCommandLists(1, commandLists);
	mpCommandQueue->Signal(fence, ++fenceValue);

	if (fence->GetCompletedValue() != fenceValue)
	{
		//	GPU処理終了時のイベント取得
		void* event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceValue, event);
		if (event == nullptr)
		{
			assert(false && "フェンスのイベントハンドラ作成に失敗しました。");
			return;
		}

		//	イベントが発行されるまで待つ
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	SAFE_RELEASE(fence);
	SAFE_RELEASE(pBackBuffer);

	mpCommandAllocator->Reset();
	mpCommandList->Reset(mpCommandAllocator,nullptr);

	mpSwapChain->Present(1,0);
}

void DirectXSystem::onInitialize()
{
#if _DEBUG

	enableDebugLayer();

#endif

	//	デバイス作成
	createDevice();

	//	DxgiFactory作成
	createDxgiFactory();
	
	//	Command類作成
	createCommandDevices();
	
	//	SwapChain作成
	createSwapChain();
	
	//	レンダーターゲットビュー作成
	createRenderTargetView();

	//	グラフィックスパイプラインステート作成
	createGraphicsPipelineState();

	//	図形作成
	Vertex vertex1 = { {-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f} };
	Vertex vertex2 = { {-0.4f,  0.7f, 0.0f}, {0.0f, 0.0f} };
	Vertex vertex3 = { { 0.4f, -0.7f, 0.0f}, {1.0f, 1.0f} };
	Vertex vertex4 = { { 0.4f,  0.7f, 0.0f}, {1.0f, 0.0f} };
	mpShape = new Rectangle(vertex1,vertex2,vertex3, vertex4);
	//mpShape = new Triangle(vertex1, vertex2, vertex3);
}

void DirectXSystem::onDestroy()
{
	SAFE_DELETE(mpShape);
	//	create〇〇の場合はSAFE_RELEASE
	SAFE_RELEASE(mpRootSignature);
	SAFE_RELEASE(mpPipelineState);
	SAFE_RELEASE(mpRtvDescriptorHeap);
	SAFE_RELEASE(mpCommandQueue);
	SAFE_RELEASE(mpCommandList);
	SAFE_RELEASE(mpCommandAllocator);
	//SAFE_RELEASE(mpSwapChain);
	SAFE_RELEASE(mpDxgiFactory);
	SAFE_RELEASE(mpDevice);

}

void DirectXSystem::createDevice()
{
	/// フィーチャーレベル（優先度が高い順）
	const D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	/// デバイスの作成
	for (auto level : levels)
	{
		if (D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(&mpDevice)) == S_OK)
		{
			break;
		}
	}
	assert(mpDevice && "DirectXデバイス作成に失敗しました。");
}

void DirectXSystem::createDxgiFactory()
{
	HRESULT result = S_OK;
#if _DEBUG
	result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mpDxgiFactory));
#elif
	result = CreateDXGIFactory(IID_PPV_ARGS(&mpDxgiFactory));
#endif

	throwAssertIfFailed(result, "DXGIFactory作成に失敗しました。");
}

void DirectXSystem::createCommandDevices()
{
	/// CommandAllocator作成
	HRESULT result = mpDevice->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mpCommandAllocator)
	);
	throwAssertIfFailed(result, "CommandAllocatorの作成に失敗しました。");

	/// CommandList作成
	result = mpDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mpCommandAllocator,
		nullptr,
		IID_PPV_ARGS(&mpCommandList)
	);
	throwAssertIfFailed(result,"CommandListの作成に失敗しました。");
	mpCommandList->Close();
	mpCommandList->Reset(mpCommandAllocator, nullptr);

	/// コマンドキュー作成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = mpDevice->CreateCommandQueue
	(
		&commandQueueDesc,
		IID_PPV_ARGS(&mpCommandQueue)
	);
	throwAssertIfFailed(result,"CommandQueueの作成に失敗しました。");
}

void DirectXSystem::createSwapChain()
{
	/// SpawnChainのdesc作成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.Width = Application::instance()->getWindow()->getWindowWidth();
	swapChainDesc.Height = Application::instance()->getWindow()->getWindowHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;  ///	3Dディスプレイのステレオモード
	swapChainDesc.SampleDesc.Count = 1; ///	マルチサンプルの指定
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = mBackBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH; ///	バックバッファーは伸び縮可能
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; ///	フリップ後破棄
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED; ///	アルファモード指定なし
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; ///	ウィンドウ⇔フルスクリーン切り替え可能
	
	///	SwapChain作成
	HWND windowHandle = Application::instance()->getWindow()->getWindowHandle();
	ComPtr<IDXGISwapChain1> tempSwapChain = nullptr;
	HRESULT result = mpDxgiFactory->CreateSwapChainForHwnd
	(
		mpCommandQueue,
		windowHandle,
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)tempSwapChain.GetAddressOf()
	);
	tempSwapChain.As(&mpSwapChain);

	throwAssertIfFailed(result, "SwapChainの作成に失敗しました。");
}

void DirectXSystem::createRenderTargetView()
{
	//	レンダーターゲットビューのためのディスクリプターヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	//	タイプはレンダーターゲットビュー
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//	他にGPUはないので0
	heapDesc.NodeMask = 0;
	//	表、裏の分の２つ
	heapDesc.NumDescriptors = mBackBufferCount;
	//	シェーダーからの見え方
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//	作成
	HRESULT result = mpDevice->CreateDescriptorHeap(&heapDesc,IID_PPV_ARGS(&mpRtvDescriptorHeap));
	throwAssertIfFailed(result, "レンダーターゲットビューのためのディスクリプターヒープの作成に失敗しました。");

	//	スワップチェーンと紐づけ
	vector<ID3D12Resource*> backBuffers(mBackBufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mpRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < mBackBufferCount; i++)
	{
		//	バックバッファー取得
		result = mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		throwAssertIfFailed(result, "バックバッファーの取得に失敗しました。");

		//	紐づけ
		mpDevice->CreateRenderTargetView(backBuffers[i],nullptr,handle);
		handle.ptr += mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//	backBufferのRelease
	for (auto backBuffer : backBuffers)
	{
		SAFE_RELEASE(backBuffer);
	}
	backBuffers.clear();
}

void DirectXSystem::createGraphicsPipelineState()
{
	//	シェーダー読み込み
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//	頂点シェーダー
	auto result = D3DCompileFromFile
	(
		L"VertexShader.hlsl", //	シェーダーファイル名
		nullptr, //		シェーダーマクロオブジェクト
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //	カレントディレクトリからインクルードファイルを検索
		"vertexShader", //		エントリポイント
		"vs_5_0",//		シェーダーのバージョン
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //		シェーダーコンパイルオプション
		0, //	シェーダーコンパイルオプション2
		&vsBlob, //		受け取るBlobオブジェクト
		&errorBlob //	受け取るエラー通知オブジェクト
	);
	throwAssertIfFailed(result, "頂点シェーダーの読み込みに失敗しました。");

	//	ピクセルシェーダー
	result = D3DCompileFromFile
	(
		L"PixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"pixelShader",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob,
		&errorBlob
	);
	throwAssertIfFailed(result, "ピクセルシェーダーの読み込みに失敗しました。");

	D3D12_INPUT_ELEMENT_DESC inputLayouts[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}, //	座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0} //	uv
	};
	
	//	グラフィックスパイプライン作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc = {};

	//	ルートシグネチャ作成（GPUで使用するパラメータの型と並びを規定するもの）
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	//	頂点情報（入力アセンブラ）がある
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* rootSignatureBlob = nullptr;
	result = D3D12SerializeRootSignature
	(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSignatureBlob,
		&errorBlob
	);
	throwAssertIfFailed(result, "ルートシグネチャBlobの作成に失敗しました。");
	
	result = mpDevice->CreateRootSignature
	(
		0,
		rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&mpRootSignature)
	);
	throwAssertIfFailed(result, "ルートシグネチャの作成に失敗しました。");
	SAFE_RELEASE(rootSignatureBlob);

	//	ルートシグネチャセット
	pipeLineDesc.pRootSignature = mpRootSignature;

	//	シェーダー
	pipeLineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipeLineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipeLineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipeLineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//	ラスタライザー設定
	//	サンプルマスク
	pipeLineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//	アンチエイリアス
	pipeLineDesc.RasterizerState.MultisampleEnable = false;
	//	カリング
	pipeLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//	塗りつぶし
	pipeLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//	深度
	pipeLineDesc.RasterizerState.DepthClipEnable = true;
	
	//	ブレンディング設定
	//	アルファブレンディング
	pipeLineDesc.BlendState.AlphaToCoverageEnable = false;
	//	ブレンドステートをRenderTarget[0]に固定
	pipeLineDesc.BlendState.IndependentBlendEnable = false;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	//	ブレンド演算をするか
	renderTargetBlendDesc.BlendEnable = false;
	//	論理演算をするか
	renderTargetBlendDesc.LogicOpEnable = false; 
	//	Red,Green,Blue,Alphaすべてブレンド対象にする
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	pipeLineDesc.BlendState.RenderTarget[0] = renderTargetBlendDesc;

	//	入力レイアウト設定
	pipeLineDesc.InputLayout.pInputElementDescs = inputLayouts;
	pipeLineDesc.InputLayout.NumElements = _countof(inputLayouts);

	//	トポロジー設定
	pipeLineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	pipeLineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//	レンダーターゲット設定
	pipeLineDesc.NumRenderTargets = 1;
	pipeLineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//	アンチエイリアシング
	pipeLineDesc.SampleDesc.Count = 1;
	pipeLineDesc.SampleDesc.Quality = 0;

	//	グラフィックスパイプラインステート作成
	result = mpDevice->CreateGraphicsPipelineState(&pipeLineDesc, IID_PPV_ARGS(&mpPipelineState));

	//	ビューポート初期化
	mViewport.Width = (float)Application::instance()->getWindow()->getWindowWidth();
	mViewport.Height = (float)Application::instance()->getWindow()->getWindowHeight();
	//	左上x座標
	mViewport.TopLeftX = 0;
	//	左上y座標
	mViewport.TopLeftY = 0;
	//	最大深度
	mViewport.MaxDepth = 1.0f;
	//	最低深度
	mViewport.MinDepth = 0.0f;

	//	シザー矩形初期化
	mScissorRect.top = 0;
	mScissorRect.bottom = mScissorRect.top + (LONG)Application::instance()->getWindow()->getWindowHeight();
	mScissorRect.left = 0;
	mScissorRect.right = mScissorRect.left + (LONG)Application::instance()->getWindow()->getWindowWidth();

}

void DirectXSystem::enableDebugLayer()
{
	ID3D12Debug* debugLayer = nullptr;
	HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	throwAssertIfFailed(result,"デバッグレイヤーの作成に失敗しました。");

	debugLayer->EnableDebugLayer();
	SAFE_RELEASE(debugLayer);
}