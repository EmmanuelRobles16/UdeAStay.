#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "Huesped.h"
#include "Anfitrion.h"
#include "Alojamiento.h"
#include "Reservacion.h"
#include "Fecha.h"
#include <cstdio>   // printf, snprintf
#include <cstdlib>  // malloc, free, realloc
#include <cstring>  // strlen, strcmp

// Indica el tipo de usuario autenticado
enum class TipoUsuario {
    Ninguno,
    Huesped,
    Anfitrion
};

// Resultado de autenticación: tipo y puntero al usuario
struct ResultadoLogin {
    TipoUsuario tipo;
    Huesped*   huesped;
    Anfitrion* anfitrion;
};

class Plataforma {
public:
    /**
     * Solicita credenciales por consola (documento y contraseña C-style),
     * busca en arreglos de Huesped* y Anfitrion* y retorna ResultadoLogin.
     */
    static ResultadoLogin autenticar(
        Huesped** huespedes, int cantidadHuespedes,
        Anfitrion** anfitriones, int cantidadAnfitriones
        );

    /**
     * Menú para Anfitrion autenticado: muestra opciones y lee con scanf.
     */
    void runMenuAnfitrion(Anfitrion*    anfitrion, Alojamiento** alojamientos,int cantidadAlojamientos
        );

    /**
     * Menú para Huesped autenticado: muestra opciones y lee con scanf.
     */
    static void runMenuHuesped(
        Huesped* huesped,
        Reservacion** reservas, int cantidadRes,
        Reservacion** historico, int cantidadHist,
        Alojamiento**   alojamientos,  int cantidadAl
        );

    static void buscarAlojamientosDisponibles(Alojamiento** alojamientos, int cantidadAlojamientos,
        const char* municipio,
        const Fecha& fechaEntrada, int duracion
        );
    static void reservarAlojamiento(Huesped* huesped,  Reservacion**&  reservas,   int& cantidadRes, Alojamiento**   alojamientos, int cantidadAl);
    static void actualizarHistorico(Anfitrion* anfitrion, Reservacion**&  reservasVig, int& cantidadVig, Reservacion**&  reservasHist, int& cantidadHist);
};


#endif
