#pragma once
#ifndef H_PLATFORM
#define H_PLATFORM

#include "texture2d.h"
#include "render_target.h"
#include <glm/glm.hpp>
#include <string>
#include <functional>

class platform
{
public:
    using window_resize_callback_t = std::function<void(int32_t width, int32_t height)>;
    static platform* singleton;

    platform();
    virtual ~platform();

    // --抽象方法--
    virtual void init() = 0;
    virtual void create_window(int32_t width, int32_t height, const std::wstring& title) = 0;
    virtual bool need_shutdown() const = 0;
    virtual void pool_events() = 0;
    virtual void swap_buffers() = 0;
    virtual void sleep_us(int64_t us) = 0;
    virtual int64_t get_ticks_persecond() const = 0;
    virtual int64_t get_abs_ticks() const = 0;
    virtual double get_time() const = 0;

    virtual int32_t get_expected_fps() const = 0;
    virtual void set_expected_fps(int32_t fps) = 0;

    // display api
    virtual int32_t window_width() const = 0;
    virtual int32_t window_height() const = 0;
    virtual void set_window_resize_callback(const window_resize_callback_t cb) = 0;

    // render api
    virtual void draw_texture(const texture2d& tex, const glm::mat3& trans) = 0;
    virtual void fill_color(float r, float g, float b) = 0;
    virtual void set_color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) = 0;
    virtual void set_render_target(const render_target& target);
    virtual void reset_render_target() = 0;

    // --非抽象方法--

    void pf_printfn(const char* fmt, ...);
    void pf_wprintfn(const wchar_t* fmt, ...);

    void draw_texture(const texture2d& tex, glm::vec2 pos);
    // origin 是 by scale 的
    void draw_texture(const texture2d& tex, glm::vec2 pos, float rotation, glm::vec2 origin = glm::vec2{ 0.0f }, glm::vec2 scale = glm::vec2{ 1.0f });
};

#endif