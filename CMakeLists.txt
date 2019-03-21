cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions("-I/foo/dir1") # インクルードパスを追加
add_definitions("-DMACRO1=bar") # プリプロセッサ定義を追加

file(GLOB_RECURSE SRC_FILES "*.cpp") # 再帰的に.cppファイルを検索する
add_executable(cmake_test ${SRC_FILES})
target_link_libraries(cmake_test pthread) # リンクするライブラリを追加
