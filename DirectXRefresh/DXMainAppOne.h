#pragma once

#include <wrl/client.h>
#include <d3d12.h>
#include <stdexcept>
#include <dxgi1_6.h>
#include <shellapi.h>
#include "BoilerPlate.h"
#include <DirectXMath.h>
#include "d3dx12.h"


using Microsoft::WRL::ComPtr;

struct XMFLOAT3
{
	float x;
	float y;
	float z;

	XMFLOAT3() = default;

	XMFLOAT3(const XMFLOAT3&) = default;
	XMFLOAT3& operator=(const XMFLOAT3&) = default;

	XMFLOAT3(XMFLOAT3&&) = default;
	XMFLOAT3& operator=(XMFLOAT3&&) = default;

	XM_CONSTEXPR XMFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	explicit XMFLOAT3(_In_reads_(3) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
};

struct XMFLOAT4
{
	float x;
	float y;
	float z;
	float w;

	XMFLOAT4() = default;

	XMFLOAT4(const XMFLOAT4&) = default;
	XMFLOAT4& operator=(const XMFLOAT4&) = default;

	XMFLOAT4(XMFLOAT4&&) = default;
	XMFLOAT4& operator=(XMFLOAT4&&) = default;

	XM_CONSTEXPR XMFLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	explicit XMFLOAT4(_In_reads_(4) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
};


class DXMainAppOne
{
public:
	DXMainAppOne(UINT width, UINT height);
	void Initialize(HWND hWnd);
	void Render();

private:
	void BuildList();
	void SyncFrame();

	void LoadShaders();
	void LoadTriangleData();

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	CD3DX12_VIEWPORT mViewport;
	CD3DX12_RECT mScissorRect;
	ComPtr<IDXGISwapChain3> mSwapChain;
	ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12Resource> mRenderTargets[2];
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12RootSignature> mRootSignature;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12PipelineState> mPipelineState;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	UINT mRtvDescriptorSize;

	ComPtr<ID3D12Resource> mVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;

	UINT mFrameIndex;
	HANDLE mFenceEvent;
	ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	// Viewport dimensions.
	UINT mWidth;
	UINT mHeight;
	float mAspectRatio;

	HWND mHwnd = nullptr;

	// Root assets path.
	std::wstring mAssetsPath;

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

inline void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
{
	if (path == nullptr)
	{
		throw std::exception();
	}

	DWORD size = GetModuleFileName(nullptr, path, pathSize);
	if (size == 0 || size == pathSize)
	{
		// Method failed or path was truncated.
		throw std::exception();
	}

	WCHAR* lastSlash = wcsrchr(path, L'\\');
	if (lastSlash)
	{
		*(lastSlash + 1) = L'\0';
	}
}