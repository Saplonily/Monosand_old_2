#include "platform.h"
#include <glm/gtx/matrix_transform_2d.hpp>

void platform::draw_texture(const texture2d& tex, glm::vec2 pos, float rotation, glm::vec2 scale)
{
    glm::mat3 trans{1.0f};
    trans = glm::translate(trans, pos);
    trans = glm::scale(trans, scale);
    trans = glm::rotate(trans, rotation);
    draw_texture(tex, trans);
}

void platform::draw_texture_ext(const texture2d& tex, glm::vec2 pos, float rotation, glm::vec2 origin, glm::vec2 scale)
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