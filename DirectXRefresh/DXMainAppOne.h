#pragma once
#include "BoilerPlate.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <stdexcept>
#include <dxgi1_4.h>

using Microsoft::WRL::ComPtr;

class DXMainAppOne
{
public:
	DXMainAppOne(UINT width, UINT height);
	void Initialize(HWND hWnd);
	void Render();

private:
	void BuildList();
	void SyncFrame();

	ComPtr<IDXGISwapChain3> mSwapChain;
	ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12Resource> mRenderTargets[2];
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12PipelineState> mPipelineState;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	UINT mRtvDescriptorSize;

	UINT mFrameIndex;
	HANDLE mFenceEvent;
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	// Viewport dimensions.
	UINT mWidth;
	UINT mHeight;
	float mAspectRatio;

	HWND mHwnd = nullptr;

	static const UINT FrameCount = 2;
};

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw HrException(hr);
	}
}