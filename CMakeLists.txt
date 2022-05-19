cmake_minimum_required(VERSION 3.20)

project(orugooru)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(-O3 -Wall -Wpedantic -Wextra -Wformat=2 -Wshadow -Wcast-align -Wconversion -Wfloat-equal -Werror=shadow) # 自定义编译选项

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_DIR) # 添加源代码文件夹, 自动扫描所有文件

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # 可执行文件输出目录

option(TERMUX "Build in Termux" off) # 开启termux的编译宏
if(TERMUX)
    add_compile_definitions(TERMUX)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/indicators/include)

# 编译
add_executable(
    ${PROJECT_NAME} 
    ${SRC_DIR} 
    ${PROJECT_SOURCE_DIR}/main.cpp
) # 编译

# 链接
target_link_libraries(
    ${PROJECT_NAME}  boost_program_options icui18n icuuc magic
)# 链接
