#include "Alojamiento.h"
#include "Reservacion.h"
#include <iostream>

// Constructor
Alojamiento::Alojamiento(const string& codigo, const string& nombre, const string& tipo, const string& direccion, const string& departamento, const string& municipio, float precioPorNoche, string* amenidadesArr, int cantidadAmenArr, Anfitrion* anfitrion)
    : codigo(codigo),
    nombre(nombre),
    tipo(tipo),
    direccion(direccion),
    departamento(departamento),
    municipio(municipio),
    precioPorNoche(precioPorNoche),
    cantidadAmenidades(cantidadAmenArr),
    cantidadReservas(0),
    capacidadReservas(5),
    anfitrion(anfitrion)
{
    // Copiar amenidades
    amenidades = new string[cantidadAmenidades];
    for (int i = 0; i < cantidadAmenidades; ++i) {
        amenidades[i] = amenidadesArr[i];
    }

    // Inicializar lista de reservas
    reservas = new Reservacion*[capacidadReservas];
}

// Destructor
Alojamiento::~Alojamiento() {
    delete[] amenidades;
    delete[] reservas;
}

// Getters
const string& Alojamiento::getCodigo() const { return codigo; }
const string& Alojamiento::getNombre() const { return nombre; }
const string& Alojamiento::getTipo() const { return tipo; }
const string& Alojamiento::getDireccion() const { return direccion; }
const string& Alojamiento::getDepartamento() const { return departamento; }
const string& Alojamiento::getMunicipio() const { return municipio; }
float Alojamiento::getPrecioPorNoche() const { return precioPorNoche; }
Anfitrion* Alojamiento::getAnfitrion() const { return anfitrion; }
string* Alojamiento::getAmenidades() const { return amenidades; }
int Alojamiento::getCantidadAmenidades() const { return cantidadAmenidades; }
/*
 *
 * por temas de preguntas propias estas funciones todavia no se van a usar porque primero quiero saber como formular la estructura de disponibilidad
// Agregar una reserva (sin tocar disponibilidad aún)
void Alojamiento::agregarReserva(Reservacion* r) {
    if (cantidadReservas == capacidadReservas) {
        capacidadReservas *= 2;
        Reservacion** tmp = new Reservacion*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i) {
            tmp[i] = reservas[i];
        }
        delete[] reservas;
        reservas = tmp;
    }
    reservas[cantidadReservas++] = r;
}

// Anular reserva y ajustar la lista (sin liberar días de calendario)
void Alojamiento::anularReserva(const string& codigoReserva) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i]->getCodigo() == codigoReserva) {
            // Desplazar elementos hacia atrás
            for (int j = i; j < cantidadReservas - 1; ++j) {
                reservas[j] = reservas[j + 1];
            }
            cantidadReservas--;
            break;
        }
    }
}
*/
