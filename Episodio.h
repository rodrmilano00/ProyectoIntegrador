//
// Created by luis1 on 12/06/2026.
//

#ifndef PROYECTOINTEGRADOR_EPISODIO_H
#define PROYECTOINTEGRADOR_EPISODIO_H

#include <vector>
#include <string>
using namespace std;

class Episodio {
private:
    string titulo;
    int temporada;
    vector<float> calificaciones;

public:
    Episodio(string titulo, int temporada);

    string getTitulo() const {return titulo;}
    int getTemporoda() const {return temporada;}

    float getPromedio() const;
    void calificar(float calificacion);
    //overcharge
    bool operator<(const Episodio &other) const;
};


#endif //PROYECTOINTEGRADOR_EPISODIO_H