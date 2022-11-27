cmake_minimum_required(VERSION 3.24)

project(felix)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS locale REQUIRED)

add_library(felix
        include/felix/String.hpp
        include/felix/StringStorage.hpp
        include/felix/Graphemes.hpp
        src/String.cpp
)

target_link_libraries(felix Boost::locale)
target_include_directories(felix PUBLIC include)

add_executable(felix_main
        main.cpp
)

target_link_libraries(felix_main felix)

add_subdirectory(catch)
