#pragma once
#ifndef H_TEXTURE2D
#define H_TEXTURE2D

#include "texture_enums.h"
#include <stdint.h>

// rgba or rgb formatted texture
class texture2d
{
public:
    using byte = unsigned char;
    inline virtual ~texture2d() {};
    inline texture2d() {};
    texture2d(const texture2d&) = delete;
    texture2d(texture2d&&) = delete;

    virtual void init();
    virtual void set_wrap_s_t(tex_wrap_type s, tex_wrap_type t) = 0;
    virtual void set_filter(tex_filter_type min_f, tex_filter_type mag_f) = 0;
    virtual void set_data(int32_t width, int32_t height, tex_format t_fmt, byte* data = nullptr) = 0;
    virtual int32_t width() const = 0;
    virtual int32_t height() const = 0;
    virtual tex_format format() const = 0;
};

#endif