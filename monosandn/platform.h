#pragma once
#ifndef H_PLATFORM
#define H_PLATFORM

#include <glm/glm.hpp>
#include "texture2d.h"
#include <string>

class platform abstract
{
public:
    virtual void init() = 0;
    virtual void* create_window(int32_t width, int32_t height, const std::wstring& title) = 0;
    virtual bool need_shutdown() const = 0;
    virtual void pool_events() = 0;
    virtual void swap_buffers() = 0;
    virtual void sleep_us(int64_t us) = 0;

    virtual void draw_texture(const texture2d& tex, const glm::mat3& trans) = 0;
    virtual void fill_color(float r, float g, float b) = 0;

    virtual int32_t window_width() const = 0;
    virtual int32_t window_height() const = 0;
    virtual double get_time() const = 0;

    void draw_texture(const texture2d& tex, glm::vec2 pos, float rotation = 0.0f, glm::vec2 scale = glm::vec2{ 1.0f });
    // origin 是 by scale 的
    void draw_texture_ext(const texture2d& tex, glm::vec2 pos, float rotation, glm::vec2 origin, glm::vec2 scale = glm::vec2{ 1.0f });
};

#endif