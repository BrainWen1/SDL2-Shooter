cmake_minimum_required(VERSION 3.10)

project(SDL2-Shooter CXX)

# 设置C++标准（SDL2支持C++11及以上）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

# 分别查找SDL2核心、Image、TTF、Mixer
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# 检查是否找到所有库
if (NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 未找到！请安装 libsdl2-dev")
endif()
if (NOT SDL2_image_FOUND)
    message(FATAL_ERROR "SDL2_image 未找到！请安装 libsdl2-image-dev")
endif()
if (NOT SDL2_ttf_FOUND)
    message(FATAL_ERROR "SDL2_ttf 未找到！请安装 libsdl2-ttf-dev")
endif()
if (NOT SDL2_mixer_FOUND)
    message(FATAL_ERROR "SDL2_mixer 未找到！请安装 libsdl2-mixer-dev")
endif()

# 添加可执行文件
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})
set(SRC_FILES   ./test/main.cpp
                ./src/Game.cpp
                ./src/ScreenMain.cpp
                ./src/Title.cpp
                ./src/Screen.cpp
                )

add_executable(${TARGET} ${SRC_FILES})

# 链接所有SDL2库
target_link_libraries(${TARGET}
    PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image 
    SDL2_ttf::SDL2_ttf 
    SDL2_mixer::SDL2_mixer
) # find_package :: 命名空间

# 查找include目录内的头文件
target_include_directories(${TARGET}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)