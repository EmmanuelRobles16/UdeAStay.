#ifndef HUESPED_H
#define HUESPED_H

#include <string>
using namespace std;

class Reservacion;
class Fecha;

class Huesped {
private:
    string documento;
    int antiguedad;
    float puntuacion;
    string password;              // ← nuevo atributo
    Reservacion** reservaciones;
    int cantidadReservas;
    int capacidadReservas;

public:
    // Ahora recibe también la contraseña
    Huesped(const string& doc, int antig, float punt, const string& pwd);
    ~Huesped();

    string getDocumento() const;
    float getPuntuacion() const;
    int getAntiguedad() const;
    string getPassword() const;   // ← nuevo getter

    void agregarReserva(Reservacion* r);
    bool tieneConflicto(const Fecha& entrada, int duracion) const;
    void mostrarReservas() const;
    void eliminarReservaPorCodigo(const string& codigo);
};

#endif // HUESPED_H
