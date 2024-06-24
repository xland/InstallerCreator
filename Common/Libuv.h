#pragma once


class Libuv
{
public:
    Libuv();
    ~Libuv();
    static void Init();
private:
    static void UVLoop();

};


