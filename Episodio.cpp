//
// Created by luis1 on 12/06/2026.
//

#include "Episodio.h"
#include "Exceptions.h"
using namespace std;

Episodio::Episodio(string titulo, int temporada) : titulo(titulo), temporada(temporada) {
}

float Episodio::getPromedio() const {
    if (calificaciones.empty()) return 0.0;

    float suma = 0.0;
    for (float c : calificaciones) {
        suma += c;
    }
    return suma / calificaciones.size();
}

//exceptions usage
void Episodio::calificar(float calificacion) {
    if (calificacion<1.0 || calificacion>5.0) {
        throw CalificacionInvalidaError("Las calificaciones deben de estar en un rango del 1 al 5.");
    }
}

//overcharge
bool Episodio::operator<(const Episodio &other) const {
    return this->getPromedio() < other.getPromedio();
}



