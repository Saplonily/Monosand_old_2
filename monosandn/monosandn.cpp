#include "platform.h"
#include "platform_win_glfw.h"
#include "main.h"

#include <utility>
#include <memory>

int main()
{
    std::unique_ptr<platform> uptr = std::make_unique<platform_win_glfw>();
    Main m;
    m.init(uptr.get());
    m.run();
}