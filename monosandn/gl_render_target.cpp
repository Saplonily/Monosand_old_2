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

void gl_render_target::set_target(bool to_self)
{
    uint32_t id = to_self ? m_gl_id : 0;
    if (s_cur_rt_id == id) return;
    s_cur_rt_id = id;
    platform_win_glfw* pf = platform_win_glfw::get_singleton();
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (id == 0)
        pf->on_framebuffer_size(pf->window_width(), pf->window_height());
    else
        pf->on_framebuffer_size(m_tex->width(), m_tex->height());
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void gl_render_target::clear(glm::vec4 color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, s_cur_rt_id);
}

const texture2d* gl_render_target::texture() { return m_tex; }