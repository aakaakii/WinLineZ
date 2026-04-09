# ==============================================================================
# CMakeLists.txt for WinLineZ
# ==============================================================================
cmake_minimum_required(VERSION 3.16)
project(WinLineZ VERSION 1.0 LANGUAGES CXX)

# ==================== C++ 标准配置 ====================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==================== 编译选项配置 ====================
# 编译优化选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-narrowing")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

# Windows 特定编译选项（UTF-8 输入，GBK 执行）
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=gbk")
endif()

# 默认构建类型为 Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ==================== 目录配置 ====================
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(GAME_DIR ${CMAKE_SOURCE_DIR}/game)
set(GAME_LIB_DIR ${CMAKE_SOURCE_DIR}/game/lib)
set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/raylib/raylib-5.5_win64_mingw-w64)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ==================== 源文件 ====================
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${GAME_DIR}/game.cpp
    ${GAME_DIR}/interact.cpp
    ${GAME_LIB_DIR}/consts.cpp
    ${GAME_LIB_DIR}/gradient.cpp
    ${GAME_LIB_DIR}/map.cpp
    ${GAME_LIB_DIR}/partical.cpp
    ${GAME_LIB_DIR}/render.cpp
)

set(HEADERS
    ${GAME_DIR}/game.h
    ${GAME_LIB_DIR}/consts.h
    ${GAME_LIB_DIR}/gradient.h
    ${GAME_LIB_DIR}/map.h
    ${GAME_LIB_DIR}/partical.h
    ${GAME_LIB_DIR}/render.h
)

# ==================== 可执行文件 ====================
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 添加头文件搜索路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${CMAKE_SOURCE_DIR}
    ${GAME_DIR}
    ${GAME_LIB_DIR}
    ${RAYLIB_DIR}/include
)

# ==================== 链接库 ====================
link_directories(${RAYLIB_DIR}/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    opengl32
    gdi32
    winmm
)

# Windows 特定链接选项
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
        -Wl,--stack,12582912
        -static
    )
endif()

# ==================== 编译定义 ====================
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

# ==================== 打印配置信息 ====================
message(STATUS "========================================")
message(STATUS "WinLineZ Configuration")
message(STATUS "========================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")
