#pragma once
#ifndef H_GL_RENDER_TARGET
#define H_GL_RENDER_TARGET

#include "render_target.h"
#include "gl_texture2d.h"

class gl_render_target final : public render_target
{
private:
    static uint32_t s_cur_rt_id;
    const gl_texture2d* m_tex;
    uint32_t m_gl_id;
    static void set_with(const gl_render_target* target);
public:
    gl_render_target();
    ~gl_render_target();
    static void reset_target();

    void set_attched_texture(const texture2d* tex) override;
    void make_current() const override;
    const texture2d* texture() const override;
    void clear(glm::vec4 color) override;
};

#endif