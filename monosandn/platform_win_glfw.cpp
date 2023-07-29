#include "platform_win_glfw.h"
#include "gl_texture2d.h"

#include <fstream>

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <codecvt>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/filesystem.hpp>

#undef APIENTRY
#include <Windows.h>
#include <ShlObj.h>
#pragma comment(lib, "winmm")

using pwg = platform_win_glfw;

pwg* platform_win_glfw::get_singleton()
{
    platform* pf = pwg::singleton;
    assert(dynamic_cast<platform_win_glfw*>(pf) != nullptr);
    return (static_cast<platform_win_glfw*>(pf));
}

static void on_glfw_error(int code, const char* msg)
{
    pwg* pf = pwg::get_singleton();
    pf->pf_printfn("=== glfw error %d ===", code);
    pf->pf_printfn(msg);
}

static void on_opengl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    pwg* pf = pwg::get_singleton();
#define make_case(eq, c) case c: eq = #c;break;
    const char* sourceMsg;
    switch (source)
    {
        make_case(sourceMsg, GL_DEBUG_SOURCE_API_ARB);
        make_case(sourceMsg, GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB);
        make_case(sourceMsg, GL_DEBUG_SOURCE_SHADER_COMPILER_ARB);
        make_case(sourceMsg, GL_DEBUG_SOURCE_THIRD_PARTY_ARB);
        make_case(sourceMsg, GL_DEBUG_SOURCE_APPLICATION_ARB);
        make_case(sourceMsg, GL_DEBUG_SOURCE_OTHER_ARB);
    default: sourceMsg = "Unknown.";
    }
    const char* typeMsg;
    switch (type)
    {
        make_case(typeMsg, GL_DEBUG_TYPE_ERROR_ARB);
        make_case(typeMsg, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB);
        make_case(typeMsg, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB);
        make_case(typeMsg, GL_DEBUG_TYPE_PORTABILITY_ARB);
        make_case(typeMsg, GL_DEBUG_TYPE_PERFORMANCE_ARB);
        make_case(typeMsg, GL_DEBUG_TYPE_OTHER_ARB);
    default: typeMsg = "Unknown.";
    }
    const char* severityMsg;
    switch (severity)
    {
        make_case(severityMsg, GL_DEBUG_SEVERITY_HIGH_ARB);
        make_case(severityMsg, GL_DEBUG_SEVERITY_MEDIUM_ARB);
        make_case(severityMsg, GL_DEBUG_SEVERITY_LOW_ARB);
    default: severityMsg = "Unknown.";
    }
    pf->pf_printfn("=== opengl error ===");
    pf->pf_printfn("msg: %s", message);
    pf->pf_printfn("source: %s", sourceMsg);
    pf->pf_printfn("type: %s", typeMsg);
    pf->pf_printfn("severity: %s", severityMsg);
    DebugBreak();
#undef make_case
}

void platform_win_glfw::on_framebuffer_size(int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
    glm::mat3 trans{1.0f};
    trans = glm::translate(trans, glm::vec2(-1.0f, 1.0f));
    trans = glm::scale(trans, glm::vec2(2.0f / width, -2.0f / height));
    glUniformMatrix3fv(m_gl_rect.shd_view_matpos, 1, GL_FALSE, glm::value_ptr(trans));
}

static void on_glfw_window_size(GLFWwindow* win, int32_t width, int32_t height)
{
    pwg* pf = pwg::get_singleton();
    pf->m_wWidth = width;
    pf->m_wHeight = height;
    if (pf->window_resize_callback)
        pf->window_resize_callback(width, height);
}

static void on_glfw_framebuffer_size(GLFWwindow* win, int32_t width, int32_t height)
{
    pwg::get_singleton()->on_framebuffer_size(width, height);
}

void platform_win_glfw::create_window(int32_t width, int32_t height, const std::wstring& title)
{
    assert(glfw_win != nullptr);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
    const std::string& str = cvt.to_bytes(title);
    glfwSetWindowSize(glfw_win, width, height);
    glfwSetWindowTitle(glfw_win, str.c_str());
    glfwShowWindow(glfw_win);
    on_glfw_framebuffer_size(glfw_win, width, height);
}

