//
// Created by luis1 on 12/06/2026.
//

#include "Catalogo.h"

#include <algorithm>

#include "Pelicula.h"
#include "Serie.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ranges>
using namespace std;

void Catalogo::cargarArchivo(const string &arch) {
    ifstream archivo(arch);
    if (!archivo.is_open()) {
        throw ArchivoInvalidoError("No se pudo abrir el archivo especificado"); //exceptions usage
    }

    videos.clear();

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string tipo;
        getline(ss, tipo, ',');

        if (tipo == "P") { //pelicula
            string id, nombre, durStr, genero, director;
            getline(ss, id, ',');
            getline(ss, nombre, ',' );
            getline(ss, durStr, ',');
            getline(ss, genero, ',');
            getline(ss, director, ',');

            if (id.empty() || nombre.empty() || durStr.empty() || genero.empty() || director.empty()) {
                throw ArchivoInvalidoError("Formato incorrecto en el archivo"); //exceptions usage
            }

            auto peli = make_unique<Pelicula>(id, nombre, stof(durStr), genero, director);
            string califStr;
            while (getline(ss, califStr, ',')) {
                if (!califStr.empty()) peli->calificar(stof(califStr));
            }
            videos.push_back(std::move(peli));

        } else if (tipo == "S") { // serie
            string id, nombre, durStr, genero;
            getline(ss, id, ',');
            getline(ss, nombre, ',');
            getline(ss, durStr, ',');
            getline(ss, genero, ',');

            if (id.empty() || nombre.empty() || durStr.empty() || genero.empty()) {
                throw ArchivoInvalidoError("Formato incorrecto en el archivo"); //exceptions usage
            }
            videos.push_back(make_unique<Serie>(id, nombre, stof(durStr), genero));

        } else if (tipo == "E") { //episodio
            string idSerie, tituloEp, tempStr;
            getline(ss, idSerie, ',');
            getline(ss, tempStr, ',');
            getline(ss, tituloEp, ',');

            if (idSerie.empty() || tempStr.empty() || tituloEp.empty()) {
                throw ArchivoInvalidoError("Formato incorrecto en el archivo");
            }

            Episodio ep(tituloEp, stoi(tempStr));
            string califStr;
            while (getline(ss, califStr, ',')) {
                if (!califStr.empty()) ep.calificar(stof(califStr));
            }

            //search for id
            bool agregada = false;
            for (auto& v:videos) {
                if (v->getId() == idSerie) {
                    auto* s = dynamic_cast<Serie*>(v.get());
                    if (s) {
                        s->agregarEpisodio(ep);
                        agregada = true;
                        break;
                    }
                }
            }
            if (!agregada) {
                throw VideoNoEncontradoError("No se pudo encontrar el ID para asignar el episodio");
            }
        }
    }
    archivo.close();
}

void Catalogo::mostrarTodos(const string &genero, float calificacionMinima) const {
    if (!genero.empty()) {
        string gLower = genero;
        transform(gLower.begin(), gLower.end(), gLower.begin(), ::tolower);
        if (gLower != "accion" && gLower != "accion" && gLower != "drama" && gLower != "misterio") {
            throw GeneroInvalidoError("Genero no valido");
        }
    }

    bool impreso = false;
    for (const auto& v:videos) {
        bool filtroGen = genero.empty() || v->getGenero() == genero;
        bool filtroCal = v->getPromedio() >= calificacionMinima;

        if (filtroGen && filtroCal) {
            v->mostrarInfo();
            impreso = true;
        }
    }
    if (!impreso) cout << "Ningun video concide con el filtro.\n";
}


void Catalogo::mostrarEpisodios(const std::string& nombreSerie, float calificacionMinima) const {
    bool encontrada = false;
    for (const auto& v : videos) {
        auto* s = dynamic_cast<Serie*>(v.get());
        if (s && s->getNombre() == nombreSerie) {
            encontrada = true;
            std::cout << "\n--- Episodios de: " << nombreSerie << " (Mínimo " << calificacionMinima << ") ---\n";
            bool algunEp = false;
            for (const auto& ep : s->getEpisodios()) {
                if (ep.getPromedio() >= calificacionMinima) {
                    std::cout << " * Temp " << ep.getTemporoda() << " | " << ep.getTitulo()
                              << " |  Calificación: " << ep.getPromedio() << "\n";
                    algunEp = true;
                }
            }
            if (!algunEp) std::cout << "Ningún episodio cumple con la calificación mínima.\n";
            return;
        }
    }
    if (!encontrada) throw VideoNoEncontradoError("La serie '" + nombreSerie + "' no se encuentra en el sistema.");
}


void Catalogo::mostrarPeliculas(float calificacionMinima) const {
    bool impresa = false;
    for (const auto& v : videos) {
        auto* p = dynamic_cast<Pelicula*>(v.get());
        if (p && p->getPromedio() >= calificacionMinima) {
            p->mostrarInfo();
            impresa = true;
        }
    }
    if (!impresa) std::cout << "No hay películas que cumplan con la calificación mínima.\n";
}

void Catalogo::calificarVideo(const std::string& titulo, float calificacion) {
    if (calificacion < 1.0f || calificacion > 5.0f) {
        throw CalificacionInvalidaError("La calificación otorgada debe estar en el rango de 1 a 5.");
    }

    for (auto& v : videos) {
        if (v->getNombre() == titulo) {
            v->calificar(calificacion);
            std::cout << "¡Éxito! Calificación registrada para " << titulo << ".\n";
            return;
        }
        auto* s = dynamic_cast<Serie*>(v.get());
        if (s) {
            for (auto& ep : s->getEpisodios()) {
                if (ep.getTitulo() == titulo) {
                    ep.calificar(calificacion);
                    std::cout << "¡Éxito! Calificación registrada para el episodio: " << titulo << ".\n";
                    return;
                }
            }
        }
    }
    throw VideoNoEncontradoError("No se localizó ninguna Película, Serie o Episodio llamado '" + titulo + "'.");
}

bool Catalogo::operator[](const std::string& titulo) const {
    for (const auto& v : _videos) {
        if (v->getNombre() == titulo) return true;
        auto* s = dynamic_cast<Serie*>(v.get());
        if (s) {
            for (const auto& ep : s->getEpisodios()) {
                if (ep.getTitulo() == titulo) return true;
            }
        }
    }
    return false;
}