if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(SRC
    main.cpp
    main.js
)
add_executable(Installer ${SRC} ${COMMON})

target_include_directories(Installer PRIVATE ../Common)
set_property(TARGET Installer PROPERTY VS_DPI_AWARE "PerMonitor")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set_property(TARGET Installer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(Installer PRIVATE $<$<CONFIG:Debug>:UNICODE DEBUG NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_DEBUG WIN32 WINDOWS>)
target_compile_definitions(Installer PRIVATE $<$<CONFIG:Release>:UNICODE NDEBUG NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_RELEASE WIN32 WINDOWS>)
TARGET_LINK_LIBRARIES(Installer skia libuv qjs dwmapi winmm kernel32 user32 shell32 imm32 gdi32 usp10 ws2_32 dbghelp userenv iphlpapi)
