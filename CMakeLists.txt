cmake_minimum_required(VERSION 3.22)
project(cfmt VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# Setup library
add_library(cfmt INTERFACE
        include/cfmt/indexable.hpp
        include/cfmt/view.hpp
        include/cfmt/cfmt.hpp
        include/cfmt/sign/exceptions.hpp include/cfmt/sign/match.hpp include/cfmt/sign/blueprint.hpp)

target_include_directories(cfmt INTERFACE include/)

set_target_properties(cfmt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(cfmt PROPERTIES VERSION ${PROJECT_VERSION})

include(GNUInstallDirs)
install(DIRECTORY include/cfmt DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(cfmt.pc.in cfmt.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/cfmt.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Setup tests
add_executable(cfmt_formatter test/formatter.cpp)
add_executable(cfmt_view test/view.cpp)
add_executable(cfmt_sign test/sign.cpp)

target_link_libraries(cfmt_formatter cfmt)
target_link_libraries(cfmt_view cfmt)
target_link_libraries(cfmt_sign cfmt)