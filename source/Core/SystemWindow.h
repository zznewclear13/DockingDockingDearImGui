#pragma once

#include "imgui.h"
#include "backends/imgui_impl_win32.h"

#include "Support/WinInclude.h"
#include "Support/Singleton.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DockingImGui
{
    class SystemWindow
    {
    public:
        inline HWND GetWindowHandle() { return m_wnd; }

        void Initialize();
        void Shutdown();

        void Update();
        bool CloseRequested();
        size_t GetWidth() const { return m_width; }
		size_t GetHeight() const { return m_height; }

    private:
        static LRESULT WindowProc_Setup(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT WindowProc_Redirect(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

        LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        HINSTANCE m_hInstance{ nullptr };
        ATOM m_windowClass{ 0 };
        HWND m_wnd{ nullptr };

        size_t m_width{ 300 };
        size_t m_height{ 300 };
        bool m_closeRequest{ false };
    };
}