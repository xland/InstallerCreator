# 编译QuickJS
- clone QuickJS的代码：https://github.com/quickjs-ng/quickjs
- 配置CMAKE（静态连接），生成VS工程
- 修改MT/MTD 再编译项目
- 修改quickjs.h文件(不然无法在C++工程中使用这个头文件)
	- 482行：return { .u{.int32 = val }, .tag{JS_TAG_BOOL} };
	- 488行：return { .u{.int32 = val }, .tag{JS_TAG_INT} };
	- 499行： return { .u{.int32 = val }, .tag{JS_TAG_CATCH_OFFSET} };

# 编译Skia

- 可能要更新third_party/external下的库
- 可能要禁用icu:skia_use_icu=false
```
bin/gn gen out/debug --args='clang_win=\"C:\Program Files\LLVM\" cc=\"clang\" cxx=\"clang++\" extra_cflags=[\"/MTd\"] is_official_build=true is_debug=false skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_harfbuzz=false skia_use_system_icu=true skia_enable_skparagraph=true skia_enable_skshaper=true skia_enable_skunicode=false skia_use_icu=false'
```
```
bin/gn gen out/release --args='clang_win=\"C:\Program Files\LLVM\" cc=\"clang\" cxx=\"clang++\" extra_cflags=[\"/MT\"] is_debug=false is_official_build=true skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_harfbuzz=false skia_use_icu=false skia_enable_skparagraph=true skia_enable_skshaper=true skia_enable_skunicode=true'
```