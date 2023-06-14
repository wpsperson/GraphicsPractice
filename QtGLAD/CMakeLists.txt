cmake_minimum_required(VERSION 3.10.0)

project(QtGladDemo)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)

add_executable(qtdemo
	main.cpp 
	OpenGLWidget.h
	OpenGLWidget.cpp
	Renderer.h
	Renderer.cpp
	glad.c
)

target_link_libraries(qtdemo Qt5::Widgets ${CMAKE_DL_LIBS})


