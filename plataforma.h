#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <string>
#include "Huesped.h"
#include "Anfitrion.h"
#include "alojamiento.h"


//simplemente una enumaracion la cual nos dice que tipo es del usuario que esta entrand
enum class TipoUsuario {
    Ninguno,
    Huesped,
    Anfitrion
};

struct ResultadoLogin {
    TipoUsuario tipo;
    Huesped*   huesped;
    Anfitrion* anfitrion;
};

class Plataforma {
public:
    /** Solicita documento y contraseña por consola, busca en ambos arrays y retorna un ResultadoLogin con el tipo y el puntero al usuario. */
    static ResultadoLogin autenticar(Huesped** huespedes, int cantidadHuespedes, Anfitrion** anfitriones, int cantidadAnfitriones);

    /** Ejecuta el menú de opciones para un anfitrión autenticado, hasta que elija salir. */
    static void runMenuAnfitrion(Anfitrion* anfitrion, Alojamiento** alojamientos, int cantidadAlojamientos);
};

#endif // PLATAFORMA_H
