#pragma once
#ifndef H_SHADER
#define H_SHADER

// like effects in monogame
class shader // abstract
{
public:
    virtual inline ~shader() {};
    virtual void make_current() = 0;
};

#endif