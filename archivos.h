#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <cstdio>   // fopen, fgets, sscanf, printf
#include <cstdlib>  // malloc, free, realloc, atoi, atof
#include <cstring>  // strlen, strcpy, strcmp, strchr

class Huesped;
class Anfitrion;
class Alojamiento;
class Reservacion;

// Clase utilitaria para gestión de cargas desde archivos de texto
class Archivo {
public:
    // Divide 'str' por 'delimitador', devuelve array de tokens C y setea totalPartes
    static char** split(const char* str, char delimitador, int& totalPartes);

    // Función para eliminar saltos de línea '\n' o '\r' al final de una cadena C
    static void trimNewline(char* s);


    // Carga arrays dinámicos de objetos, retornando punteros y cantidades
    static void cargarHuespedes(Huesped**& huespedes, int& cantidad);
    static void cargarAnfitriones(Anfitrion**& anfitriones, int& cantidad);
    static void cargarAlojamientos(Alojamiento**& alojamientos, int& cantidad, Anfitrion** anfitriones, int cantidadAnf);
    static void cargarReservasVigentes(Reservacion**& reservas, int& cantidad,Alojamiento** alojamientos, int cantidadAloj,Huesped** huespedes, int cantidadHues);
    static void cargarReservasHistoricas(Reservacion**& reservas, int& cantidad, Alojamiento** alojamientos, int cantidadAloj, Huesped** huespedes, int cantidadHues);
};

#endif
