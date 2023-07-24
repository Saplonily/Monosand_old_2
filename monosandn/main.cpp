#include "main.h"

void main::init(platform* platform)
{
    m_platform = platform;
    m_platform->init();
    m_platform->create_window(1200, 800, L"这里是临时的标题...");
}

void main::run()
{
    float last_delta = 1.0f / m_platform->get_expected_fps();
    while (!m_platform->need_shutdown())
    {
        int64_t before = m_platform->get_abs_ticks();
        tick(last_delta);
        int64_t after = m_platform->get_abs_ticks();
        int64_t expected_ticks_per_frame = m_platform->get_ticks_persecond() / m_platform->get_expected_fps();
        int64_t actual_ticks_this_frame = after - before;
        int64_t ticks_to_sleep = expected_ticks_per_frame - actual_ticks_this_frame;
        last_delta = 1.0f / m_platform->get_expected_fps();
        if (ticks_to_sleep > 0)
            m_platform->sleep_us(ticks_to_sleep * 1000000 / m_platform->get_ticks_persecond());
        else
            last_delta += (float)(-ticks_to_sleep) / m_platform->get_ticks_persecond();
    }
}

void main::tick(float delta)
{
    m_platform->pool_events();
    update(delta);
    render();
    m_platform->swap_buffers();
}