#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#if defined(_DEBUG) || defined(DEBUG)
#include <dxgidebug.h>
#endif

#include "imgui.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DockingImGui
{
	class ImGuiSystem
	{
	public:
		bool Initialize();
		void Shutdown();

		void Update();
		bool CloseRequested() const { return !show_imgui_window; }
	private:
		ImGuiIO* m_io{ nullptr };
        // Our state
        bool show_imgui_window = true;
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    private:
        // Config for example app
        static const int APP_NUM_FRAMES_IN_FLIGHT = 2;
        static const int APP_NUM_BACK_BUFFERS = 2;
        static const int APP_SRV_HEAP_SIZE = 64;

        struct FrameContext
        {
            ID3D12CommandAllocator*     CommandAllocator;
            UINT64                      FenceValue;
        };

        // Simple free list based allocator
        struct ExampleDescriptorHeapAllocator
        {
            ID3D12DescriptorHeap*       Heap = nullptr;
            D3D12_DESCRIPTOR_HEAP_TYPE  HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
            D3D12_CPU_DESCRIPTOR_HANDLE HeapStartCpu;
            D3D12_GPU_DESCRIPTOR_HANDLE HeapStartGpu;
            UINT                        HeapHandleIncrement;
            ImVector<int>               FreeIndices;

            void Create(ID3D12Device* device, ID3D12DescriptorHeap* heap)
            {
                IM_ASSERT(Heap == nullptr && FreeIndices.empty());
                Heap = heap;
                D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();
                HeapType = desc.Type;
                HeapStartCpu = Heap->GetCPUDescriptorHandleForHeapStart();
                HeapStartGpu = Heap->GetGPUDescriptorHandleForHeapStart();
                HeapHandleIncrement = device->GetDescriptorHandleIncrementSize(HeapType);
                FreeIndices.reserve((int)desc.NumDescriptors);
                for (int n = desc.NumDescriptors; n > 0; n--)
                    FreeIndices.push_back(n - 1);
            }
            void Destroy()
            {
                Heap = nullptr;
                FreeIndices.clear();
            }
            void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
            {
                IM_ASSERT(FreeIndices.Size > 0);
                int idx = FreeIndices.back();
                FreeIndices.pop_back();
                out_cpu_desc_handle->ptr = HeapStartCpu.ptr + (idx * HeapHandleIncrement);
                out_gpu_desc_handle->ptr = HeapStartGpu.ptr + (idx * HeapHandleIncrement);
            }
            void Free(D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle)
            {
                int cpu_idx = (int)((out_cpu_desc_handle.ptr - HeapStartCpu.ptr) / HeapHandleIncrement);
                int gpu_idx = (int)((out_gpu_desc_handle.ptr - HeapStartGpu.ptr) / HeapHandleIncrement);
                IM_ASSERT(cpu_idx == gpu_idx);
                FreeIndices.push_back(cpu_idx);
            }
        };

    private:
        static ExampleDescriptorHeapAllocator g_pd3dSrvDescHeapAlloc;

        // Data
        FrameContext                 g_frameContext[APP_NUM_FRAMES_IN_FLIGHT] = {};
        UINT                         g_frameIndex = 0;

        ID3D12Device*                g_pd3dDevice = nullptr;
        ID3D12DescriptorHeap*        g_pd3dRtvDescHeap = nullptr;
        ID3D12DescriptorHeap*        g_pd3dSrvDescHeap = nullptr;
        ID3D12CommandQueue*          g_pd3dCommandQueue = nullptr;
        ID3D12GraphicsCommandList*   g_pd3dCommandList = nullptr;
        ID3D12Fence*                 g_fence = nullptr;
        HANDLE                       g_fenceEvent = nullptr;
        UINT64                       g_fenceLastSignaledValue = 0;
        IDXGISwapChain3*             g_pSwapChain = nullptr;
        bool                         g_SwapChainOccluded = false;
        HANDLE                       g_hSwapChainWaitableObject = nullptr;
        ID3D12Resource*              g_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};
        D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[APP_NUM_BACK_BUFFERS] = {};
        
    private:
        // Forward declarations of helper functions
        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        void WaitForLastSubmittedFrame();
        FrameContext* WaitForNextFrameResources();

    public:
        LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}