#ifndef archivos_H
#define archivos_H

#include <string>
using namespace std;

// Declaraciones anticipadas
class Huesped;
class Anfitrion;
class Alojamiento;
class Reservacion;

class ArchivoManager {
public:
    // Carga
    static void cargarHuespedes(Huesped**& huespedes, int& cantidad);
    static void cargarAnfitriones(Anfitrion**& anfitriones, int& cantidad);
    static void cargarAlojamientos(Alojamiento**& alojamientos, int& cantidad, Anfitrion** anfitriones, int cantidadAnfitriones);
    static void cargarReservasVigentes(Reservacion**& reservas, int& cantidad, Alojamiento** alojamientos, int cantidadAloj, Huesped** huespedes, int cantidadHues);

    // Utilidad
    static string* split(const string& str, char delimitador, int& totalPartes);
};

#endif
