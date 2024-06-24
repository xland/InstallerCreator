#include "Libuv.h"
#include <thread>
#include <iostream>
#include "uv.h"

namespace {
    uv_async_t* async_handle;
}

Libuv::Libuv()
{
}

Libuv::~Libuv()
{
}

void Libuv::Init()
{
    std::thread libuvThread(Libuv::UVLoop);
    libuvThread.detach();
    uv_async_send(async_handle);
}

void Libuv::UVLoop()
{
    uv_loop_t* loop = uv_default_loop();
    uv_async_init(loop, async_handle, [](uv_async_t* handle) {
        // 当主线程发送信号时，此回调将被调用
        std::cout << "event" << std::endl;
    });
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    free(loop);
}
