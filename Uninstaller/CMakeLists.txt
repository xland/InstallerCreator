set(SRC
    main.cpp
)
add_executable(Uninstaller ${SRC} ${COMMON})
target_include_directories(Uninstaller PRIVATE ../Common)