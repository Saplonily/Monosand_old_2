#include "platform.h"
#include "platform_win_glfw.h"
#include "gl_texture2d.h"
#include "main.h"

#include <utility>
#include <memory>
#include <stb_image.h>
#include <glm/gtx/matrix_transform_2d.hpp>

int main()
{
    std::unique_ptr<platform> pf = std::make_unique<platform_win_glfw>();
    Main m;
    m.init(pf.get());

    std::unique_ptr<texture2d> tex = std::unique_ptr<texture2d>(new gl_texture2d());
    tex->init();
    int width, height, channels;
    stbi_uc* img = stbi_load("pic.png", &width, &height, &channels, 0);
    tex->set_data(width, height, channels == 3 ? tex_format::rgb : channels == 4 ? tex_format::rgba : (tex_format)-1, img);
    stbi_image_free(img);
    std::function<void()> update = [&]()
    {
        glm::vec2 position(10.0f, 10.0f);
        float rotation = static_cast<float>(pf->get_time());

        pf->draw_texture_ext(*tex, position, rotation, glm::vec2(0.5f, 0.5f));
    };
    m.run(update);
}