#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
using namespace std;

class Anfitrion {
private:
    string documento;
    int antiguedad;
    float puntuacion;
    string* codigosAlojamientos;   // Códigos de alojamientos administrados
    int cantidadAlojamientos;
    int capacidadAlojamientos;

public:
    // Constructor: recibe documento, antigüedad, puntuación y array de códigos
    Anfitrion(const string& doc, int antig, float punt, string* codigos, int numCodigos);
    ~Anfitrion();

    // Getters
    string getDocumento() const;
    int getAntiguedad() const;
    float getPuntuacion() const;

    // Operaciones sobre la lista de códigos
    void agregarAlojamiento(const string& codigo);
    void mostrarAlojamientos() const;
};

#endif // ANFITRION_H
