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
/*
void Huesped::agregarReserva(Reservacion* r) {
    if (cantidadReservas == capacidadReservas) {
        capacidadReservas *= 2;
        Reservacion** nuevo = new Reservacion*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i)
            nuevo[i] = reservaciones[i];
        delete[] reservaciones;
        reservaciones = nuevo;
    }
    reservaciones[cantidadReservas++] = r;
}

bool Huesped::tieneConflicto(Fecha entrada, int duracion) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservaciones[i]->seCruzaCon(entrada, duracion)) {
            return true;
        }
    }
    return false;
}

void Huesped::mostrarReservas() {
    for (int i = 0; i < cantidadReservas; ++i) {
        cout << reservaciones[i]->getResumen() << endl;
    }
}

void Huesped::eliminarReservaPorCodigo(string codigo) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservaciones[i]->getCodigo() == codigo) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservaciones[j] = reservaciones[j + 1];
            cantidadReservas--;
            break;
        }
    }
}
*/
