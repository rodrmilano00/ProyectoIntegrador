//
// Created by luis1 on 12/06/2026.
//

#ifndef PROYECTOINTEGRADOR_VIDEO_H
#define PROYECTOINTEGRADOR_VIDEO_H

#include <string>
#include <vector>
using namespace std;

class Video {
protected:
    string id;
    string nombre;
    float duracion;
    string genero;
    vector<float> calificaciones;

public:
    Video(string id, string nombre, float duracion, string genero);
    virtual ~Video() = default; //polimorfism

    //getters
    string getId();
    string getNombre();
    string getGenero();
    float getDuracion();

    virtual float getPromedio() const;
    virtual void calificar(float calificacion);
    virtual void mostrarInfo() const = 0; //pure virtual

    virtual bool operator<(const Video &other) const; //operator overcharge

};


#endif //PROYECTOINTEGRADOR_VIDEO_H