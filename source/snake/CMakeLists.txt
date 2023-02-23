project(snake)
add_executable(snake Game.cpp snake.cpp)
target_link_libraries(snake sfml-graphics)
set_target_properties(snake PROPERTIES FOLDER "bin")