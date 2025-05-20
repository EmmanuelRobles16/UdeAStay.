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
    // 2) Autenticación
    auto res = Plataforma::autenticar(huespedes, cantH, anfitriones, cantA);
    if (res.tipo == TipoUsuario::Anfitrion) {
        // 3) Todo el menú de anfitrión dentro de Plataforma
        Plataforma::runMenuAnfitrion(res.anfitrion, alojamientos, cantAl);
    } else {
        cout << "Acceso denegado o no implementado.\n";
    }
    return 0;
}

