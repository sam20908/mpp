project("Sandobx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Sandbox "matrix.hpp")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Sandbox PUBLIC -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wnon-virtual-dtor -Werror -pedantic-errors)
else()
    target_compile_options(Sandbox PUBLIC /W4 /WX)
endif()
