#pragma once
#ifndef H_PLATFORM_WIN_GLFW
#define H_PLATFORM_WIN_GLFW

#include "platform.h"
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#undef APIENTRY
#include <GLFW/glfw3native.h>

class platform_win_glfw final : public platform
{
private:
	bool glad_loaded = false;
	GLFWwindow* glfw_win = nullptr;
public:
	void* create_window(int32_t width, int32_t height,const std::wstring& title) override;
	void init() override;
	bool need_shutdown() override;
	void pool_events() override;
	void swap_buffers() override;
	void sleep_us(int64_t us) override;
};

#endif