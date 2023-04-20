#pragma once
#include <Windows.h>
#include <dxgi.h>

void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);