
find_package(libpng REQUIRED)

list(APPEND
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CFLAGS
    -Wall -Wextra -pedantic
    -Wconversion -Wshadow -Wstrict-aliasing
    -Winit-self -Wcast-align -Wpointer-arith
    -Wmissing-declarations -Wmissing-include-dirs
    -Wno-unused-parameter -Wuninitialized
)

add_executable(splash ${SOURCES})
target_compile_options(splash PUBLIC ${CFLAGS})

target_include_directories(splash PUBLIC ${LIBPNG_INCLUDE_DIRS})

target_link_libraries(splash LINK_PUBLIC ${LIBPNG_LIBRARIES})
target_link_libraries(splash LINK_PUBLIC m)
