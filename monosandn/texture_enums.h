#pragma once
#ifndef H_TEXTURE_ENUMS
#define H_REXTURE_ENUMS

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
    rgb
};

#endif