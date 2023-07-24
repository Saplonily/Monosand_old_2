#pragma once
#ifndef H_MAIN
#define H_MAIN

#include "platform.h"
#include <functional>

class main
{
private:
    platform* m_platform = nullptr;
    void tick(float delta);
public:
    void init(platform* platform);
    void run();
    virtual inline void update(float delta) {};
    virtual inline void render() {};
};

#endif