#include "platform_win_glfw.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <codecvt>
#undef APIENTRY
#include <Windows.h>

#pragma comment(lib, "winmm")

void* platform_win_glfw::create_window(int32_t width, int32_t height, const std::wstring& title)
{
    assert(glfw_win != nullptr);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
    const std::string& str = cvt.to_bytes(title);
    glfwSetWindowSize(glfw_win, width, height);
    glfwSetWindowTitle(glfw_win, str.c_str());
    glfwShowWindow(glfw_win);
    m_wWidth = width;
    m_wHeight = height;
    glViewport(0, 0, width, height);
    glm::mat3 trans{1.0f};
    trans = glm::translate(trans, glm::vec2(-1.0f, 1.0f));
    trans = glm::scale(trans, glm::vec2(2.0f / width, -2.0f / height));
    m_gl_rect.view_mat = trans;

    int32_t loc = glGetUniformLocation(m_gl_rect.shdId, "view");
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(trans));

    m_gl_rect.shd_model_matpos = glGetUniformLocation(m_gl_rect.shdId, "model");
    m_gl_rect.shd_view_matpos = loc;

    return glfw_win;
}

void platform_win_glfw::init()
{
    // windows init
    SetConsoleOutputCP(CP_UTF8);
    timeBeginPeriod(1);

    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwSetErrorCallback([](int code, const char* msg)
        {
            printf("你踏马error了!, code: %d, msg: %s\n", code, msg);
        });
    char c = '\0';
    glfw_win = glfwCreateWindow(1, 1, &c, nullptr, nullptr);
    glfwMakeContextCurrent(glfw_win);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    // gl init
    glGenVertexArrays(1, &m_gl_rect.vaoId);
    glGenBuffers(1, &m_gl_rect.vboId);
    glBindVertexArray(m_gl_rect.vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_rect.vboId);
    glActiveTexture(GL_TEXTURE0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(m_gl_rect.vertices), m_gl_rect.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    const char* vshdSource = R"(#version 330 core
layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 v_tex_coord;

uniform mat3 view;
uniform mat3 model;
out vec2 f_tex_coord;

void main()
{
    vec3 v_pos2 = view * model * vec3(v_pos, 1.0);
    gl_Position = vec4(v_pos2, 1.0);
    f_tex_coord = v_tex_coord;
})";
    const char* fshdSource = R"(#version 330 core
out vec4 FragColor;

in vec2 f_tex_coord;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, f_tex_coord);
})";

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vshdSource, NULL);
    glShaderSource(fs, 1, &fshdSource, NULL);
    glCompileShader(vs);
    glCompileShader(fs);

    int  success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    uint32_t program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(program);
    m_gl_rect.shdId = program;
}

void platform_win_glfw::draw_texture(const texture2d& tex, const glm::mat3& trans)
{
    glUseProgram(m_gl_rect.shdId);
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

void platform_win_glfw::pool_events() { glfwPollEvents(); }

void platform_win_glfw::swap_buffers() { glfwSwapBuffers(glfw_win); }

bool platform_win_glfw::need_shutdown() const { return glfwWindowShouldClose(glfw_win); }

int32_t platform_win_glfw::window_width() const { return m_wWidth; }
int32_t platform_win_glfw::window_height() const { return m_wHeight; }
double platform_win_glfw::get_time() const { return glfwGetTime(); }

void platform_win_glfw::sleep_us(int64_t us)
{
    DWORD t = static_cast<DWORD>(us / 1000.0);
    Sleep(us <= 1000 ? 1 : t);
}