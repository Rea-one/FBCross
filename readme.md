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
Boost直接用msys2安装
安装PostgreSQL需要的库，然后源码安装

### MSVC

Boost按照官方文档默认安装就行
PostgreSQL直接用安装包安装
