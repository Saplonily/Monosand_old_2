#pragma once
#ifndef H_MAIN
#define H_MAIN

#include "platform.h"
#include <functional>

class main
{
private:
    platform* m_platform = nullptr;
    void tick();
public:
    void init(platform* platform);
    void run();
    virtual inline void update() {};
    virtual inline void render() {};
};

#endif