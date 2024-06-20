#include "FontIcon.h"
namespace {
	static JSClassID id;
	static JSClassDef inputClass = {
		.class_name{"FontIcon"},
		.finalizer{[](JSRuntime* rt, JSValue val) {
				auto fontIcon = (FontIcon*)JS_GetOpaque(val, id);
				delete fontIcon;
			}
		}
	};
}

FontIcon::FontIcon()
{
}

FontIcon::~FontIcon()
{
}