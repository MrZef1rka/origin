cmake_minimum_required(VERSION 3.22.0)
project(complex_project)
set(SOURCES
	Class_Hierarchy.cpp
	Figure.cpp
	Triangle.cpp
	RightTriangle.cpp
	EquilateralTriangle.cpp
	IsoscelesTriangle.cpp
	Rectangle.cpp
	Quadrilateral.cpp
	Parallelogram.cpp
	Rhombus.cpp
	Square.cpp
)
add_executable(Class_Hierarchy ${SOURCES})