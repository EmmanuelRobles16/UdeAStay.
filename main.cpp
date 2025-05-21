#include <iostream>
#include "archivos.h"
#include "Huesped.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "reservacion.h"
#include "Fecha.h"
#include "plataforma.h"

using namespace std;

int main() {
    // 1) Cargo datos
    Huesped** huespedes; int cantH;
    Anfitrion** anfitriones; int cantA;
    Alojamiento** alojamientos; int cantAl;
    Archivo::cargarHuespedes(huespedes, cantH);
    Archivo::cargarAnfitriones(anfitriones, cantA);
    Archivo::cargarAlojamientos(alojamientos, cantAl, anfitriones, cantA);
    // 2) Cargar también las reservas vigentes
    Reservacion** reservasV     = nullptr;  // array global de reservas
    int           cantV         = 0;
    Archivo::cargarReservasVigentes(reservasV, cantV,alojamientos, cantAl, huespedes,cantH);
    Reservacion** historico = nullptr;
    int cantidadHistorico = 0;
    Archivo::cargarReservasHistoricas(historico, cantidadHistorico, alojamientos, cantA, huespedes, cantH);
    // 2) Autenticación
    auto res = Plataforma::autenticar(huespedes, cantH, anfitriones, cantA);
    // 3) Despachar al menú correcto
    if (res.tipo == TipoUsuario::Huesped) {
        Plataforma::runMenuHuesped(res.huesped, reservasV, cantV);
    }
    else if (res.tipo == TipoUsuario::Anfitrion) {
        Plataforma::runMenuAnfitrion(res.anfitrion, alojamientos, cantA, reservasV, cantV, historico, cantidadHistorico);
    }
    else {
        cout << "\nCredenciales inválidas. Saliendo...\n";
    }
    return 0;
    //
}

