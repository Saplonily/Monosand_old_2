#pragma once
#ifndef H_PLATFORM_WIN_GLFW
#define H_PLATFORM_WIN_GLFW

#include "platform.h"
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#undef APIENTRY
#include <GLFW/glfw3native.h>

class platform_win_glfw final : public platform
{
private:
    GLFWwindow* glfw_win = nullptr;
    int32_t m_wWidth = -1, m_wHeight = -1;

    struct {
        uint32_t vaoId = -1;
        uint32_t vboId = -1;
        uint32_t shdId = -1;
        int32_t shd_model_matpos = -1;
        int32_t shd_view_matpos = -1;
        float vertices[24] = {
            // 位置     // 纹理
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };
        glm::mat3 view_mat{};
    } m_gl_rect;
public:
    void* create_window(int32_t width, int32_t height, const std::wstring& title) override;
    void init() override;
    bool need_shutdown() const override;
    void pool_events() override;
    void swap_buffers() override;
    void sleep_us(int64_t us) override;

    void draw_texture(const texture2d& tex, const glm::mat3& trans) override;
    void fill_color(float r, float g, float b) override;

    int32_t window_width() const override;
    int32_t window_height() const override;
    double get_time() const override;
};

#endif