#ifndef HUESPED_H
#define HUESPED_H

#include <cstdio>  // printf, sprintf
#include <cstdlib> // malloc, free, atoi
#include "Reservacion.h"

class Huesped {
private:
    char* documento;           // Cédula o pasaporte
    int antiguedad;            // Años en el sistema
    float puntuacion;          // Rating acumulado
    char* password;            // Contraseña

    // Puntero a arreglo dinámico de Reservacion*
    Reservacion** reservaciones;
    int cantidadReservas;
    int capacidadReservas;

public:
    // Constructor y destructor
    Huesped(const char* doc, int antig, float punt, const char* pwd);
    ~Huesped();

    // Getters con buffers C-style
    void getDocumento(char* buffer, int bufSize) const;
    int  getAntiguedad() const;
    float getPuntuacion() const;
    void getPassword(char* buffer, int bufSize) const;

    // Mostrar reservaciones usando printf
    void mostrarReservas() const;

    // Añadir una Reservacion*
    void agregarReserva(Reservacion* r);
    /**
     * Anula una reserva activa:
     *  - La quita del arreglo interno del huésped. - La quita del arreglo global de reservas vigentes.  - Reescribe el archivo "reservas_vigentes.txt" excluyendo dicha línea.
     */
    void anularReservacion(const char* codigoReserva, Reservacion**& reservasGlobal, int& cantidadGlobal);
};

#endif
