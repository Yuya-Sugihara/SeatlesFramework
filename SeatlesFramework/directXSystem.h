#pragma once

#include "singletonBase.h"
#include "directX.h"

using namespace Microsoft::WRL;

namespace seatlesFramework
{
	namespace render
	{
		class Shape;

		/// <summary>
		/// directxのシステムクラス、directx本体の処理を管理する
		/// </summary>
		class DirectXSystem :public SingletonBase<DirectXSystem>
		{
		public:
			friend class SingletonBase<DirectXSystem>;

			void update();

			//	アクセサ
			ID3D12Device* getDevice() const { return mpDevice; }
			ID3D12GraphicsCommandList* getCommandList() const { return mpCommandList; }
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
			void createGraphicsPipelineState();

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
			ID3D12PipelineState* mpPipelineState;
			ID3D12RootSignature* mpRootSignature;

			Shape* mpShape;

			int mBackBufferCount;
			D3D12_VIEWPORT mViewport;
			D3D12_RECT mScissorRect;
		};
	}
}

