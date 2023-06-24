cmake_minimum_required(VERSION 3.23)
project(socp)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-stdlib=libc++ -fexperimental-library)
add_link_options(-stdlib=libc++ -fexperimental-library)

add_subdirectory(SOCPU)
add_subdirectory(SOCC)

file(GLOB SOCP_DEV_SRC CONFIGURE_DEPENDS "src/dev/*.cpp")
add_library(dev ${SOCP_DEV_SRC})
target_include_directories(dev PUBLIC include)
target_link_libraries(dev libsocc-soisv1)

file(GLOB SOCP_PROGRAMS_SRC CONFIGURE_DEPENDS "src/programs/*.cpp")
add_executable(socp main.cpp src/socp.cpp ${SOCP_PROGRAMS_SRC})
target_include_directories(socp PUBLIC include)
target_link_libraries(socp dev libsocc-soisv1 soarchv2_soisv1 libdcsim)