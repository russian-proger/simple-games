project(simple-games)

cmake_minimum_required(VERSION 3.22)


find_package(SFML COMPONENTS graphics audio REQUIRED)

# target_link_libraries(simple-games sfml-graphics)


add_subdirectory(source/snake)

add_compile_options(-O3)