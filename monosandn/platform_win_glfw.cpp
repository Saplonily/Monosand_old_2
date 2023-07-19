#include "platform_win_glfw.h"
#include <codecvt>
#undef APIENTRY
#include <Windows.h>

#pragma comment(lib, "winmm")

void* platform_win_glfw::create_window(int32_t width, int32_t height, const std::wstring& title)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
    const std::string& str = cvt.to_bytes(title);
    glfw_win = glfwCreateWindow(width, height, str.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(glfw_win);
    if (!glad_loaded)
    {
        glad_loaded = true;
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }
    return glfw_win;
}

void platform_win_glfw::init()
{
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback([](int code, const char* msg)
        {
            printf("你踏马error了!, code: %d, msg: %s\n", code, msg);
        });

    // windows init
    SetConsoleOutputCP(CP_UTF8);
    timeBeginPeriod(1);
}

void platform_win_glfw::pool_events() { glfwPollEvents(); }

void platform_win_glfw::swap_buffers() { glfwSwapBuffers(glfw_win); }

bool platform_win_glfw::need_shutdown() { return glfwWindowShouldClose(glfw_win); }

void platform_win_glfw::sleep_us(int64_t us)
{
    DWORD t = static_cast<DWORD>(us / 1000.0);
    Sleep(us <= 1000 ? 1 : t);
}