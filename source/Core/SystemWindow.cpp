#include "SystemWindow.h"

#include "Core/AppContext.h"

namespace DockingImGui
{
	void SystemWindow::Initialize()
	{
		m_hInstance = GetModuleHandle(nullptr);

		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        
        WNDCLASSEXW wcex{};
        wcex.cbSize = sizeof(wcex);
        wcex.style = CS_OWNDC;
        wcex.lpfnWndProc = &SystemWindow::WindowProc_Setup;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = m_hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"DockingImGuiWindow";
        wcex.hIconSm = nullptr;

        m_windowClass = RegisterClassExW(&wcex);

        DWORD ex_style = WS_EX_TOPMOST;
        DWORD style = WS_POPUP;
        // === Create the window === 
        m_wnd = CreateWindowExW(
            ex_style,
            (LPCWSTR)m_windowClass,
            L"Docking ImGui Invisible Window",
            style,
            10, 10,
            m_width,
            m_height,
            nullptr,
            nullptr,
            m_hInstance,
            this
        );
	}

    void SystemWindow::Update()
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    void SystemWindow::Shutdown()
    {
        if (m_wnd)
        {
            DestroyWindow(m_wnd);
            m_wnd = nullptr;
        }
        if (m_windowClass)
        {
            UnregisterClassW((LPCWSTR)m_windowClass, m_hInstance);
            m_windowClass = 0;
		}
    }

    bool SystemWindow::CloseRequested()
    {
        bool closeRequest = m_closeRequest;
        m_closeRequest = false;
        return closeRequest;
    }

    LRESULT SystemWindow::WindowProc_Setup(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_NCCREATE)
        {
            auto* createStruct = (CREATESTRUCTW*)lparam;
            SetWindowLongPtrW(wnd, GWLP_USERDATA, (LONG_PTR)createStruct->lpCreateParams);
            SetWindowLongPtrW(wnd, GWLP_WNDPROC, (LONG_PTR)&SystemWindow::WindowProc_Redirect);
            return WindowProc_Redirect(wnd, msg, wparam, lparam);
        }
        return DefWindowProcW(wnd, msg, wparam, lparam);
    }

    LRESULT SystemWindow::WindowProc_Redirect(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        auto* window = (SystemWindow*)GetWindowLongPtrW(wnd, GWLP_USERDATA);
        return window->WindowProc(wnd, msg, wparam, lparam);
    }

    LRESULT SystemWindow::WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        // === Our events ===
        switch (msg)
        {
        case WM_CLOSE:
            m_closeRequest = true;
            return 0;
        case WM_SIZE:
            if (LOWORD(lparam) >= 64 && HIWORD(lparam) >= 64)
            {
                m_width = LOWORD(lparam);
                m_height = HIWORD(lparam);
            }
            break;
        }

        // === Fallback default handler ===
        return DefWindowProcW(wnd, msg, wparam, lparam);
    }

}