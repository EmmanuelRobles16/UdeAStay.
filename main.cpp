#include <cstdio>
#include "Alojamiento.h"
#include "Plataforma.h"
#include "archivos.h"
#include "Reservacion.h"
#include "Huesped.h"
#include "Anfitrion.h"

// ✅ Definiciones globales (visibles desde extern)
Reservacion** reservasHistoricas = nullptr;
int cantidadHistoricas = 0;

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
    Reservacion** reservasV = nullptr;
    int cantV = 0;
    Archivo::cargarReservasVigentes(reservasV, cantV, alojamientos, cantAl, huespedes, cantH);

    // 3) Cargo reservas históricas
    Archivo::cargarReservasHistoricas(reservasHistoricas, cantidadHistoricas, alojamientos, cantAl, huespedes, cantH);

    // 4) Autenticación de usuario
    ResultadoLogin res = Plataforma::autenticar(huespedes, cantH, anfitriones, cantA);

    // 5) Ejecución del menú según tipo de usuario
    if (res.tipo == TipoUsuario::Anfitrion) {
        Plataforma::runMenuAnfitrion(res.anfitrion, alojamientos, cantAl);
    } else if (res.tipo == TipoUsuario::Huesped) {
        Plataforma::runMenuHuesped(res.huesped, reservasV, cantV, reservasHistoricas, cantidadHistoricas);
    } else {
        std::printf("Acceso denegado o usuario no encontrado.\n");
    }

    return EXIT_SUCCESS;
}
