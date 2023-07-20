#pragma once
#ifndef H_MAIN
#define H_MAIN

#include "platform.h"
#include <functional>

class Main final
{
private:
    platform* m_platform = nullptr;
public:
    void init(platform* platform);
    void run(const std::function<void()>& between);
    void tick(const std::function<void()>& between);
};

#endif