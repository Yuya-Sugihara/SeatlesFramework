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
			///	����ɔj�������̂Ŗ����I��delete����K�v�Ȃ�
			//SAFE_DELETE(mpDevice);
			//SAFE_DELETE(mpDxgiFactory);
			//SAFE_DELETE(mpSwapChain);
			//SAFE_DELETE(mpCommandAllocator);
			//SAFE_DELETE(mpCommandList);
		}

		void DirectXSystem::initialize()
		{
			///	�f�o�C�X�쐬
			createDevice();

			///	DxgiFactory�쐬
			createDxgiFactory();

			/// Command�ލ쐬
			createCommandDevices();

			///	SwapChain�쐬
			createSwapChain();

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
			CreateDXGIFactory(IID_PPV_ARGS(&mpDxgiFactory));
			assert(mpDxgiFactory && "DXGIFactory�쐬�Ɏ��s���܂����B");
		}

		void DirectXSystem::createCommandDevices()
		{
			/// CommandAllocator�쐬
			mpDevice->CreateCommandAllocator
			(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&mpCommandAllocator)
			);
			assert(mpCommandAllocator && "CommandAllocator�̍쐬�Ɏ��s���܂����B");

			/// CommandList�쐬
			mpDevice->CreateCommandList
			(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				mpCommandAllocator,
				nullptr,
				IID_PPV_ARGS(&mpCommandList)
			);
			assert(mpCommandList && "CommandList�̍쐬�Ɏ��s���܂����B");

			/// �R�}���h�L���[�쐬
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
			assert(mpCommandQueue && "CommandQueue�̍쐬�Ɏ��s���܂����B");
		}

		void DirectXSystem::createSwapChain()
		{

		}
	}
}