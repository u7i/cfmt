cmake_minimum_required(VERSION 3.22)
project(cfmt VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# Setup library
add_library(cfmt INTERFACE)
target_include_directories(cfmt INTERFACE include/)

set_target_properties(cfmt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(cfmt PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cfmt PROPERTIES PUBLIC_HEADER include/cfmt.hpp)

include(GNUInstallDirs)
install(TARGETS cfmt
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(cfmt.pc.in cfmt.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/cfmt.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Setup tests
add_executable(cfmt_simple_format test/simple_format.cpp)
target_link_libraries(cfmt_simple_format cfmt)