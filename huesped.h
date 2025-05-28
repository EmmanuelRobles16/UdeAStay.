#ifndef HUESPED_H
#define HUESPED_H

#include <cstdio>   // printf, snprintf
#include <cstdlib>  // malloc, free, realloc
#include <cstring>  // strlen, strcpy
#include "Reservacion.h"

class Huesped {
private:
    char* documento;           // Cédula o pasaporte
    int antiguedad;            // Años en el sistema
    float puntuacion;          // Rating acumulado
    char* password;            // Contraseña

    // Arreglo dinámico de Reservacion*
    Reservacion** reservaciones;
    int cantidadReservas;
    int capacidadReservas;

public:
    // Constructor parametrizado
    Huesped(const char* doc, int antig, float punt, const char* pwd);

    // Constructor de copia (deep copy)
    Huesped(const Huesped& otro);

    // Operador de asignación (deep copy + self-check)
    Huesped& operator=(const Huesped& otro);

    // Destructor
    ~Huesped();

    // Getters C-style
    void getDocumento(char* buffer, int bufSize) const;
    int  getAntiguedad() const;
    float getPuntuacion() const;
    void getPassword(char* buffer, int bufSize) const;

    // Mostrar reservaciones usando printf
    void mostrarReservas() const;

    // Añadir una Reservacion*
    void agregarReserva(Reservacion* r);

    // Anula una reserva activa:
    //  - La quita del arreglo interno del huésped.
    //  - La quita del arreglo global de reservas vigentes.
    //  - Reescribe "reservas_vigentes.txt" excluyendo la línea.
    void anularReservacion(const char* codigoReserva, Reservacion**& reservasGlobal, int& cantidadGlobal);
};

#endif
