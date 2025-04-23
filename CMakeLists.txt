cmake_minimum_required(VERSION 3.20)
project(FBCross LANGUAGES CXX)

cmake_policy(SET CMP0144 NEW)    # 启用 BOOST_ROOT 变量
cmake_policy(SET CMP0167 NEW)    # 使用新版 Boost 查找逻辑

include(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)

find_package(Boost REQUIRED COMPONENTS 
    system          # 必须包含
    thread          # 若使用多线程功能
)


set(POSTGRESQL_INCLUDE_DIR "${POSTGRESQL_ROOT}/include")
set(POSTGRESQL_LIBRARY "${POSTGRESQL_ROOT}/lib/libpq.a")
set(LIBPQXX_INCLUDE_DIR "${LIBPQXX_ROOT}/include")
set(LIBPQXX_LIBRARY "${LIBPQXX_ROOT}/build/src/LIBPQXX.a")
set(LIBPQXX_BUILD_INCLUDE_DIR "${LIBPQXX_ROOT}/build/include")

# 添加可执行文件（包含所有源文件）
add_executable(FBCross
    src/main.cpp
    src/listener.cpp
    src/threads.cpp
    src/connect_pool.cpp
    src/workers.cpp
    src/server.cpp
    src/receiver.cpp
)

# 包含头文件目录
target_include_directories(FBCross PUBLIC
    include               # 当前项目的头文件目录
    ${POSTGRESQL_INCLUDE_DIR}
    ${LIBPQXX_INCLUDE_DIR}
    ${LIBPQXX_BUILD_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

# 链接库
target_link_libraries(FBCross
    ${POSTGRESQL_LIBRARY}
    ${LIBPQXX_LIBRARY}
    Boost::boost
    Boost::system
    Boost::thread
)

if (WIN32)
    target_compile_definitions(FBCross PRIVATE _WIN32)
ENDIF()