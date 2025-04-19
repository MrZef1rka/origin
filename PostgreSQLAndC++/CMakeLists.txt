cmake_minimum_required (VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

project (PostgreSQLAndC++)

add_subdirectory("C:\\Users\\MrZefirka\\Desktop\\libpqxx-7.9.2" libpqxx-build)

add_executable (PostgreSQLAndC++ main.cpp)

target_compile_features(PostgreSQLAndC++ PRIVATE cxx_std_17)

target_link_libraries(PostgreSQLAndC++ PRIVATE pqxx)
