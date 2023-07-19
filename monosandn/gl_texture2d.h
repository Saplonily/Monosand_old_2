#pragma once
#ifndef H_GL_TEXTURE2D
#define H_GL_TEXTURE2D

#include "texture2d.h"

class gl_texture2d final: public texture2d
{
private:
    uint32_t m_gl_id;
    static uint32_t s_cur_tex_id;
public:
    gl_texture2d();

    // gl spec
    void gl_bind();
    void check_and_bind();

    // base
    virtual void set_wrap_s_t(tex_wrap_type s, tex_wrap_type t) override;
    virtual void set_filter(tex_filter_type min_f, tex_filter_type mag_f) override;
    virtual void set_data(int32_t width, int32_t height, tex_format t_fmt,byte* data) override;

};

#endif