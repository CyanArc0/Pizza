# disable-Exceptions-RTTI.cmake - 全局禁用 C++ 异常和 RTTI
# 用法：在 CMakeLists.txt 中 include 此文件即可

message(STATUS "全局禁用 C++ 异常和 RTTI")

# MSVC 编译器
if(MSVC)
    # 禁用异常
    add_compile_options(/EHs-c-)
    add_definitions(-D_HAS_EXCEPTIONS=0)
    
    # 禁用 RTTI
    add_compile_options(/GR-)
    
# GCC 和 Clang 编译器
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # 禁用异常
    add_compile_options(-fno-exceptions)
    
    # 禁用 RTTI
    add_compile_options(-fno-rtti)
    
    # 添加宏定义
    add_definitions(-DNO_EXCEPTIONS -DNO_RTTI)
    
# Intel 编译器
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    add_compile_options(-fno-exceptions -fno-rtti)
    
else()
    message(WARNING "不支持的编译器: ${CMAKE_CXX_COMPILER_ID}")
endif()

# 全局添加宏定义（可以在代码中检查）
add_definitions(
    -DBOOST_NO_EXCEPTIONS
    -DBOOST_NO_RTTI
)

message(STATUS "已禁用异常: -fno-exceptions 或 /EHs-c-")
message(STATUS "已禁用 RTTI: -fno-rtti 或 /GR-")