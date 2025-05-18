#include <iostream>
#include "Archivos.h"
#include "Anfitrion.h"
#include "huesped.h"
#include "alojamiento.h"

int main() {
    // Primero carga anfitriones (suponiendo que ya funciona)
    Anfitrion** anfitriones = nullptr;
    int cantidadAnfitriones = 0;
    Archivo::cargarAnfitriones(anfitriones, cantidadAnfitriones);

    // Ahora carga alojamientos
    Alojamiento** alojamientos = nullptr;
    int cantidadAlojamientos = 0;
    Archivo::cargarAlojamientos(alojamientos,
                                cantidadAlojamientos,
                                anfitriones,
                                cantidadAnfitriones);

    // Validación
    cout << "Cantidad de alojamientos cargados: "
         << cantidadAlojamientos << "\n\n";

    for (int i = 0; i < cantidadAlojamientos; ++i) {
        auto a = alojamientos[i];
        cout << "- Código: "     << a->getCodigo()
             << " | Nombre: "    << a->getNombre()
             << " | Anfitrión: " << a->getAnfitrion()->getDocumento()
             << " | Precio: "    << a->getPrecioPorNoche()
             << " | Amenidades: ";
        for (int j = 0; j < a->getCantidadAmenidades(); ++j) {
            cout << a->getAmenidades()[j]
                 << (j+1 < a->getCantidadAmenidades() ? ", " : "");
        }
        cout << "\n";
    }

    // Liberar memoria
    for (int i = 0; i < cantidadAlojamientos; ++i)
        delete alojamientos[i];
    delete[] alojamientos;
    for (int i = 0; i < cantidadAnfitriones; ++i)
        delete anfitriones[i];
    delete[] anfitriones;

    return 0;
}
