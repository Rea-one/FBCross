cmake_minimum_required(VERSION 3.20)
project(FBCross LANGUAGES CXX)


cmake_policy(SET CMP0144 NEW)    # 启用 BOOST_ROOT 变量
cmake_policy(SET CMP0167 NEW)    # 使用新版 Boost 查找逻辑


add_subdirectory("packs/src/libpqxx" "packs/build/libpqxx")


include(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)

# list(PREPEND CMAKE_PREFIX_PATH ${POSTGRESQL_ROOT}/bin
#     ${POSTGRESQL_ROOT}/lib
#     ${POSTGRESQL_ROOT}/include
# )


find_package(Boost REQUIRED COMPONENTS 
    system          # 必须包含
    thread          # 若使用多线程功能
)
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
    intl                 # gettext 的 intl 库
    iconv                # iconv 库（可能需要）
)

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