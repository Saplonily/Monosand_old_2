#pragma once
#ifndef H_PLATFORM_WIN_GLFW
#define H_PLATFORM_WIN_GLFW

#include "platform.h"
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#undef APIENTRY
#include <GLFW/glfw3native.h>

#define vertices_data {    \
/*位置*/     /*纹理*/       \
0.0f, 1.0f, 0.0f, 1.0f,    \
1.0f, 0.0f, 1.0f, 0.0f,    \
0.0f, 0.0f, 0.0f, 0.0f,    \
                           \
0.0f, 1.0f, 0.0f, 1.0f,    \
1.0f, 1.0f, 1.0f, 1.0f,    \
1.0f, 0.0f, 1.0f, 0.0f,    \
}

class platform_win_glfw final : public platform
{
private:
    GLFWwindow* glfw_win = nullptr;
    int32_t m_wWidth = -1, m_wHeight = -1;
    int64_t ticks_per_seconds = -1;
    int32_t expected_fps = 60;
    window_resize_callback_t window_resize_callback;

    struct {
        uint32_t vao_id = -1;
        uint32_t vbo_id = -1;
        uint32_t shd_id = -1;

        int32_t shd_model_matpos = -1;
        int32_t shd_view_matpos = -1;
        int32_t shd_color_pos = -1;
        float vertices[4 * 6] = vertices_data;
    } m_gl_rect;

    friend void on_glfw_window_size(GLFWwindow*, int32_t, int32_t);
    friend void on_glfw_framebuffer_size(GLFWwindow*, int32_t, int32_t);
    friend class custom_main;
public:
    void create_window(int32_t width, int32_t height, const std::wstring& title) override;
    void init() override;
    bool need_shutdown() const override;
    void pool_events() override;
    void swap_buffers() override;
    void sleep_us(int64_t us) override;
    int64_t get_ticks_persecond() const override;
    int64_t get_abs_ticks() const override;
    double get_time() const override;

    int32_t get_expected_fps() const override;
    void set_expected_fps(int32_t fps) override;

    // display api
    int32_t window_width() const override;
    int32_t window_height() const override;
    void set_window_resize_callback(const window_resize_callback_t cb) override;

    // render api
    void draw_texture(const texture2d& tex, const glm::mat3& trans) override;
    void fill_color(float r, float g, float b) override;

    void on_framebuffer_size(int32_t width, int32_t height);

    static platform_win_glfw* get_singleton();
};

#undef vertices_data
#endif