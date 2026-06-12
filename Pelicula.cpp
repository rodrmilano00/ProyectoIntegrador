//
// Created by luis1 on 12/06/2026.
//

#include "Pelicula.h"
#include <iostream>
using namespace std;

Pelicula::Pelicula(string id, string nombre, float duracion, string genero, string director) : Video(id, nombre, duracion, genero), director(director) {
}

void Pelicula::mostrarInfo() const {
    cout << "[Película] ID: " << id
              << " | Título: " << nombre
              << " | Género: " << genero
              << " | Duración: " << duracion << " min"
              << " | Director: " << director
              << " | Promedio: " << getPromedio() << "\n";
}

bool Pelicula::operator==(const Pelicula &other) const {
    return this-> nombre == other.nombre();
}

