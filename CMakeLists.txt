cmake_minimum_required(VERSION 3.18)
project(InstallerCreator)
include(Common/CMakeLists.txt)
set(SKIA_INCLUDE_PATH "D:\\sdk\\skia" CACHE PATH "Skia include path,default:D:/sdk/skia" FORCE)

if(NOT IS_DIRECTORY "${SKIA_INCLUDE_PATH}")
    message(FATAL_ERROR "The path ${SKIA_INCLUDE_PATH} does not exist.")
endif()

set(SKIA_LIB_PATH "D:\\sdk\\skia\\out" CACHE PATH "Skia lib path,default:D:/sdk/skia/out" FORCE)

if(NOT IS_DIRECTORY "${SKIA_LIB_PATH}")
    message(FATAL_ERROR "The path ${SKIA_LIB_PATH} does not exist.")
endif()

set(QUICKJS_INCLUDE_PATH "D:\\sdk\\quickjs" CACHE PATH "QuickJS include path,default:D:/sdk/quickjs" FORCE)

if(NOT IS_DIRECTORY "${QUICKJS_INCLUDE_PATH}")
    message(FATAL_ERROR "The path ${QUICKJS_INCLUDE_PATH} does not exist.")
endif()

set(QUICKJS_LIB_PATH "D:\\sdk\\quickjs\\build" CACHE PATH "Skia lib path,default:D:/sdk/quickjs/build" FORCE)

if(NOT IS_DIRECTORY "${QUICKJS_LIB_PATH}")
    message(FATAL_ERROR "The path ${QUICKJS_LIB_PATH} does not exist.")
endif()

set(LIBUV_INCLUDE_PATH "D:\\sdk\\libuv\\include" CACHE PATH "QuickJS include path,default:D:/sdk/libuv/include" FORCE)

if(NOT IS_DIRECTORY "${LIBUV_INCLUDE_PATH}")
    message(FATAL_ERROR "The path ${LIBUV_INCLUDE_PATH} does not exist.")
endif()

set(LIBUV_LIB_PATH "D:\\sdk\\libuv\\build" CACHE PATH "Skia lib path,default:D:/sdk/libuv/build" FORCE)

if(NOT IS_DIRECTORY "${LIBUV_LIB_PATH}")
    message(FATAL_ERROR "The path ${LIBUV_LIB_PATH} does not exist.")
endif()

set(CMAKE_CXX_STANDARD 20)
link_directories("${SKIA_LIB_PATH};${QUICKJS_LIB_PATH};${LIBUV_LIB_PATH};")
include_directories("${SKIA_INCLUDE_PATH};${QUICKJS_INCLUDE_PATH};${LIBUV_INCLUDE_PATH};")

add_subdirectory(Common)
add_subdirectory(Installer)
add_subdirectory(InstallerCreator)
add_subdirectory(Uninstaller)
