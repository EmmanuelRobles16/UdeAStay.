#include "Huesped.h"
#include "Reservacion.h"
//#include "Fecha.h"
//#include <iostream>

Huesped::Huesped(string doc, int antig, float punt) {
    documento = doc;
    antiguedad = antig;
    puntuacion = punt;
    capacidadReservas = 5;
    cantidadReservas = 0;
    reservaciones = new Reservacion*[capacidadReservas];
}

Huesped::~Huesped() {
    delete[] reservaciones;
}

string Huesped::getDocumento() {
    return documento;
}

float Huesped::getPuntuacion() {
    return puntuacion;
}

int Huesped::getAntiguedad() {
    return antiguedad;
}

