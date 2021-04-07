#pragma once
#include "directX.h"
#include "component.h"

namespace seatlesFramework
{
	/// <summary>
	/// GameObject‚ÌPosition,Rotation,Scale‚Ì’l‚ğŠÇ—‚·‚é
	/// </summary>
	class Transform: public Component
	{
	public:
		Transform();
		~Transform();

		void awake() override;
		void update() override;

		ID3D12DescriptorHeap* getDescriptorHeap() const { return mpDescriptorHeap; }
	private: 
		XMMATRIX mWorldMatrix;

		ID3D12Resource* mpConstantBuffer;
		ID3D12DescriptorHeap* mpDescriptorHeap;
	};
}