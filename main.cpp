#include <iostream>
#include "Archivos.h"
#include "Anfitrion.h"

using namespace std;

int main() {
    Anfitrion** anfitriones = nullptr;
    int cantidadAnfitriones = 0;

    // 1) Llama a tu método de carga
    Archivo::cargarAnfitriones(anfitriones, cantidadAnfitriones);

    // 2) Valida que hayan llegado datos
    cout << "Cantidad de anfitriones cargados: "
         << cantidadAnfitriones << "\n";

    for (int i = 0; i < cantidadAnfitriones; ++i) {
        cout << "- Documento: "   << anfitriones[i]->getDocumento()
        << " | Antigüedad: " << anfitriones[i]->getAntiguedad()
        << " | Puntuación: "  << anfitriones[i]->getPuntuacion() << "\n";
    }

    // 3) Liberar memoria
    for (int i = 0; i < cantidadAnfitriones; ++i)
        delete anfitriones[i];
    delete[] anfitriones;

    return 0;
}
