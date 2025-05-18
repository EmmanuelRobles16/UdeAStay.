#ifndef HUESPED_H
#define HUESPED_H

#include <string>
using namespace std;

// Declaración anticipada
class Reservacion;
class Fecha;

class Huesped {
private:
    string documento;
    int antiguedad;
    float puntuacion;
    Reservacion** reservaciones;
    int cantidadReservas;
    int capacidadReservas;

public:
    Huesped(string doc, int antig, float punt);
    ~Huesped();

    string getDocumento();
    float getPuntuacion();
    int getAntiguedad();

    void agregarReserva(Reservacion* r);
    bool tieneConflicto(Fecha entrada, int duracion);
    void mostrarReservas();

    // Métodos de mantenimiento
    void eliminarReservaPorCodigo(string codigo);
};

#endif
