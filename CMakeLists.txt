cmake_minimum_required(VERSION 3.20)
project(FBCross LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()


# cmake_policy(SET CMP0144 NEW)    # 启用 BOOST_ROOT 变量
cmake_policy(SET CMP0167 NEW)    # 使用新版 Boost 查找逻辑



# include(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)

add_subdirectory("packs/src/libpqxx" "packs/libpqxx")


find_package(Boost REQUIRED COMPONENTS
    system          # 必须包含
    thread          # 若使用多线程功能
)

# if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
#     find_package(PostgreSQL REQUIRED
#         PATHS ${PostgreSQL_DIR}  # 强制指定用户设置的路径
#         NO_DEFAULT_PATH
#     )
# else ()
#     find_package(PostgreSQL REQUIRED)
# endif()


find_package(PostgreSQL REQUIRED)

find_package(OpenSSL REQUIRED)
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# 添加可执行文件（包含所有源文件）
add_executable(FBCross
    ${SOURCE_FILES}
)

# 包含头文件目录
target_include_directories(FBCross PUBLIC
    "include"
    ${PostgreSQL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

# 链接库
target_link_libraries(FBCross PRIVATE
    ${PostgreSQL_LIBRARIES}
    ${Boost_LIBRARIES}
    pqxx
    OpenSSL::SSL
    OpenSSL::Crypto
    # gettext
    # iconv                # iconv 库（可能需要）
)

if (UNIX)
    target_link_libraries(FBCross PRIVATE
        intl                 # gettext 的 intl 库
    )
endif()

if (WIN32)
    target_compile_definitions(FBCross PRIVATE _WIN32)
    target_link_libraries(FBCross PRIVATE
        z
        ws2_32
        crypt32
        secur32
        gdi32
        user32
    )
endif()