void platform_win_glfw::init()
{
    // windows init
    SetConsoleOutputCP(CP_UTF8);
    timeBeginPeriod(1);
    QueryPerformanceFrequency((LARGE_INTEGER*)&ticks_per_seconds);



#pragma region glfw&glad init
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    glfwSetErrorCallback(on_glfw_error);
    char c = '\0';
    glfw_win = glfwCreateWindow(1, 1, &c, nullptr, nullptr);
    glfwMakeContextCurrent(glfw_win);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetWindowSizeCallback(glfw_win, on_glfw_window_size);
    glfwSetFramebufferSizeCallback(glfw_win, on_glfw_framebuffer_size);
#pragma endregion

#pragma region opengl init
    glDebugMessageCallbackARB(on_opengl_error, nullptr);
    // texture rect vao,vbo,shd etc.
    glGenVertexArrays(1, &m_gl_rect.vao_id);
    glGenBuffers(1, &m_gl_rect.vbo_id);
    glBindVertexArray(m_gl_rect.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_rect.vbo_id);
    glActiveTexture(GL_TEXTURE0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(m_gl_rect.vertices), m_gl_rect.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    const char* vshSource = R"(#version 330 core
layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 v_tex_coord;

uniform mat3 u_view;
uniform mat3 u_model;
out vec2 f_tex_coord;

void main()
{
    vec3 v_pos2 = u_view * u_model * vec3(v_pos, 1.0);
    gl_Position = vec4(v_pos2, 1.0);
    f_tex_coord = v_tex_coord;
})";
    const char* fshSource = R"(#version 330 core
out vec4 FragColor;

in vec2 f_tex_coord;

uniform sampler2D tex;
uniform vec4 color;

void main()
{
    FragColor = texture(tex, f_tex_coord) * color;
})";

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vshSource, NULL);
    glShaderSource(fs, 1, &fshSource, NULL);
    glCompileShader(vs);
    glCompileShader(fs);

    // TODO: prevent buffer from overflowing
    const size_t buffer_size = 1024;
    int success;
    char infoLog[buffer_size];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, buffer_size, NULL, infoLog);
        pf_printfn("=== vertex shader compilation failed ===\n%s", infoLog);
    }
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, buffer_size, NULL, infoLog);
        pf_printfn("=== fragment shader compilation failed ===\n%s", infoLog);
    }

    uint32_t pg = glCreateProgram();
    glAttachShader(pg, vs);
    glAttachShader(pg, fs);
    glLinkProgram(pg);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glGetProgramiv(pg, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(pg, buffer_size, NULL, infoLog);
        pf_printfn("=== shader program linking failed ===\n%s", infoLog);
    }

    glUseProgram(pg);
    m_gl_rect.shd_id = pg;

    m_gl_rect.shd_view_matpos = glGetUniformLocation(m_gl_rect.shd_id, "u_view");
    m_gl_rect.shd_model_matpos = glGetUniformLocation(m_gl_rect.shd_id, "u_model");
    m_gl_rect.shd_color_pos = glGetUniformLocation(m_gl_rect.shd_id, "color");
    glUniform4f(m_gl_rect.shd_color_pos, 1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#pragma endregion

#pragma region default font init
    

#pragma endregion

#pragma region ImGui init

#pragma endregion
}

void platform_win_glfw::draw_texture(const texture2d& tex, const glm::mat3& trans)
{
    assert(dynamic_cast<const gl_texture2d*>(&tex) != nullptr);
    const gl_texture2d* gl_tex = static_cast<const gl_texture2d*>(&tex);

    gl_tex->check_and_bind();
    glUseProgram(m_gl_rect.shd_id);
    int width = tex.width();
    int height = tex.height();

    glm::mat3 mat{1.0f};
    mat = mat * trans;
    mat = glm::scale(mat, glm::vec2(width, height));

    glUniformMatrix3fv(m_gl_rect.shd_model_matpos, 1, GL_FALSE, glm::value_ptr(mat));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void platform_win_glfw::fill_color(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void platform_win_glfw::sleep_us(int64_t us)
{
    DWORD t = static_cast<DWORD>(us / 1000.0);
    Sleep(us <= 1000 ? 1 : t);
}

int64_t platform_win_glfw::get_ticks_persecond() const { return ticks_per_seconds; }
int64_t platform_win_glfw::get_abs_ticks() const { int64_t tick; QueryPerformanceCounter((LARGE_INTEGER*)&tick); return tick; }

int32_t platform_win_glfw::get_expected_fps() const { return expected_fps; }
void platform_win_glfw::set_expected_fps(int32_t fps) { expected_fps = fps; }

void platform_win_glfw::set_window_resize_callback(const window_resize_callback_t cb) { window_resize_callback = cb; }
void platform_win_glfw::pool_events() { glfwPollEvents(); }
void platform_win_glfw::swap_buffers() { glfwSwapBuffers(glfw_win); }
bool platform_win_glfw::need_shutdown() const { return glfwWindowShouldClose(glfw_win); }
int32_t platform_win_glfw::window_width() const { return m_wWidth; }
int32_t platform_win_glfw::window_height() const { return m_wHeight; }
double platform_win_glfw::get_time() const { return glfwGetTime(); }
