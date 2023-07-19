#include "texture2d.h"

void texture2d::init()
{
    set_filter(tex_filter_type::linear, tex_filter_type::linear);
    set_wrap_s_t(tex_wrap_type::clamp_to_edge, tex_wrap_type::clamp_to_edge);
}