#pragma once
#ifndef H_ERR_MACROS
#define H_ERR_MACROS

#include <assert.h>

#define fail_assert(msg) assert(false && msg)

#endif