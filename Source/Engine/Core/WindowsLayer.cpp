#include "Asserts.h"
#include "PlatformLayer.h"

#if FATAL_PLATFORM_WINDOWS
#include "WindowsLayer.h"

#include <FatalPCH.h>
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include <windowsx.h>

static float         clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, uint32_t msg, WPARAM w_param, LPARAM l_param);

namespace Platform
{
bool PlatformState::start_platform(std::string_view app_name, int32_t x, int32_t y, int32_t width, int32_t height)
{
	m_InternalState = new InternalState;

	auto* st       = static_cast<InternalState*>(m_InternalState);
	st->m_Instance = GetModuleHandleA(0);

	HICON     icon = LoadIcon(st->m_Instance, IDI_APPLICATION);
	WNDCLASSA win;
	memset(&win, 0, sizeof win);

	win.style         = CS_DBLCLKS;
	win.lpfnWndProc   = win32_process_message;
	win.cbClsExtra    = 0;
	win.cbWndExtra    = 0;
	win.hInstance     = st->m_Instance;
	win.hIcon         = icon;
	win.hCursor       = LoadCursor(NULL, IDC_ARROW);
	win.hbrBackground = nullptr;
	win.lpszClassName = "FatalEngine";

	if(!RegisterClassA(&win))
	{
		MessageBoxA(0, "Window couldn't register properly.", "Error", MB_ICONEXCLAMATION | MB_OK);
	}

	uint32_t client_x      = x;
	uint32_t client_y      = y;
	uint32_t client_width  = width;
	uint32_t client_height = height;

	uint32_t window_x      = client_x;
	uint32_t window_y      = client_y;
	uint32_t window_width  = client_width;
	uint32_t window_height = client_height;

	uint32_t window_style    = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	uint32_t window_ex_style = WS_EX_APPWINDOW;

	window_style |= WS_MAXIMIZEBOX;
	window_style |= WS_MINIMIZEBOX;
	window_style |= WS_THICKFRAME;

	RECT border_rect = { 0, 0, 0, 0 };
	AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

	window_x += border_rect.left;
	window_y += border_rect.top;

	window_width += border_rect.right - border_rect.left;
	window_height += border_rect.bottom - border_rect.top;

	HWND handle = CreateWindowExA(window_ex_style, "FatalEngine", app_name.data(), window_style, window_x, window_y,
		window_width, window_height, 0, 0, st->m_Instance, 0);

	if(!handle)
	{
		MessageBoxA(nullptr, "Window creation failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
	}
	else
	{
		st->m_HWND = handle;
	}

	bool    should_activate = true;
	int32_t window_flags    = should_activate == true ? SW_SHOW : SW_SHOWNOACTIVATE;
	ShowWindow(st->m_HWND, window_flags);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0f / static_cast<float>(frequency.QuadPart);
	QueryPerformanceCounter(&start_time);

	return true;
}

bool PlatformState::shut_down_platform()
{
	InternalState* st = static_cast<InternalState*>(m_InternalState);

	if(st->m_HWND)
	{
		DestroyWindow(st->m_HWND);
		st->m_HWND = nullptr;
	}
	return true;
}

bool PlatformState::pump_messages()
{
	MSG msg;
	while(PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return true;
}

void* PlatformState::get_state() const noexcept
{
	return m_InternalState;
}

PlatformState::~PlatformState()
{
	shut_down_platform();

	delete m_InternalState;
	m_InternalState = nullptr;
}

void sleep(unsigned long miliseconds)
{
	Sleep(miliseconds);
}

void* platform_allocate(uint64_t size, [[maybe_unused]] bool aligned)
{
	return malloc(size);
}

void platform_free(void* block, [[maybe_unused]] bool aligned)
{
	free(block);
}

void* platform_zero_memory(void* block, uint64_t size)
{
	return memset(block, 0, size);
}

void* platform_copy_memory(void* destination, void const* source, uint64_t size)
{
	return memcpy(destination, source, size);
}

void* platform_set_memory(void* destination, int32_t value, uint64_t size)
{
	return memset(destination, value, size);
}

const char* get_vulkan_extension_name()
{
	return "VK_KHR_win32_surface";
}

VkSurfaceKHR create_vulkan_surface(VkInstance const& instance, void* state)
{
	auto st = static_cast<InternalState*>(state);

	VkWin32SurfaceCreateInfoKHR create_info = {
		.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		.hinstance = st->m_Instance,
		.hwnd      = st->m_HWND,
	};

	VkSurfaceKHR surface;
	Assert::fatal_vk_assert(vkCreateWin32SurfaceKHR(instance, &create_info, nullptr, &surface));

	return surface;
}
} // namespace Platform

LRESULT CALLBACK win32_process_message(HWND hwnd, uint32_t msg, WPARAM w_param, LPARAM l_param)
{
	switch(msg)
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_CLOSE:
			// TODO: fire event to quit
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
		{
			/*
		TODO: fire event to resize
		RECT r;
		GetClientRect(hwnd, &r);
		uint32_t width = r.right - r.left;
		uint32_t height = r.bottom - r.top;
		*/
		}
		break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			// bool pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
			// TODO: input processing
		}
		break;
		case WM_MOUSEMOVE:
		{
			/*
		int32_t x_pos = GET_X_LPARAM(l_param);
		int32_t y_pos = GET_Y_LPARAM(l_param);
	  */
		}
		break;
		case WM_MOUSEWHEEL:
		{
			/*
		  int32_t z_dt = GET_WHEEL_DELTA_WPARAM(w_param);
		if (z_dt != 0) {
		  z_dt = (z_dt < 0) ? -1 : 1;
		}
		*/
		}
		break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			// bool pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg ==
			// WM_MBUTTONDOWN;
		}
		break;
	}

	return DefWindowProcA(hwnd, msg, w_param, l_param);
}
#endif
