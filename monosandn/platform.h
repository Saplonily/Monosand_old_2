#pragma once
#ifndef H_PLATFORM
#define H_PLATFORM

#include <string>

class platform abstract
{
public:
	virtual void init() = 0;
	virtual void* create_window(int32_t width, int32_t height, const std::wstring& title) = 0;
	virtual bool need_shutdown() = 0;
	virtual void pool_events() = 0;
	virtual void swap_buffers() = 0;
	virtual void sleep_us(int64_t us) = 0;
};

#endif