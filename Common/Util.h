#pragma once
extern "C" {
#include <quickjs.h>
}
class Util
{
public:
#ifdef DEBUG
	static void InitDebuger();
#endif
};

