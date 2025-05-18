#include "anfitrion.h"
#include <iostream>

Anfitrion::Anfitrion(const string& doc, int antig, float punt, string* codigos, int numCodigos)
    : documento(doc), antiguedad(antig), puntuacion(punt),
    cantidadAlojamientos(numCodigos), capacidadAlojamientos(numCodigos)
{
    // Copiar los códigos iniciales
    codigosAlojamientos = new string[capacidadAlojamientos];
    for (int i = 0; i < numCodigos; ++i) {
        codigosAlojamientos[i] = codigos[i];
    }
}

Anfitrion::~Anfitrion() {
    delete[] codigosAlojamientos;
}

string Anfitrion::getDocumento() const {
    return documento;
}

int Anfitrion::getAntiguedad() const {
    return antiguedad;
}

float Anfitrion::getPuntuacion() const {
    return puntuacion;
}
