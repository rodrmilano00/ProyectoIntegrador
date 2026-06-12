cmake_minimum_required(VERSION 4.0)
project(ProyectoIntegrador)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProyectoIntegrador main.cpp
        Exceptions.h
        Video.cpp
        Video.h
        Pelicula.cpp
        Pelicula.h
        Episodio.cpp
        Episodio.h
        Serie.cpp
        Serie.h
        Catalogo.cpp
        Catalogo.h)
