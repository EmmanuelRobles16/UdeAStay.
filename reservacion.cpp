#include "Reservacion.h"
#include "Huesped.h"
#include "Alojamiento.h"
#include <cstring>

// Constructor: copia cadenas a memoria dinámica para evitar STL
Reservacion::Reservacion(const char* cod, const Fecha& fEntrada, int dur, const char* metodo, const Fecha& fPago, float monto_,const char* nota, Alojamiento* alo, Huesped* hue)
    : fechaEntrada(fEntrada), duracion(dur), fechaPago(fPago), monto(monto_), alojamiento(alo), huesped(hue)
{
    // std::strlen: obtiene la longitud de la cadena C (sin contar '\0')
    // Se usa para calcular cuántos bytes asignar con std::malloc
    int lenCod = std::strlen(cod) + 1;
    // std::malloc: reserva lenCod bytes en el heap para almacenar la cadena
    codigo = (char*)std::malloc(lenCod);
    // strcpy copia el contenido de cod al buffer recién asignado
    std::strcpy(codigo, cod);

    int lenMet = std::strlen(metodo) + 1;
    metodoPago = (char*)std::malloc(lenMet);  // Reservar memoria para método de pago
    std::strcpy(metodoPago, metodo);

    int lenNote = std::strlen(nota) + 1;
    anotacion = (char*)std::malloc(lenNote);   // Reservar memoria para anotaciones
    std::strcpy(anotacion, nota);
}

// Destructor: libera cada bloque asignado con std::malloc
Reservacion::~Reservacion() {
    // std::free libera memoria previamente reservada con std::malloc
    std::free(codigo);
    std::free(metodoPago);
    std::free(anotacion);
}

void Reservacion::getCodigo(char* buffer, int bufSize) const {
    // std::snprintf: escribe de forma segura (hasta bufSize-1) en buffer
    std::snprintf(buffer, bufSize, "%s", codigo);
}

Fecha Reservacion::getFechaEntrada() const { return fechaEntrada; }
int   Reservacion::getDuracion() const       { return duracion; }

void Reservacion::getMetodoPago(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", metodoPago);
}

Fecha Reservacion::getFechaPago() const      { return fechaPago; }
float Reservacion::getMonto() const          { return monto; }

void Reservacion::getAnotacion(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", anotacion);
}

Alojamiento* Reservacion::getAlojamiento() const { return alojamiento; }
Huesped*     Reservacion::getHuesped() const     { return huesped; }

Fecha Reservacion::fechaSalida() const {
    Fecha salida = fechaEntrada;
    salida.sumarDias(duracion);
    return salida;
}

void Reservacion::toResumen(char* buffer, int bufSize) const {
    char inicio[12], fin[12];
    fechaEntrada.toShortString(inicio);
    fechaSalida().toShortString(fin);
    std::snprintf(buffer, bufSize, "%s : %s - %s", codigo, inicio, fin);
}

void Reservacion::toComprobante(char* buffer, int bufSize) const {
    char ent[64], sal[64], doc[64], alo[64];
    fechaEntrada.toLongString(ent);
    fechaSalida().toLongString(sal);
    huesped->getDocumento(doc, sizeof(doc));
    alojamiento->getCodigo(alo, sizeof(alo));
    std::snprintf(buffer, bufSize, "Reserva %s | Huésped: %s | Alojamiento: %s | Entrada: %s | Salida: %s | Monto: %.2f", codigo, doc, alo, ent, sal, monto );
}

bool Reservacion::seCruzaCon(const Fecha& otraEntrada, int otraDuracion) const {
    Fecha miSalida   = fechaSalida();
    Fecha otraSalida = otraEntrada;
    otraSalida.sumarDias(otraDuracion);
    // std::strcmp: compara cadenas para verificar igualdad (no se usa aquí)
    // Se cruzan si los rangos de fechas no están completamente antes uno del otro
    return !(miSalida.esAnterior(otraEntrada) || otraSalida.esAnterior(fechaEntrada));
}

bool Reservacion::esDelHuesped(const char* doc) const {
    char actual[64];
    huesped->getDocumento(actual, sizeof(actual));
    // std::strcmp devuelve 0 si las cadenas son idénticas
    return std::strcmp(actual, doc) == 0;
}
