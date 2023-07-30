#include "gl_render_target.h"
#include "gl_texture2d.h"
#include "platform_win_glfw.h"
#include <glad/glad.h>
#include <assert.h>
#include <glm/gtc/type_ptr.hpp>

uint32_t gl_render_target::s_cur_rt_id = 0;

void gl_render_target::set_attched_texture(const texture2d* tex)
{
    assert(dynamic_cast<const gl_texture2d*>(tex) != nullptr);
    const gl_texture2d* gl_tex = static_cast<const gl_texture2d*>(tex);
    m_tex = gl_tex;
    glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_tex->m_gl_id, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, s_cur_rt_id);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

gl_render_target::~gl_render_target()
{
    glDeleteFramebuffers(1, &m_gl_id);
    delete m_tex;
}

gl_render_target::gl_render_target()
{
    glGenFramebuffers(1, &m_gl_id);
    m_tex = nullptr;
}

void gl_render_target::set_with(const gl_render_target* target)
{
    platform_win_glfw* pf = platform_win_glfw::get_singleton();
    uint32_t id = target == nullptr ? 0 : target->m_gl_id;
    if (s_cur_rt_id == id) return;
    s_cur_rt_id = id;

    int32_t tw = target == nullptr ? pf->window_width() : target->texture()->width();
    int32_t th = target == nullptr ? pf->window_height() : target->texture()->height();

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    pf->on_framebuffer_size(tw, th);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void gl_render_target::make_current() const { set_with(this); }

void gl_render_target::reset_target() { set_with(nullptr); }

void gl_render_target::clear(glm::vec4 color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, s_cur_rt_id);
}

const texture2d* gl_render_target::texture() const { return m_tex; }