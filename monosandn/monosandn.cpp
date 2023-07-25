#include "platform_win_glfw.h"
#include "gl_texture2d.h"
#include "main.h"
#include "gl_render_target.h"

#include <iostream>
#include <memory>
#include <stb_image.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

class custom_main : public main
{
private:
    std::unique_ptr<platform> pf;
    std::unique_ptr<texture2d> tex;
    std::unique_ptr<render_target> rtg;
    float num;
public:

    inline void init()
    {
        pf = std::make_unique<platform_win_glfw>();
        main::init(pf.get());

        tex = std::make_unique<gl_texture2d>();
        tex->init();
        int width, height, channels;
        stbi_uc* img = stbi_load("pic.png", &width, &height, &channels, 0);
        tex->set_data(width, height, channels == 3 ? tex_format::rgb : channels == 4 ? tex_format::rgba : (tex_format)-1, img);
        stbi_image_free(img);

        rtg = std::make_unique<gl_render_target>();
        rtg->init();
        gl_texture2d* gt = new gl_texture2d;
        gt->init();
        gt->set_data(800, 600, tex_format::rgba, nullptr);
        rtg->set_attched_texture(gt);


    }

    inline void update(float delta) override
    {
        num += delta;
        auto* ppf = platform_win_glfw::get_singleton();
        //pf->pf_printfn("delta: %f", delta);



        //std::cout << ppf->window_width() << " " << ppf->window_height() << std::endl;

    };

    inline void render() override
    {

        glm::vec2 position(0.0f, 0.0f);

        rtg->set_target(true);
        {
            rtg->clear(glm::vec4(sinf(num) / 2.0f + 0.5f, cosf(num) / 2.0f + 0.5f, 1.0f, 1.0f));
            glm::vec2 position(0.0f, 0.0f);
            pf->draw_texture(*tex, position + glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2{0.0f}, glm::vec2(1.0f, 1.0f));
        }
        rtg->set_target(false);

        pf->fill_color(0.0f, 0.0f, 0.0f);
        //pf->draw_texture(*tex, position, 0.0f, glm::vec2(0.0f, 0.0f));
        pf->draw_texture(*rtg->texture(), glm::vec2(0.0f, rtg->texture()->height()), 0.0f, glm::vec2{0.0f}, glm::vec2(1.0f, -1.0f));

    };
};

int main()
{
    custom_main cm;
    cm.init();
    cm.run();
}