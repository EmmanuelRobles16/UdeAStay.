#include "Reservacion.h"
#include "Alojamiento.h"
#include "Huesped.h"
#include <sstream>

using std::string;
using std::ostringstream;

// Constructor
Reservacion::Reservacion(const string& codigo, const Fecha& fechaEntrada, int duracion, const string& metodoPago, const Fecha& fechaPago, float monto, const string& anotacion, Alojamiento* alojamiento, Huesped* huesped)
    : codigo(codigo),
    fechaEntrada(fechaEntrada),
    duracion(duracion),
    metodoPago(metodoPago),
    fechaPago(fechaPago),
    monto(monto),
    anotacion(anotacion),
    alojamiento(alojamiento),
    huesped(huesped)
{}

// Getters
string Reservacion::getCodigo() const             { return codigo; }
Fecha  Reservacion::getFechaEntrada() const      { return fechaEntrada; }
int    Reservacion::getDuracion() const          { return duracion; }
string Reservacion::getMetodoPago() const        { return metodoPago; }
Fecha  Reservacion::getFechaPago() const         { return fechaPago; }
float  Reservacion::getMonto() const             { return monto; }
string Reservacion::getAnotacion() const         { return anotacion; }
Alojamiento* Reservacion::getAlojamiento() const { return alojamiento; }
Huesped*     Reservacion::getHuesped() const     { return huesped; }

// Calcula fecha de salida
Fecha Reservacion::fechaSalida() const {
    return fechaEntrada.sumarDias(duracion);
}

// Genera comprobante detallado
string Reservacion::comprobante() const {
    ostringstream ss;
    ss << "Reserva " << codigo
       << " | Huésped: " << huesped->getDocumento()
       << " | Alojamiento: " << alojamiento->getCodigo()
       << " | Entrada: " << fechaEntrada.toStringLargo()
       << " | Salida: "  << fechaSalida().toStringLargo()
       << " | Monto: "   << monto;
    return ss.str();
}

// Resumen breve para listados
string Reservacion::getResumen() const {
    ostringstream ss;
    ss << codigo << ": "
       << fechaEntrada.toStringCorto()
       << " - " << fechaSalida().toStringCorto();
    return ss.str();
}

// ¿Se cruza con otro rango?
bool Reservacion::seCruzaCon(const Fecha& otraEntrada, int otraDuracion) const {
    Fecha otraSalida = otraEntrada.sumarDias(otraDuracion);
    Fecha thisSalida = fechaSalida();
    // No se cruzan si uno sale antes de que entre el otro
    if (thisSalida.esAnterior(otraEntrada) || otraSalida.esAnterior(fechaEntrada))
        return false;
    return true;
}

// ¿Pertenece esta resrevacion a este huésped?
bool Reservacion::esDelHuesped(const string& doc) const {
    return huesped->getDocumento() == doc;
}
