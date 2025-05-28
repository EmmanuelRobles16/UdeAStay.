#include "Huesped.h"
#include <cstring> // strlen, strcpy

// Constructor: copia cadenas y crea arreglo inicial
Huesped::Huesped(const char* doc, int antig, float punt, const char* pwd)
    : antiguedad(antig), puntuacion(punt), cantidadReservas(0), capacidadReservas(5)
{
    // Copiar documento a memoria dinámica
    int lenDoc = std::strlen(doc) + 1;
    documento = (char*)std::malloc(lenDoc);
    std::strcpy(documento, doc);

    // Copiar password a memoria dinámica
    int lenPwd = std::strlen(pwd) + 1;
    password = (char*)std::malloc(lenPwd);
    std::strcpy(password, pwd);

    // Crear arreglo de Reservacion* con malloc
    reservaciones = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidadReservas);
}

// Destructor: libera cadenas y arreglo
Huesped::~Huesped() {
    std::free(documento);
    std::free(password);
    std::free(reservaciones);
}


void Huesped::getDocumento(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", documento);
}

int Huesped::getAntiguedad() const {
    return antiguedad;
}

float Huesped::getPuntuacion() const {
    return puntuacion;
}

void Huesped::getPassword(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", password);
}

void Huesped::mostrarReservas() const {
    char resumen[256];
    for (int i = 0; i < cantidadReservas; ++i) {
        // Suponiendo Reservacion tiene método toResumen(char*, int)
        reservaciones[i]->toResumen(resumen, sizeof(resumen));
        std::printf("%s\n", resumen);
    }
}

void Huesped::agregarReserva(Reservacion* r) {
    if (cantidadReservas == capacidadReservas) {
        int nuevaCap = capacidadReservas * 2;
        reservaciones = (Reservacion**)std::realloc(reservaciones, sizeof(Reservacion*) * nuevaCap);
        capacidadReservas = nuevaCap;
    }
    reservaciones[cantidadReservas++] = r;
}
