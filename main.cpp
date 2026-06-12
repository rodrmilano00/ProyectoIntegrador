#include <iostream>
#include <limits>
#include <string>
#include "Catalogo.h"
#include "Exceptions.h"

using namespace std;

void vaciarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    Catalogo sistema;
    int opcion = -1;
    bool datosCargados = false;

    while (opcion != 0) {
        cout << "\n=== PLATAFORMA STREAMING ===\n";
        cout << "1. Cargar archivo de datos\n";
        cout << "2. Mostrar videos por calificacion o genero\n";
        cout << "3. Mostrar episodios de una serie especifica\n";
        cout << "4. Mostrar peliculas por calificacion\n";
        cout << "5. Calificar un video o episodio\n";
        cout << "0. Salir de la plataforma\n";
        cout << "============================================================\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cout << "[Error] Entrada invalida. Ingrese un digito del menu.\n";
            vaciarBuffer();
            continue;
        }
        vaciarBuffer();

        try {
            if (opcion > 1 && opcion <= 5 && !datosCargados) {
                cout << "[Aviso] El catalogo esta vacio. Cargue el archivo de datos primero (Opcion 1).\n";
                continue;
            }

            switch (opcion) {
                case 1: {
                    string ruta;
                    cout << "Ingrese el nombre/ruta del archivo (ej: datos.txt): ";
                    getline(cin, ruta);
                    sistema.cargarArchivo(ruta);
                    cout << "Catalogo procesado con exito!\n";
                    datosCargados = true;
                    break;
                }
                case 2: {
                    cout << "Filtrar por: (1) Genero o (2) Calificacion Minima: ";
                    int subOpc;
                    if (!(cin >> subOpc)) { vaciarBuffer(); throw invalid_argument("Opcion invalida."); }
                    vaciarBuffer();

                    if (subOpc == 1) {
                        string gen;
                        cout << "Ingrese genero (Accion, Drama, Misterio): ";
                        getline(cin, gen);
                        sistema.mostrarTodos(gen, 0.0f);
                    } else if (subOpc == 2) {
                        float calif;
                        cout << "Ingrese umbral de calificacion (1-5): ";
                        if (!(cin >> calif)) { vaciarBuffer(); throw CalificacionInvalidaError("No ingreso un numero."); }
                        vaciarBuffer();
                        sistema.mostrarTodos("", calif);
                    } else {
                        cout << "Seleccion no valida.\n";
                    }
                    break;
                }
                case 3: {
                    string serie;
                    float calif;
                    cout << "Nombre exacto de la Serie: ";
                    getline(cin, serie);
                    cout << "Calificacion minima requerida para los episodios: ";
                    if (!(cin >> calif)) { vaciarBuffer(); throw CalificacionInvalidaError("No es un valor numerico."); }
                    vaciarBuffer();
                    sistema.mostrarEpisodios(serie, calif);
                    break;
                }
                case 4: {
                    float calif;
                    cout << "Ingrese calificacion minima para filtrar peliculas: ";
                    if (!(cin >> calif)) { vaciarBuffer(); throw CalificacionInvalidaError("No es un valor numerico."); }
                    vaciarBuffer();
                    sistema.mostrarPeliculas(calif);
                    break;
                }
                case 5: {
                    string titulo;
                    float calif;
                    cout << "Ingrese el titulo exacto de la Pelicula, Serie o Episodio: ";
                    getline(cin, titulo);
                    cout << "Asigne su calificacion (1 a 5): ";
                    if (!(cin >> calif)) { vaciarBuffer(); throw CalificacionInvalidaError("Calificacion no numerica."); }
                    vaciarBuffer();
                    sistema.calificarVideo(titulo, calif);
                    break;
                }
                case 0:
                    cout << "Cerrando sesion en el servicio de streaming... Buen dia!\n";
                    break;
                default:
                    cout << "Opcion fuera de rango. Reintente.\n";
            }
        }
        catch (const CalificacionInvalidaError& e) { cerr << "[ERROR DE VALIDACION] " << e.what() << "\n"; }
        catch (const VideoNoEncontradoError& e) { cerr << "[ERROR DE SISTEMA] " << e.what() << "\n"; }
        catch (const ArchivoInvalidoError& e) { cerr << "[ERROR DE ARCHIVO] " << e.what() << "\n"; }
        catch (const GeneroInvalidoError& e) { cerr << "[ERROR DE FORMATO] " << e.what() << "\n"; }
        catch (const exception& e) { cerr << "[ERROR DESCONOCIDO] " << e.what() << "\n"; }
    }
    return 0;
}