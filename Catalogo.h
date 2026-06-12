//
// Created by luis1 on 12/06/2026.
//

#ifndef PROYECTOINTEGRADOR_CATALOGO_H
#define PROYECTOINTEGRADOR_CATALOGO_H

#include <vector>
#include <memory>
#include <string>
#include "Video.h"
using namespace std;

class Catalogo {
private:
    vector<unique_ptr<Video>> videos;

public:
    Catalogo() = default;

    void cargarArchivo(const string &arch);
    void mostrarTodos(const string &genero, float calificacionMinima) const;
    void mostrarEpisodios(const string &nombreSerie, float calificacionMinima) const;
    void mostrarPeliculas(float calificacionMinima) const;
    void calificarVideo(string &titulo, float calificacion);

    //overcharge
    bool operator[](const string &titulo) const;
};


#endif //PROYECTOINTEGRADOR_CATALOGO_H