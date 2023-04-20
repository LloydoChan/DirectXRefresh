#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>
#include "BoilerPlate.h"

using namespace Microsoft::WRL;

void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapterPtr;
	ComPtr<IDXGIFactory6> factoryPtr;

	if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factoryPtr))))
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(factoryPtr->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapterPtr)));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapterPtr->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			if (SUCCEEDED(D3D12CreateDevice(adapterPtr.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)), adapterIndex++)
			{
				break;
			}
		}
	}

	if (adapterPtr.Get() == nullptr)
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(factoryPtr->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapterPtr)));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapterPtr->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			if (SUCCEEDED(D3D12CreateDevice(adapterPtr.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)), adapterIndex++)
			{
				break;
			}
		}
	}

	*ppAdapter = adapterPtr.Detach();
}