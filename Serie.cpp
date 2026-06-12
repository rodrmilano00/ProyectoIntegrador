//
// Created by luis1 on 12/06/2026.
//

#include "Serie.h"
#include <iostream>
using namespace std;

Serie::Serie(string id, string nombre, float duracion, string genero) : Video(id, nombre, duracion, genero) {
}

void Serie::agregarEpisodio(const Episodio &ep) {
    episodios.push_back(ep);
}

float Serie::getPromedio() {
    if (episodios.empty()) return 0.0;

    float suma = 0.0;
    for (const auto &ep : episodios) {
        suma += ep.getPromedio();
    }
    return suma / episodios.size();
}

void Serie::mostrarInfo() const {
    cout << "[Serie] ID: " << id
              << " | Título: " << nombre
              << " | Género: " << genero
              << " | Duración: " << duracion << " min"
              << " | Episodios: " << episodios.size()
              << " | Promedio: " << getPromedio() << "\n";
}



