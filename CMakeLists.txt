# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)
# 设置项目名称
project(RunningBug)

# 搜索并收集utils文件夹下的所有源文件
file(GLOB UTILS "utils/*.cpp", "utils/*.h")

# 添加可执行文件（还要加入utils文件夹下的源文件）
add_executable(RunningBug main.cpp ${UTILS})

# 链接所需的库
target_link_libraries(RunningBug PRIVATE winmm)

# 检查项目是否有Media目录，如果存在，则在使用add_custom_command命令在构建后将Media目录中的文件复制到项目的输出目录
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Media")
if(EXISTS ${SOURCE_DIR})
    add_custom_command(TARGET RunningBug POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR} $<TARGET_FILE_DIR:RunningBug>)
endif()