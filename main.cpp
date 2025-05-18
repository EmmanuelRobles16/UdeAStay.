#include <iostream>
#include "archivos.h"
#include "Huesped.h"
using namespace std;

int main() {
    Huesped** huespedes;
    int cantidadHuespedes = 0;

    // Cargar desde el archivo
    ArchivoManager::cargarHuespedes(huespedes, cantidadHuespedes);

    // Validación 1: archivo abierto correctamente (ya se muestra en consola si no)
    // Validación 2: cantidad correcta
    cout << "Cantidad de huespedes cargados: " << cantidadHuespedes << endl;

    // Validación 3: mostrar documentos para verificar carga
    for (int i = 0; i < cantidadHuespedes; i++) {
        cout << "- " << huespedes[i]->getDocumento() << " | antiguedad: "
             << huespedes[i]->getAntiguedad() << " | puntuacion: "
             << huespedes[i]->getPuntuacion() << endl;
    }

    // Liberar memoria
    for (int i = 0; i < cantidadHuespedes; i++) {
        delete huespedes[i];
    }
    delete[] huespedes;

    return 0;
}
