//
// Created by luis1 on 12/06/2026.
//

#ifndef PROYECTOINTEGRADOR_PELICULA_H
#define PROYECTOINTEGRADOR_PELICULA_H

#include "Video.h"
using namespace std;

class Pelicula : public Video {
private:
    string director;

public:
    Pelicula(string id, string nombre, float duracion, string genero, string director);

    void mostrarInfo() const override;
    bool operator==(const Pelicula &other) const;
};


#endif //PROYECTOINTEGRADOR_PELICULA_H