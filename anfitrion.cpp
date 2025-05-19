#include "Anfitrion.h"
#include <iostream>

Anfitrion::Anfitrion(const string& doc, int antig, float punt,
                     const string& pwd, string* codigos, int numCodigos)
    : documento(doc), antiguedad(antig), puntuacion(punt),
    password(pwd), cantidadAlojamientos(numCodigos),
    capacidadAlojamientos(numCodigos)
{
    codigosAlojamientos = new string[capacidadAlojamientos];
    for (int i = 0; i < cantidadAlojamientos; ++i)
        codigosAlojamientos[i] = codigos[i];
}

Anfitrion::~Anfitrion() {
    delete[] codigosAlojamientos;
}

string Anfitrion::getDocumento() const { return documento; }
int    Anfitrion::getAntiguedad() const { return antiguedad; }
float  Anfitrion::getPuntuacion() const { return puntuacion; }
string Anfitrion::getPassword() const  { return password; }  // ← impl.


void Anfitrion::mostrarAlojamientos() const {
    cout << "Alojamientos del anfitrión " << documento << ":\n";
    for (int i = 0; i < cantidadAlojamientos; ++i)
        cout << "  - " << codigosAlojamientos[i] << "\n";
}
