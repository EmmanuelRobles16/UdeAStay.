#include <cstdio>
#include "Alojamiento.h"
#include "Plataforma.h"
#include "archivos.h"
#include "Reservacion.h"
#include "Huesped.h"
#include "Anfitrion.h"

// ✅ Definiciones globales (visibles desde extern)
Reservacion** reservasHistoricas = nullptr;
int           cantidadHistoricas  = 0;
Reservacion** reservasVigentes   = nullptr;
int           cantidadVigentes   = 0;
int main() {
    // 1) Cargo datos desde archivos de texto
    Huesped** huespedes = nullptr;
    int cantH = 0;

    Anfitrion** anfitriones = nullptr;
    int cantA = 0;

    Alojamiento** alojamientos = nullptr;
    int cantAl = 0;

    Archivo::cargarHuespedes(huespedes, cantH);
    Archivo::cargarAnfitriones(anfitriones, cantA);
    Archivo::cargarAlojamientos(alojamientos, cantAl, anfitriones, cantA);

    // 2) Cargo reservas vigentes
    Archivo::cargarReservasVigentes(reservasVigentes, cantidadVigentes, alojamientos, cantAl, huespedes, cantH);

    // 3) Cargo reservas históricas
    Archivo::cargarReservasHistoricas(reservasHistoricas, cantidadHistoricas, alojamientos, cantAl, huespedes, cantH);

    // 4) Autenticación de usuario
    Plataforma plataforma;
    ResultadoLogin res = Plataforma::autenticar(huespedes, cantH, anfitriones, cantA);

    // 5) Ejecución del menú según tipo de usuario
    if (res.tipo == TipoUsuario::Anfitrion) {
        plataforma.runMenuAnfitrion(res.anfitrion,
                                    alojamientos, cantAl);
    }
    else if (res.tipo == TipoUsuario::Huesped) {
        plataforma.runMenuHuesped(res.huesped,
                                  reservasVigentes,   cantidadVigentes,
                                  reservasHistoricas, cantidadHistoricas,
                                  alojamientos,       cantAl);
    }
    else {
        std::printf("Acceso denegado o usuario no encontrado.\n");
    }

    return EXIT_SUCCESS;
}
