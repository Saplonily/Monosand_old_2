#include "main.h"

void main::init(platform* platform)
{
    m_platform = platform;
    m_platform->init();
    m_platform->create_window(1200, 800, L"这里是临时的标题...");
}

void main::run()
{
    while (!m_platform->need_shutdown())
    {
        tick();

        // for now, sleep a period
        m_platform->sleep_us(static_cast<int64_t>(1000.0 * 1000.0 / 60.0));
    }
}

void main::tick()
{
    m_platform->pool_events();
    update();
    render();
    m_platform->swap_buffers();
}