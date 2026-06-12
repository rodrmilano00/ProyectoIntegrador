//
// Created by luis1 on 12/06/2026.
//

#include "Video.h"
#include "Exceptions.h"

Video::Video(string id, string nombre, float duracion, string genero) : id(id), nombre(nombre), duracion(duracion), genero(genero) {
}

//average ratings
float Video::getPromedio() const {
    if (calificaciones.empty()) return 0.0;

    float suma = 0.0;
    for (float c : calificaciones) {
        suma += c;
    }
    return suma / calificaciones.size();
}

//exceptions usage
void Video::calificar(float calificacion) {
    if (calificacion<1.0 || calificacion>5.0) {
        throw CalificacionInvalidaError("Las calificaciones deben de ser en un rango del 1 al 5.");
    }
    calificaciones.push_back(calificacion);
}

//operator overcharge
bool Video::operator<(const Video &other) const {
    return this->getPromedio() < other.getPromedio();
}

string Video::getId() {
    return id;
}

string Video::getNombre() {
    return nombre;
}

string Video::getGenero() {
    return genero;
}




