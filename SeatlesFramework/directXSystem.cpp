#include "directXSystem.h"
#include "application.h"
#include "window.h"
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
	mBackBufferCount(2)
{}

DirectXSystem::DirectXSystem(const DirectXSystem&) :
	mpDevice(nullptr),
	mpDxgiFactory(nullptr),
	mpSwapChain(nullptr),
	mpCommandAllocator(nullptr),
	mpCommandList(nullptr),
	mpCommandQueue(nullptr),
	mBackBufferCount(2)
{}

DirectXSystem::~DirectXSystem()
{}

void DirectXSystem::update()
{
	//	コマンドアロケータリセット
	HRESULT result = mpCommandAllocator->Reset();
	throwAssertIfFailed(result,"コマンドアロケータのリセット処理に失敗しました。");

	//	レンダーターゲットビューのセット
	int backBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();
}

void DirectXSystem::onInitialize()
{
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
}

void DirectXSystem::onDestroy()
{
	///	勝手に破棄されるので明示的にdeleteする必要なし
	//SAFE_DELETE(mpDevice);
	//SAFE_DELETE(mpDxgiFactory);
	//SAFE_DELETE(mpSwapChain);
	//SAFE_DELETE(mpCommandAllocator);
	//SAFE_DELETE(mpCommandList);
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
	HRESULT result = CreateDXGIFactory(IID_PPV_ARGS(&mpDxgiFactory));
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
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	HRESULT result = mpDevice->CreateDescriptorHeap
	(
		&heapDesc,
		IID_PPV_ARGS(&rtvHeap)
	);
	throwAssertIfFailed(result, "レンダーターゲットビューのためのディスクリプターヒープの作成に失敗しました。");

	//	スワップチェーンと紐づけ
	vector<ID3D12Resource*> backBuffers(mBackBufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < mBackBufferCount; i++)
	{
		//	バックバッファー取得
		result = mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		throwAssertIfFailed(result, "バックバッファーの取得に失敗しました。");

		//	紐づけ
		mpDevice->CreateRenderTargetView(backBuffers[i],nullptr,handle);
		handle.ptr += mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
}

