#pragma once

#include "FrameDefine.h"

// 仅可用于linux,需要将编译选项修改为
//set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -g2 -rdynamic -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-omit-frame-pointer")
// 也就是依赖于-g2 -rdynamic -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-omit-frame-pointer这些参数
class MICRO_LEGEND_FRAME_API CrashHandler
{
public:
    static void init();
protected:
#ifdef LINUX
    static int on_backtrace(void* data, uintptr_t pc, const char* filename, int lineno, const char* function);
    static void on_backtrace_error(void* data, const char* msg, int errnum);
    static void crash_handler(int sig, siginfo_t* info, void* ucontext);
    static void safe_write(int fd, const char* str);
    static struct backtrace_state* bt_state;
#endif
};