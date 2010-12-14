cmake_minimum_required(VERSION 2.8)

set(PREFIX ../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PREFIX})

set(CMAKE_MODULE_PATH ./)
find_package(SFML REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SERVER ${PREFIX}/src/server/*.cpp)
file(GLOB_RECURSE VIS ${PREFIX}/src/vis/*.cpp)
file(GLOB COMMON ${PREFIX}/src/common/*.cpp)

include_directories(${PREFIX}/include/ ${PREFIX}/include/server/ ${PREFIX}/include/vis/ ${PREFIX}/include/common/ ${SFML_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(server ${SERVER} ${COMMON})
target_link_libraries(server ${SFML_LIBRARY})
add_executable(vis ${VIS} ${COMMON})
target_link_libraries(vis ${SFML_LIBRARY} ${OPENGL_gl_LIBRARY})