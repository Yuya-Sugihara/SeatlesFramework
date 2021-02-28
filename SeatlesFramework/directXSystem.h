#pragma once
#include "singletonBase.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

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

		protected:
			DirectXSystem();
			DirectXSystem(const DirectXSystem&);
			~DirectXSystem();

		private:

			void initialize();
			void createDevice();
			void createDxgiFactory();
			void createCommandDevices();
			void createSwapChain();

			ID3D12Device* mpDevice;
			IDXGIFactory6* mpDxgiFactory;
			IDXGISwapChain4* mpSwapChain;

			ID3D12CommandAllocator* mpCommandAllocator;
			ID3D12CommandList* mpCommandList;
			ID3D12CommandQueue* mpCommandQueue;
		};
	}
}

