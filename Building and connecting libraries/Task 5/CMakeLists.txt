cmake_minimum_required(VERSION 3.22.0)
project(DynamicLibrary)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(DynamicLibrary SHARED
	ClassHierarchy.cpp
	DynamicLibrary.h
	src/Figure.cpp
	include/Figure.h
	src/Triangle.cpp
	include/Triangle.h
	src/RightTriangle.cpp
	include/RightTriangle.h
	src/EquilateralTriangle.cpp
	include/EquilateralTriangle.h
	src/IsoscelesTriangle.cpp
	include/IsoscelesTriangle.h
	src/Rectangle.cpp
	include/Rectangle.h
	src/Quadrilateral.cpp
	include/Quadrilateral.h
	src/Parallelogram.cpp
	include/Parallelogram.h
	src/Rhombus.cpp
	include/Rhombus.h
	src/Square.cpp
	include/Square.h
)
add_executable(ClassHierarchy ClassHierarchy.cpp)
target_link_libraries(ClassHierarchy DynamicLibrary)