#include "platform_win_glfw.h"
#include "gl_texture2d.h"
#include "main.h"
#include "gl_render_target.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <stb_image.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/filesystem.hpp>
#include <ShlObj.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class custom_main : public main
{
private:
    std::unique_ptr<platform> pf;
    std::unique_ptr<texture2d> tex;
    std::unique_ptr<texture2d> charTex;
    std::unique_ptr<render_target> rtg;
    float num = 0.0f;
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

        {
            namespace bfs = boost::filesystem;
            bfs::path font_file;
            PWSTR str;
            SHGetKnownFolderPath(FOLDERID_Fonts, 0, 0, &str);
            font_file /= str;
            CoTaskMemFree(str);
            font_file /= L"msyhl.ttc";
            uintmax_t size = bfs::file_size(font_file);
            byte* buffer = new byte[size]{ 0 };

            std::ifstream is(font_file.c_str(), std::ios::binary);
            is.read((char*)buffer, size);
            is.close();

            FT_Library ft;
            FT_Init_FreeType(&ft);

            FT_Face face;
            FT_New_Memory_Face(ft, buffer, static_cast<FT_Long>(size), 0, &face);
            FT_Set_Pixel_Sizes(face, 0, 59);
            FT_Select_Charmap(face, FT_ENCODING_UNICODE);
            FT_Load_Char(face, L'好', FT_LOAD_RENDER);
            printf("buffer size: %lld", size);
            delete[] buffer;

            charTex = std::make_unique<gl_texture2d>();
            FT_Bitmap& map = face->glyph->bitmap;
            charTex->init();
            charTex->set_data(map.width, map.rows, tex_format::red, map.buffer);
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }
    }

    inline void update(float delta) override
    {
        num += delta;
        auto* ppf = platform_win_glfw::get_singleton();

    };

    inline void render() override
    {
        glm::vec2 position(0.0f, 0.0f);

        rtg->set_target(true);
        {
            rtg->clear(glm::vec4(sinf(num) / 2.0f + 0.5f, cosf(num) / 2.0f + 0.5f, 1.0f, 0.1f));
            glm::vec2 position(0.0f, 0.0f);
            pf->draw_texture(*tex, position + glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2{0.0f}, glm::vec2(1.0f, 1.0f));
        }
        rtg->set_target(false);

        pf->fill_color(0.8f, 1.0f, 1.0f);
        //pf->draw_texture(*rtg->texture(), glm::vec2(0.0f, rtg->texture()->height()), 0.0f, glm::vec2{0.0f}, glm::vec2(1.0f, -1.0f));

        pf->draw_texture(*charTex, glm::vec2(100.0f, 0.0f));
    };
};

int main()
{
    custom_main cm;
    cm.init();
    cm.run();
}