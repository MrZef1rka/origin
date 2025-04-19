cmake_minimum_required(VERSION 3.16)

project(Console_app LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(Console_app
  main.cpp
)
target_link_libraries(Console_app PRIVATE Qt6::Core)
target_link_libraries(Console_app PRIVATE Qt6::Sql)
target_link_libraries(Console_app PRIVATE Qt6::Network)


include(GNUInstallDirs)
install(TARGETS Console_app
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
