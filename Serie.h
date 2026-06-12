//
// Created by luis1 on 12/06/2026.
//

#ifndef PROYECTOINTEGRADOR_SERIE_H
#define PROYECTOINTEGRADOR_SERIE_H

#include "Video.h"
#include "Episodio.h"
#include <vector>
using namespace std;

class Serie : public Video {
private:
    vector <Episodio> episodios;

public:
    Serie(string id, string nombre, float duracion, string genero);

    void agregarEpisodio(const Episodio& ep);
    const vector <Episodio>& getEpisodios() const {return episodios;}

    void mostrarInfo() const override;
    float getPromedio() const override;
    bool calificarEpisodio(const std::string& tituloEpisodio, float calificacion);
};


#endif //PROYECTOINTEGRADOR_SERIE_H