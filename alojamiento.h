#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <cstdio>   // printf, snprintf
#include <cstdlib>  // malloc, free, realloc
#include <cstring>  // strlen, strcpy, strcmp
#include "Reservacion.h"

class Anfitrion;  // Declaración adelantada

class Alojamiento {
private:
    char* codigo;             // Código único del alojamiento
    char* nombre;             // Nombre del alojamiento
    char* tipo;               // Tipo (hotel, hostal, etc.)
    char* direccion;          // Dirección completa
    char* departamento;       // Departamento o estado
    char* municipio;          // Municipio o ciudad
    float precioPorNoche;     // Tarifa por noche

    // Amenidades como cadenas C en arreglo dinámico
    char** amenidades;
    int cantidadAmenidades;

    // Reservas asociadas (punteros a Reservacion)
    Reservacion** reservas;
    int cantidadReservas;
    int capacidadReservas;

    // Relación con anfitrión (puntero)
    Anfitrion* anfitrion;

public:
    // Constructor: copia cadenas C y prepara arreglos
    Alojamiento(const char* cod, const char* nom, const char* tip, const char* dir, const char* depto, const char* muni, float precio, char** amenArr, int cantAmen, Anfitrion*Anf);
    // Destructor: libera toda la memoria dinámica
    ~Alojamiento();

    // Getters C-style
    void getCodigo(char* buffer, int bufSize) const;
    void getNombre(char* buffer, int bufSize) const;
    void getTipo(char* buffer, int bufSize) const;
    void getDireccion(char* buffer, int bufSize) const;
    void getDepartamento(char* buffer, int bufSize) const;
    void getMunicipio(char* buffer, int bufSize) const;
    float getPrecioPorNoche() const;
    Anfitrion* getAnfitrion() const;
    int           getCantidadReservas() const;
    Reservacion*  getReserva(int idx) const;

    // Amenidades
    int getCantidadAmenidades() const;
    void mostrarAmenidades() const;

    // Reservas: agregar, anular y mostrar
    void agregarReserva(Reservacion* r);
    void anularReserva(const char* codRes);
    void mostrarReservas() const;

    bool estaDisponible(const Fecha& inicio, int duracion) const;
};

#endif
