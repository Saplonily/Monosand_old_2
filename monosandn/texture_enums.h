#pragma once
#ifndef H_TEXTURE_ENUMS
#define H_REXTURE_ENUMS

#include <cstdint>
#include "err_macros.h"

enum class tex_wrap_type
{
    repeat = 1,
    mirrored_repeat,
    clamp_to_edge
};

enum class tex_filter_type
{
    nearest = 1,
    linear,
    nearest_mipmap_nearest,
    linear_mipmap_nearest,
    nearest_mipmap_linear,
    linear_mipmap_linear
};

enum class tex_format
{
    rgba = 1,
    rgb,
    red
};

constexpr inline int32_t get_tex_format_pixel_size(tex_format fmt)
{
    switch (fmt)
    {
    case tex_format::rgba:
        return 4;
    case tex_format::rgb:
        return 3;
    case tex_format::red:
        return 1;
    }
    fail_assert("Invalid enum");
    return -1;
}

#endif