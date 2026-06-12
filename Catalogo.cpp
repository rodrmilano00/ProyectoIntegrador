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

string limpiarCadena(const string& str) {
    string aux = str;
    while (!aux.empty() && (aux.back() == '\r' || aux.back() == '\n' || aux.back() == ' ')) {
        aux.pop_back();
    }
    while (!aux.empty() && aux.front() == ' ') {
        aux.erase(0, 1);
    }
    return aux;
}

void Catalogo::cargarArchivo(const string &arch) {
    ifstream archivo(arch);
    if (!archivo.is_open()) {
        throw ArchivoInvalidoError("No se pudo abrir el archivo especificado");
    }

    videos.clear();
    string linea;
    int numLinea = 0;

    while (getline(archivo, linea)) {
        numLinea++;
        if (linea.empty()) continue;

        stringstream ss(linea);
        string tipo;
        getline(ss, tipo, ',');
        tipo = limpiarCadena(tipo);

        try {
            if (tipo == "P") { // Pelicula
                string id, nombre, durStr, genero, director;
                getline(ss, id, ',');
                getline(ss, nombre, ',');
                getline(ss, durStr, ',');
                getline(ss, genero, ',');
                getline(ss, director, ',');

                id = limpiarCadena(id);
                nombre = limpiarCadena(nombre);
                durStr = limpiarCadena(durStr);
                genero = limpiarCadena(genero);
                director = limpiarCadena(director);

                if (id.empty() || nombre.empty() || durStr.empty() || genero.empty() || director.empty()) {
                    throw ArchivoInvalidoError("Formato incorrecto en el archivo (Pelicula)");
                }

                auto peli = make_unique<Pelicula>(id, nombre, stof(durStr), genero, director);
                string califStr;
                while (getline(ss, califStr, ',')) {
                    califStr = limpiarCadena(califStr);
                    if (!califStr.empty()) peli->calificar(stof(califStr));
                }
                videos.push_back(std::move(peli));

            } else if (tipo == "S") { // Serie
                string id, nombre, durStr, genero;
                getline(ss, id, ',');
                getline(ss, nombre, ',');
                getline(ss, durStr, ',');
                getline(ss, genero, ',');

                id = limpiarCadena(id);
                nombre = limpiarCadena(nombre);
                durStr = limpiarCadena(durStr);
                genero = limpiarCadena(genero);

                if (id.empty() || nombre.empty() || durStr.empty() || genero.empty()) {
                    throw ArchivoInvalidoError("Formato incorrecto en el archivo (Serie)");
                }
                videos.push_back(make_unique<Serie>(id, nombre, stof(durStr), genero));

            } else if (tipo == "E") { // Episodio
                string idSerie, tituloEp, tempStr;
                getline(ss, idSerie, ',');
                getline(ss, tempStr, ',');
                getline(ss, tituloEp, ',');

                idSerie = limpiarCadena(idSerie);
                tempStr = limpiarCadena(tempStr);
                tituloEp = limpiarCadena(tituloEp);

                if (idSerie.empty() || tempStr.empty() || tituloEp.empty()) {
                    throw ArchivoInvalidoError("Formato incorrecto en el archivo (Episodio)");
                }

                Episodio ep(tituloEp, stoi(tempStr));
                string califStr;
                while (getline(ss, califStr, ',')) {
                    califStr = limpiarCadena(califStr);
                    if (!califStr.empty()) ep.calificar(stof(califStr));
                }

                bool agregada = false;
                for (auto& v : videos) {
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
                    throw VideoNoEncontradoError("No se pudo encontrar el ID de Serie para asignar el episodio");
                }
            }
        }
        catch (const invalid_argument& e) {
            throw ArchivoInvalidoError("Error de conversion numerica (stoi/stof) en la linea " + to_string(numLinea) + ". Revise el archivo.");
        }
    }
    archivo.close();
}

void Catalogo::mostrarTodos(const string &genero, float calificacionMinima) const {
    if (!genero.empty()) {
        string gLower = genero;
        transform(gLower.begin(), gLower.end(), gLower.begin(), ::tolower);
        if (gLower != "accion" && gLower != "drama" && gLower != "misterio") {
            throw GeneroInvalidoError("Genero no valido (Solo Accion, Drama, Misterio)");
        }
    }

    bool impreso = false;
    for (const auto& v : videos) {
        bool filtroGen = genero.empty() || v->getGenero() == genero;
        bool filtroCal = v->getPromedio() >= calificacionMinima;

        if (filtroGen && filtroCal) {
            v->mostrarInfo();
            impreso = true;
        }
    }
    if (!impreso) cout << "Ningun video coincide con el filtro.\n";
}

void Catalogo::mostrarEpisodios(const string& nombreSerie, float calificacionMinima) const {
    bool encontrada = false;
    for (const auto& v : videos) {
        auto* s = dynamic_cast<Serie*>(v.get());
        if (s && s->getNombre() == nombreSerie) {
            encontrada = true;
            cout << "\n--- Episodios de: " << nombreSerie << " (Minimo " << calificacionMinima << ") ---\n";
            bool algunEp = false;
            for (const auto& ep : s->getEpisodios()) {
                if (ep.getPromedio() >= calificacionMinima) {
                    // Mantiene ep.getTemporoda() tal como esta implementado en tu clase Episodio
                    cout << " * Temp " << ep.getTemporoda() << " | " << ep.getTitulo()
                         << " | Calificacion: " << ep.getPromedio() << "\n";
                    algunEp = true;
                }
            }
            if (!algunEp) cout << "Ningun episodio cumple con la calificacion minima.\n";
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
    if (!impresa) cout << "No hay peliculas que cumplan con la calificacion minima.\n";
}

void Catalogo::calificarVideo(string& titulo, float calificacion) {
    if (calificacion < 1.0f || calificacion > 5.0f) {
        throw CalificacionInvalidaError("La calificacion otorgada debe estar en el rango de 1 a 5.");
    }
    for (auto& v : videos) {
        if (v->getNombre() == titulo) {
            v->calificar(calificacion);
            cout << "Exito! Calificacion registrada para " << titulo << ".\n";
            return;
        }

        auto* s = dynamic_cast<Serie*>(v.get());
        if (s) {
            if (s->calificarEpisodio(titulo, calificacion)) {
                cout << "Exito! Calificacion registrada para el episodio: " << titulo << ".\n";
                return;
            }
        }
    }
    throw VideoNoEncontradoError("No se localizo ninguna Pelicula, Serie o Episodio llamado '" + titulo + "'.");
}

bool Catalogo::operator[](const string& titulo) const {
    for (const auto& v : videos) {
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