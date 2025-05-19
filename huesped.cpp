#include "Huesped.h"
#include "Reservacion.h"
#include "Fecha.h"
#include <iostream>

Huesped::Huesped(const string& doc, int antig, float punt, const string& pwd)
    : documento(doc), antiguedad(antig), puntuacion(punt), password(pwd),
    cantidadReservas(0), capacidadReservas(5)
{
    reservaciones = new Reservacion*[capacidadReservas];
}

Huesped::~Huesped() {
    delete[] reservaciones;
}

string Huesped::getDocumento() const { return documento; }
float  Huesped::getPuntuacion() const { return puntuacion; }
int    Huesped::getAntiguedad() const { return antiguedad; }
string Huesped::getPassword() const  { return password; }  // ← implementación



void Huesped::mostrarReservas() const {
    for (int i = 0; i < cantidadReservas; ++i)
        cout << reservaciones[i]->getResumen() << endl;
}
