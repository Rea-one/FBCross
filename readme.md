# 依赖

Boost
PostgreSQL
libpqxx

## 通用项

libpqxx直接复制到当前目录的packs/src下
cmake添加add_subdirectory("packs/src/libpqxx" "这里是build目录中libpqxx构建出的目录")

## linux下

直接apt就好

## windows下

### GNU系列编译器

推荐msys2
Boost和PostgreSQL都用msys2安装

### MSVC

至于MSVC，我只能祝它好运


## 注意项

CMakeLists.txt目前暂不提供，由于在ubuntu和windows中都在尝试构建此项目，追踪过于繁琐，目前暂不提供，且目前项目存在海量问题
