#pragma once
#ifndef H_MAIN
#define H_MAIN

#include "platform.h"

class Main final
{
private:
	platform* m_platform = nullptr;
public:
	void init(platform* platform);
	void run();
	void tick();
};

#endif