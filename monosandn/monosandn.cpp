#include "platform_win_glfw.h"
#include "gl_texture2d.h"
#include "main.h"

#include <memory>
#include <stb_image.h>
#include <glm/gtx/matrix_transform_2d.hpp>

class custom_main : public main
{
private:
    std::unique_ptr<platform> pf;
    std::unique_ptr<texture2d> tex;
public:

    void init()
    {
        pf = std::make_unique<platform_win_glfw>();
        main::init(pf.get());

        tex = std::unique_ptr<texture2d>(new gl_texture2d());
        tex->init();
        int width, height, channels;
        stbi_uc* img = stbi_load("pic.png", &width, &height, &channels, 0);
        tex->set_data(width, height, channels == 3 ? tex_format::rgb : channels == 4 ? tex_format::rgba : (tex_format)-1, img);
        stbi_image_free(img);
    }

    virtual inline void update() override
    {

    };

    virtual inline void render() override
    {
        pf->fill_color(0.8f, 1.0f, 1.0f);

        glm::vec2 position(100.0f, 100.0f);
        float rotation = static_cast<float>(pf->get_time());

        pf->draw_texture(*tex, position, rotation, glm::vec2(0.5f, 0.5f));
    };
};

int main()
{
    custom_main cm;
    cm.init();
    cm.run();
}