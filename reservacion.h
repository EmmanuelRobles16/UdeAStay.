#ifndef RESERVACION_H
#define RESERVACION_H

#include "Fecha.h"
#include <cstdio>   // snprintf para escribir cadenas formateadas en buffers C
#include <cstdlib>  // malloc, free para gestión de memoria en C

class Alojamiento;  // Declaración adelantada
class Huesped;

class Reservacion {
private:
    char* codigo;            // Código único de la reserva (cadena C)
    Fecha fechaEntrada;      // Fecha de inicio de la reserva
    int duracion;            // Duración en días
    char* metodoPago;        // Método de pago (efectivo, tarjeta, etc.)
    Fecha fechaPago;         // Fecha de realización del pago
    float monto;             // Monto total pagado
    char* anotacion;         // Comentarios adicionales
    Alojamiento* alojamiento; // Puntero al alojamiento asociado
    Huesped* huesped;        // Puntero al huésped asociado

public:
    // Constructor: copia cadenas C a memoria dinámica
    Reservacion(const char* cod, const Fecha& fEntrada,  int dur, const char* metodo, const Fecha& fPago, float monto, const char* nota, Alojamiento* alo, Huesped* hue);

    // Destructor: libera memoria asignada con std::malloc usando std::free
    ~Reservacion();

    // Getters C-style: llenan buffer de tamaño bufSize
    void getCodigo(char* buffer, int bufSize) const;
    Fecha getFechaEntrada() const;
    int   getDuracion() const;
    void  getMetodoPago(char* buffer, int bufSize) const;
    Fecha getFechaPago() const;
    float getMonto() const;
    void  getAnotacion(char* buffer, int bufSize) const;
    Alojamiento* getAlojamiento() const;
    Huesped*     getHuesped() const;

    // Devuelve fecha de salida calculada: fechaEntrada + duracion
    Fecha fechaSalida() const;

    // Crea resumen corto: "COD : YYYY-MM-DD - YYYY-MM-DD"
    void toResumen(char* buffer, int bufSize) const;

    // Crea comprobante detallado en buffer (texto formateado)
    void toComprobante(char* buffer, int bufSize) const;

    // Comprueba solapamiento con otro rango de fechas
    bool seCruzaCon(const Fecha& otraEntrada, int otraDuracion) const;

    // Verifica si la reservación pertenece a huésped con documento doc
    bool esDelHuesped(const char* doc) const;
};

#endif
