set(SRC
    main.cpp
)
add_executable(InstallerCreator ${SRC} ${COMMON})
target_include_directories(InstallerCreator PRIVATE ../Common)