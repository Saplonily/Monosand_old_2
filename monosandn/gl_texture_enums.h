#pragma once
#ifndef GL_TEXTURE_ENUMS
#define GL_TEXTURE_ENUMS

#include "texture_enums.h"
#include "err_macros.h"

#include <glad/glad.h>
#include <cstdint>
#include <cassert>

static const char* invalid_enum_msg = "Invalid enum";

constexpr inline uint32_t to_gl_wrap_type(tex_wrap_type t)
{
    switch (t)
    {
    case tex_wrap_type::repeat:
        return GL_REPEAT;
    case tex_wrap_type::mirrored_repeat:
        return GL_MIRRORED_REPEAT;
    case tex_wrap_type::clamp_to_edge:
        return GL_CLAMP_TO_EDGE;
    }
    fail_assert(invalid_enum_msg);
    return -1;
}

constexpr inline tex_wrap_type from_gl_wrap_type(uint32_t t)
{
    switch (t)
    {
    case GL_REPEAT:
        return tex_wrap_type::repeat;
    case GL_MIRRORED_REPEAT:
        return tex_wrap_type::mirrored_repeat;
    case GL_CLAMP_TO_EDGE:
        return tex_wrap_type::clamp_to_edge;
    }
    fail_assert(invalid_enum_msg);
    return static_cast<tex_wrap_type>(-1);
}

constexpr inline uint32_t to_gl_filter_type(tex_filter_type t)
{
    switch (t)
    {
    case tex_filter_type::nearest:
        return GL_NEAREST;
    case tex_filter_type::linear:
        return GL_LINEAR;
    case tex_filter_type::nearest_mipmap_nearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    case tex_filter_type::linear_mipmap_nearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case tex_filter_type::nearest_mipmap_linear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case tex_filter_type::linear_mipmap_linear:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
    fail_assert(invalid_enum_msg);
    return -1;
}

constexpr inline tex_filter_type from_gl_filter_type(uint32_t t)
{
    switch (t)
    {
    case GL_NEAREST:
        return tex_filter_type::nearest;
    case GL_LINEAR:
        return tex_filter_type::linear;
    case GL_NEAREST_MIPMAP_NEAREST:
        return tex_filter_type::nearest_mipmap_nearest;
    case GL_LINEAR_MIPMAP_NEAREST:
        return tex_filter_type::linear_mipmap_nearest;
    case GL_NEAREST_MIPMAP_LINEAR:
        return tex_filter_type::nearest_mipmap_linear;
    case GL_LINEAR_MIPMAP_LINEAR:
        return tex_filter_type::linear_mipmap_linear;
    }
    fail_assert(invalid_enum_msg);
    return static_cast<tex_filter_type>(-1);
}

constexpr inline uint32_t to_gl_tex_format(tex_format t)
{
    switch (t)
    {
    case tex_format::rgba:
        return GL_RGBA;
    case tex_format::rgb:
        return GL_RGB;
    }
    fail_assert(invalid_enum_msg);
    return -1;
}

constexpr inline tex_format to_gl_tex_format(uint32_t t)
{
    switch (t)
    {
    case GL_RGBA:
        return tex_format::rgba;
    case GL_RGB:
        return tex_format::rgb;
    }
    fail_assert(invalid_enum_msg);
    return static_cast<tex_format>(-1);
}

#endif