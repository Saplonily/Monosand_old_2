#pragma once
#ifndef H_GL_TEXTURE2D
#define H_GL_TEXTURE2D

#include "texture2d.h"

class gl_texture2d final : public texture2d
{
    friend class gl_render_target;
private:
    static uint32_t s_cur_tex_id;
    int32_t m_width, m_height;
    tex_format m_format;
    uint32_t m_gl_id;
public:
    gl_texture2d();
    ~gl_texture2d() override;

    // gl spec
    void gl_bind() const; // hm... ok maybe it's const
    void check_and_bind() const; // i think so for this...

    // base
    void set_wrap_s_t(tex_wrap_type s, tex_wrap_type t) override;
    void set_filter(tex_filter_type min_f, tex_filter_type mag_f) override;
    void set_data(int32_t width, int32_t height, tex_format t_fmt, byte* data) override;
    int32_t width() const override;
    int32_t height() const override;
    tex_format format() const override;
};

#endif