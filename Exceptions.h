//
// Created by luis1 on 12/06/2026.
//

#ifndef PROYECTOINTEGRADOR_EXCEPTIONS_H
#define PROYECTOINTEGRADOR_EXCEPTIONS_H

#include <stdexcept>
#include <string>
using namespace std;

//exceptions
class CalificacionInvalidaError : public runtime_error {
public:
    explicit CalificacionInvalidaError (const string&msg) : runtime_error(msg){}
};

class VideoNoEncontradoError : public runtime_error {
public:
    explicit VideoNoEncontradoError (const string&msg) : runtime_error(msg){}
};

class ArchivoInvalidoError : public runtime_error {
public:
    explicit ArchivoInvalidoError (const string&msg) : runtime_error(msg){}
};

class GeneroInvalidoError : public runtime_error {
public:
    explicit GeneroInvalidoError (const string&msg) : runtime_error(msg){}
};


#endif //PROYECTOINTEGRADOR_EXCEPTIONS_H