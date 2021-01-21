cmake_minimum_required(VERSION 3.7)
project(DUNGEON_CRAWL)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 17)

include(contrib/contrib.cmake)
add_subdirectory(app)