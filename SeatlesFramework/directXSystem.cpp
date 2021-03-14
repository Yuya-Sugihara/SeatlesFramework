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
	mpRtvDescriptorHeap(nullptr),
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
	mBackBufferCount(2)
{}

DirectXSystem::~DirectXSystem()
{}

void DirectXSystem::update()
{
	int backBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();
	ID3D12Resource* pBackBuffer = nullptr;
	HRESULT result = mpSwapChain->GetBuffer(backBufferIndex,IID_PPV_ARGS(&pBackBuffer));

	//	���\�[�X�o���A�Z�b�g
	//	�����I�Ƀ��\�[�X�����Ɏg�p���邩��GPU�ɓn��
	D3D12_RESOURCE_BARRIER barriorDesc = {};
	barriorDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barriorDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barriorDesc.Transition.pResource = pBackBuffer;
	barriorDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barriorDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barriorDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mpCommandList->ResourceBarrier(1, &barriorDesc);

	//	�����_�[�^�[�Q�b�g�r���[�̃Z�b�g
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeap = mpRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHeap.ptr += backBufferIndex * mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mpCommandList->OMSetRenderTargets
	(
		1, //			�����_�[�^�[�Q�b�g��
		&rtvHeap, //	�����_�[�^�[�Q�b�g�n���h���̃A�h���X
		true, //		�������ɘA�����Ă��邩
		nullptr //		�[�x�X�e���V���o�b�t�@�[�r���[�̃n���h��
	);

	//	�����_�[�^�[�Q�b�g�̃N���A
	float clearColor[] = { 0.5f,0.16f,0.56f,1.0f };
	mpCommandList->ClearRenderTargetView(rtvHeap, clearColor, 0, nullptr);

	barriorDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barriorDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	mpCommandList->ResourceBarrier(1, &barriorDesc);

	mpCommandList->Close();

	//	�t�F���X�쐬
	//	GPU�̏����I����m�邱�Ƃ��ł���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceValue = 0;
	result = mpDevice->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//	�R�}���h�L���[�ɒǉ����Ď��s
	ID3D12CommandList* commandLists[] = { mpCommandList };
	mpCommandQueue->ExecuteCommandLists(1, commandLists);
	mpCommandQueue->Signal(fence, ++fenceValue);

	if (fence->GetCompletedValue() != fenceValue)
	{
		//	GPU�����I�����̃C�x���g�擾
		void* event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceValue, event);
		if (event == nullptr)
		{
			assert(false && "�t�F���X�̃C�x���g�n���h���쐬�Ɏ��s���܂����B");
			return;
		}

		//	�C�x���g�����s�����܂ő҂�
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

	//	�f�o�C�X�쐬
	createDevice();

	//	DxgiFactory�쐬
	createDxgiFactory();
	
	//	Command�ލ쐬
	createCommandDevices();
	
	//	SwapChain�쐬
	createSwapChain();
	
	//	�����_�[�^�[�Q�b�g�r���[�쐬
	createRenderTargetView();
}

void DirectXSystem::onDestroy()
{
	//	create�Z�Z�̏ꍇ��SAFE_RELEASE
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
	/// �t�B�[�`���[���x���i�D��x���������j
	const D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	/// �f�o�C�X�̍쐬
	for (auto level : levels)
	{
		if (D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(&mpDevice)) == S_OK)
		{
			break;
		}
	}
	assert(mpDevice && "DirectX�f�o�C�X�쐬�Ɏ��s���܂����B");
}

void DirectXSystem::createDxgiFactory()
{
	HRESULT result = S_OK;
#if _DEBUG
	result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mpDxgiFactory));
#elif
	result = CreateDXGIFactory(IID_PPV_ARGS(&mpDxgiFactory));
#endif

	throwAssertIfFailed(result, "DXGIFactory�쐬�Ɏ��s���܂����B");
}

void DirectXSystem::createCommandDevices()
{
	/// CommandAllocator�쐬
	HRESULT result = mpDevice->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mpCommandAllocator)
	);
	throwAssertIfFailed(result, "CommandAllocator�̍쐬�Ɏ��s���܂����B");

	/// CommandList�쐬
	result = mpDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mpCommandAllocator,
		nullptr,
		IID_PPV_ARGS(&mpCommandList)
	);
	throwAssertIfFailed(result,"CommandList�̍쐬�Ɏ��s���܂����B");
	mpCommandList->Close();
	mpCommandList->Reset(mpCommandAllocator, nullptr);

	/// �R�}���h�L���[�쐬
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
	throwAssertIfFailed(result,"CommandQueue�̍쐬�Ɏ��s���܂����B");
}

void DirectXSystem::createSwapChain()
{
	/// SpawnChain��desc�쐬
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.Width = Application::instance()->getWindow()->getWindowWidth();
	swapChainDesc.Height = Application::instance()->getWindow()->getWindowHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;  ///	3D�f�B�X�v���C�̃X�e���I���[�h
	swapChainDesc.SampleDesc.Count = 1; ///	�}���`�T���v���̎w��
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = mBackBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH; ///	�o�b�N�o�b�t�@�[�͐L�яk�\
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; ///	�t���b�v��j��
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED; ///	�A���t�@���[�h�w��Ȃ�
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; ///	�E�B���h�E�̃t���X�N���[���؂�ւ��\
	
	///	SwapChain�쐬
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

	throwAssertIfFailed(result, "SwapChain�̍쐬�Ɏ��s���܂����B");
}

void DirectXSystem::createRenderTargetView()
{
	//	�����_�[�^�[�Q�b�g�r���[�̂��߂̃f�B�X�N���v�^�[�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	//	�^�C�v�̓����_�[�^�[�Q�b�g�r���[
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//	����GPU�͂Ȃ��̂�0
	heapDesc.NodeMask = 0;
	//	�\�A���̕��̂Q��
	heapDesc.NumDescriptors = mBackBufferCount;
	//	�V�F�[�_�[����̌�����
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//	�쐬
	HRESULT result = mpDevice->CreateDescriptorHeap
	(
		&heapDesc,
		IID_PPV_ARGS(&mpRtvDescriptorHeap)
	);
	throwAssertIfFailed(result, "�����_�[�^�[�Q�b�g�r���[�̂��߂̃f�B�X�N���v�^�[�q�[�v�̍쐬�Ɏ��s���܂����B");

	//	�X���b�v�`�F�[���ƕR�Â�
	vector<ID3D12Resource*> backBuffers(mBackBufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mpRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < mBackBufferCount; i++)
	{
		//	�o�b�N�o�b�t�@�[�擾
		result = mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		throwAssertIfFailed(result, "�o�b�N�o�b�t�@�[�̎擾�Ɏ��s���܂����B");

		//	�R�Â�
		mpDevice->CreateRenderTargetView(backBuffers[i],nullptr,handle);
		handle.ptr += mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//	backBuffer��Release
	for (auto backBuffer : backBuffers)
	{
		SAFE_RELEASE(backBuffer);
	}
	backBuffers.clear();
}

void DirectXSystem::enableDebugLayer()
{
	ID3D12Debug* debugLayer = nullptr;
	HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	throwAssertIfFailed(result,"�f�o�b�O���C���[�̍쐬�Ɏ��s���܂����B");

	debugLayer->EnableDebugLayer();
	SAFE_RELEASE(debugLayer);
}