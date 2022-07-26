cmake_minimum_required(VERSION 3.22)
project(cfmt VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# Setup library
add_library(cfmt INTERFACE
        include/cfmt/indexable.hpp
        include/cfmt/view.hpp
        include/cfmt/cfmt.hpp
        include/cfmt/formatter.hpp
        include/cfmt/group/bad_match.hpp
        include/cfmt/group/match.hpp
        include/cfmt/group/blueprint.hpp
        )

target_include_directories(cfmt INTERFACE include/)

set_target_properties(cfmt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(cfmt PROPERTIES VERSION ${PROJECT_VERSION})

include(GNUInstallDirs)
install(DIRECTORY include/cfmt DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(cfmt.pc.in cfmt.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/cfmt.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Setup tests
add_executable(cfmt_formatter test/formatter.cpp test/base.hpp)
add_executable(cfmt_view test/view.cpp test/base.hpp)
add_executable(cfmt_group test/group.cpp test/base.hpp)

target_link_libraries(cfmt_formatter cfmt)
target_link_libraries(cfmt_view cfmt)
target_link_libraries(cfmt_group cfmt)