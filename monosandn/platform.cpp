#include "platform.h"
#include <cstdarg>
#include <glm/gtx/matrix_transform_2d.hpp>

platform* platform::singleton = nullptr;

platform::platform()
{
    assert(singleton == nullptr);
    singleton = this;
}

platform::~platform()
{
    if (singleton == this)
        singleton = nullptr;
}

void platform::pf_printfn(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vprintf(fmt, va);
    va_end(va);
    putchar('\n');
}

void platform::pf_wprintfn(const wchar_t* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vwprintf(fmt, va);
    va_end(va);
    putchar('\n');
}

void platform::draw_texture(const texture2d& tex, glm::vec2 pos)
{
    glm::mat3 trans{1.0f};
    trans = glm::translate(trans, pos);
    draw_texture(tex, trans);
}

void platform::draw_texture(const texture2d& tex, glm::vec2 pos, float rotation, glm::vec2 origin, glm::vec2 scale)
{
    glm::mat3 trans{1.0f};
    glm::vec2 origin_translate = glm::vec2(tex.width(), tex.height()) * origin;
    trans = glm::translate(trans, pos);
    trans = glm::scale(trans, scale);
    trans = glm::translate(trans, origin_translate);
    trans = glm::rotate(trans, rotation);
    trans = glm::translate(trans, -origin_translate);
    draw_texture(tex, trans);
}

void platform::set_render_target(const render_target& rt)
{
    rt.make_current();
}