#pragma once
#ifndef H_RENDER_TARGET
#define H_RENDER_TARGET

#include "texture2d.h"
#include <glm/vec4.hpp>

class render_target
{
public:
    virtual inline ~render_target() {}
    virtual inline void init() {};

    virtual void set_attched_texture(const texture2d* tex) = 0;
    virtual void make_current() const = 0;

    virtual const texture2d* texture() const = 0;
    virtual void clear(glm::vec4 color) = 0;
};

#endif