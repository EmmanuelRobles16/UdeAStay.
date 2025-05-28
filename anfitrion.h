// Anfitrion.h
#ifndef ANFITRION_H
#define ANFITRION_H

#include <cstdio>   // printf, snprintf
#include <cstdlib>  // malloc, free, realloc
#include <cstring>  // strlen, strcpy, strcmp
#include "Alojamiento.h"

class Anfitrion {
private:
    char* documento;            // Cédula o pasaporte (cadena C)
    int   antiguedad;           // Años en el sistema
    float puntuacion;           // Rating acumulado
    char* password;             // Contraseña (cadena C)

    // Códigos de alojamientos gestionados por el anfitrión
    char** codigosAlojamientos; // Arreglo de cadenas C dinámico
    int    cantidadAlojamientos;// Número actual de códigos
    int    capacidadAlojamientos;// Capacidad del arreglo

public:
    // Constructor y destructor
    Anfitrion(const char* doc, int antig, float punt, const char* pwd,
              const char** codigos, int numCodigos);
    ~Anfitrion();

    // Getters C-style
    void  getDocumento(char* buffer, int bufSize) const;
    int   getAntiguedad() const;
    float getPuntuacion() const;
    void  getPassword(char* buffer, int bufSize) const;

    // Gestión de alojamientos
    void agregarAlojamiento(const char* codigo);
    void mostrarAlojamientos() const;

    // Muestra alojamientos y sus reservas:
    //   historicas=false → reservas vigentes
    //   historicas=true  → reservas históricas
    void mostrarAlojamientosYReservas(Alojamiento** todosAlojs, int total, bool historicas = false) const;

    // Anula una reservación por código
    void anularReservacion(const char* codigoReserva, Reservacion**& reservasVigentes, int& cantidadVigentes) const;
    void mostrarReservasPorRango(Alojamiento** todosAlojs, int total, const Fecha& desde,const Fecha& hasta) const;

};

#endif
