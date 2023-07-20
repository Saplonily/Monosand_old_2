#include "main.h"

void Main::init(platform* platform)
{
    m_platform = platform;
    m_platform->init();
    m_platform->create_window(1200, 800, L"这里是临时的标题...");
}

void Main::run(const std::function<void()>& between)
{
    while (!m_platform->need_shutdown())
    {
        tick(between);
        // for now sleep a period
        m_platform->sleep_us(static_cast<int64_t>(1000.0 * 1000.0 / 60.0));
    }
}

void Main::tick(const std::function<void()>& between)
{
    m_platform->pool_events();

    // do something...

    m_platform->fill_color(0.8f, 1.0f, 1.0f);
    between();

    // do something...

    m_platform->swap_buffers();
}