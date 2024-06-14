#include "Path.h"

class Shadow:public Path
{
public:
	~Shadow();
	void Paint(Win* win) override;
	static void Reg(JSContext* ctx);
protected:
	Shadow();
private:
	uint32_t ambientColor;
	uint32_t spotColor;
	static JSValue constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv);
};
