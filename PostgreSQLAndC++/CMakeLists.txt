cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "17")

project(PostgreSQLAndC++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libpqxx REQUIRED)

add_executable(PostgreSQLAndC++ main.cpp)

target_link_libraries(PostgreSQLAndC++ pqxx)
