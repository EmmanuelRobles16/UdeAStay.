#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
using namespace std;

class Alojamiento;

class Anfitrion {
private:
    string documento;
    int antiguedad;
    float puntuacion;
    string password;              // ← nuevo atributo
    string* codigosAlojamientos;
    int cantidadAlojamientos;
    int capacidadAlojamientos;

public:
    // Añadimos el parámetro pwd
    Anfitrion(const string& doc, int antig, float punt,
              const string& pwd, string* codigos, int numCodigos);
    ~Anfitrion();

    string getDocumento() const;
    int    getAntiguedad() const;
    float  getPuntuacion() const;
    string getPassword() const;   // ← nuevo getter

    void agregarAlojamiento(const string& codigo);
    void mostrarAlojamientos() const;
    // Muestra alojamientos y sus reservas activas
    void mostrarAlojamientosYReservas(Alojamiento** todos, int cantidadTodos) const;
};

#endif // ANFITRION_H
