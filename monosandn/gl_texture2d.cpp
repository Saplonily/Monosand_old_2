#include "gl_texture2d.h"
#include "gl_texture_enums.h"
#include <glad/glad.h>

uint32_t gl_texture2d::s_cur_tex_id = -1;

void gl_texture2d::check_and_bind() const
{
    if (s_cur_tex_id != m_gl_id)
        gl_bind();
}

void gl_texture2d::gl_bind() const
{
    s_cur_tex_id = m_gl_id;
    glBindTexture(GL_TEXTURE_2D, m_gl_id);
}

gl_texture2d::~gl_texture2d()
{
    glDeleteTextures(1, &m_gl_id);
}

gl_texture2d::gl_texture2d()
{
    glGenTextures(1, &m_gl_id);
    m_width = m_height = -1;
    m_format = static_cast<tex_format>(-1);
}

void gl_texture2d::set_data(int32_t width, int32_t height, tex_format t_fmt, byte* data)
{
    check_and_bind();
    glTexImage2D(GL_TEXTURE_2D, 0, to_gl_tex_format(t_fmt), width, height, 0, to_gl_tex_format(t_fmt), GL_UNSIGNED_BYTE, data);
    m_width = width;
    m_height = height;
    m_format = t_fmt;
}

void gl_texture2d::set_wrap_s_t(tex_wrap_type s, tex_wrap_type t)
{
    check_and_bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, to_gl_wrap_type(s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, to_gl_wrap_type(t));
}

void gl_texture2d::set_filter(tex_filter_type min_f, tex_filter_type mag_f)
{
    check_and_bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, to_gl_filter_type(min_f));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, to_gl_filter_type(mag_f));
}

int32_t gl_texture2d::width() const { return m_width; }
int32_t gl_texture2d::height() const { return m_height; }
tex_format gl_texture2d::format() const { return m_format; }