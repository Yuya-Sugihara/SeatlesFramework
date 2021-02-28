#include "directXSystem.h"
#include <cassert>

namespace SeatlesFramework
{
	render::DirectXSystem* render::DirectXSystem::mpInstance = nullptr;

	namespace render
	{
		DirectXSystem::DirectXSystem():
			mpDevice(nullptr),
			mpDxgiFactory(nullptr),
			mpSwapChain(nullptr),
			mpCommandAllocator(nullptr),
			mpCommandList(nullptr),
			mpCommandQueue(nullptr)
		{
			initialize();
		}

		DirectXSystem::DirectXSystem(const DirectXSystem&) :
			mpDevice(nullptr),
			mpDxgiFactory(nullptr),
			mpSwapChain(nullptr),
			mpCommandAllocator(nullptr),
			mpCommandList(nullptr),
			mpCommandQueue(nullptr)
		{}

		DirectXSystem::~DirectXSystem()
		{
			///	勝手に破棄されるので明示的にdeleteする必要なし
			//SAFE_DELETE(mpDevice);
			//SAFE_DELETE(mpDxgiFactory);
			//SAFE_DELETE(mpSwapChain);
			//SAFE_DELETE(mpCommandAllocator);
			//SAFE_DELETE(mpCommandList);
		}

		void DirectXSystem::initialize()
		{
			///	デバイス作成
			createDevice();

			///	DxgiFactory作成
			createDxgiFactory();

			/// Command類作成
			createCommandDevices();

			///	SwapChain作成
			createSwapChain();

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
			CreateDXGIFactory(IID_PPV_ARGS(&mpDxgiFactory));
			assert(mpDxgiFactory && "DXGIFactory作成に失敗しました。");
		}

		void DirectXSystem::createCommandDevices()
		{
			/// CommandAllocator作成
			mpDevice->CreateCommandAllocator
			(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&mpCommandAllocator)
			);
			assert(mpCommandAllocator && "CommandAllocatorの作成に失敗しました。");

			/// CommandList作成
			mpDevice->CreateCommandList
			(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				mpCommandAllocator,
				nullptr,
				IID_PPV_ARGS(&mpCommandList)
			);
			assert(mpCommandList && "CommandListの作成に失敗しました。");

			/// コマンドキュー作成
			D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
			commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			commandQueueDesc.NodeMask = 0;
			commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

			mpDevice->CreateCommandQueue
			(
				&commandQueueDesc,
				IID_PPV_ARGS(&mpCommandQueue)
			);
			assert(mpCommandQueue && "CommandQueueの作成に失敗しました。");
		}

		void DirectXSystem::createSwapChain()
		{

		}
	}
}