#pragma once

#include "singletonBase.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

namespace SeatlesFramework
{
	namespace render
	{
		/// <summary>
		/// directxのシステムクラス、directx本体の処理を管理する
		/// </summary>
		class DirectXSystem :public SingletonBase<DirectXSystem>
		{
		public:
			friend class SingletonBase<DirectXSystem>;

			void update();

		protected:
			DirectXSystem();
			DirectXSystem(const DirectXSystem&);
			~DirectXSystem();

			void onInitialize() override;
			void onDestroy() override;

		private:
			void createDevice();
			void createDxgiFactory();
			void createCommandDevices();
			void createSwapChain();
			void createRenderTargetView();
			
#if _DEBUG
			void enableDebugLayer();
#endif

			ID3D12Device* mpDevice;
			IDXGIFactory6* mpDxgiFactory;
			ComPtr<IDXGISwapChain3> mpSwapChain;

			ID3D12CommandAllocator* mpCommandAllocator;
			ID3D12GraphicsCommandList* mpCommandList;
			ID3D12CommandQueue* mpCommandQueue;
			ID3D12DescriptorHeap* mpRtvDescriptorHeap;

			int mBackBufferCount;
		};
	}
}

