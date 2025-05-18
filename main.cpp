#include <iostream>
#include "archivos.h"
#include "huesped.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "reservacion.h"
#include "Fecha.h"

using namespace std;

int main() {
    // 1) Cargar huéspedes
    Huesped** huespedes = nullptr;
    int cantidadHuespedes = 0;
    Archivo::cargarHuespedes(huespedes, cantidadHuespedes);
    cout << "Huéspedes cargados: " << cantidadHuespedes << "\n";
    for (int i = 0; i < cantidadHuespedes; ++i) {
        cout << "  - " << huespedes[i]->getDocumento()
        << " | antigüedad: " << huespedes[i]->getAntiguedad()
        << " | puntuación: " << huespedes[i]->getPuntuacion() << "\n";
    }
    cout << "\n";

    // 2) Cargar anfitriones
    Anfitrion** anfitriones = nullptr;
    int cantidadAnfitriones = 0;
    Archivo::cargarAnfitriones(anfitriones, cantidadAnfitriones);
    cout << "Anfitriones cargados: " << cantidadAnfitriones << "\n";
    for (int i = 0; i < cantidadAnfitriones; ++i) {
        cout << "  - " << anfitriones[i]->getDocumento()
        << " | antigüedad: " << anfitriones[i]->getAntiguedad()
        << " | puntuación: " << anfitriones[i]->getPuntuacion() << "\n";
        anfitriones[i]->mostrarAlojamientos();
    }
    cout << "\n";

    // 3) Cargar alojamientos
    Alojamiento** alojamientos = nullptr;
    int cantidadAlojamientos = 0;
    Archivo::cargarAlojamientos(
        alojamientos, cantidadAlojamientos,
        anfitriones, cantidadAnfitriones
        );
    cout << "Alojamientos cargados: " << cantidadAlojamientos << "\n";
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        auto a = alojamientos[i];
        cout << "  - " << a->getCodigo()
             << " | " << a->getNombre()
             << " | anfitrión: " << a->getAnfitrion()->getDocumento() << "\n";
    }
    cout << "\n";

    // 4) Cargar reservas vigentes
    Reservacion** vigentes = nullptr;
    int cantidadVigentes = 0;
    Archivo::cargarReservasVigentes(
        vigentes, cantidadVigentes,
        alojamientos, cantidadAlojamientos,
        huespedes, cantidadHuespedes
        );
    cout << "Reservas vigentes cargadas: " << cantidadVigentes << "\n";
    for (int i = 0; i < cantidadVigentes; ++i) {
        cout << "  - " << vigentes[i]->getCodigo()
        << " | huésped: " << vigentes[i]->getHuesped()->getDocumento()
        << " | entrada: " << vigentes[i]->getFechaEntrada().toStringCorto()
        << "\n";
    }
    cout << "\n";

    // 5) Cargar reservas históricas
    Reservacion** historico = nullptr;
    int cantidadHistorico = 0;
    Archivo::cargarReservasHistoricas(historico, cantidadHistorico,alojamientos, cantidadAlojamientos, huespedes, cantidadHuespedes);
    cout << "Reservas históricas cargadas: " << cantidadHistorico << "\n";
    for (int i = 0; i < cantidadHistorico; ++i) {
        cout << "  - " << historico[i]->getCodigo()
        << " | huésped: " << historico[i]->getHuesped()->getDocumento()
        << " | entrada: " << historico[i]->getFechaEntrada().toStringCorto()
        << "\n";
    }
    cout << "\n";

    // 6) Liberar memoria
    for (int i = 0; i < cantidadHuespedes; ++i) delete huespedes[i];
    delete[] huespedes;

    for (int i = 0; i < cantidadAnfitriones; ++i) delete anfitriones[i];
    delete[] anfitriones;

    for (int i = 0; i < cantidadAlojamientos; ++i) delete alojamientos[i];
    delete[] alojamientos;

    for (int i = 0; i < cantidadVigentes; ++i) delete vigentes[i];
    delete[] vigentes;

    for (int i = 0; i < cantidadHistorico; ++i) delete historico[i];
    delete[] historico;

    return 0;
}
