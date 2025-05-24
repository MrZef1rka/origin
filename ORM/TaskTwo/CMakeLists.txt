cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "17")

project(TaskOne)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(libpqxx REQUIRED)
find_package(Wt REQUIRED COMPONENTS Wt Dbo DboPostgres)

add_executable(TaskOne main.cpp)

target_link_libraries(TaskOne PRIVATE
	pqxx
	Wt::Wt
	Wt::Dbo
	Wt::DboPostgres)