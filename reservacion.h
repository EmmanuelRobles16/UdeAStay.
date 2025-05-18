#ifndef RESERVACION_H
#define RESERVACION_H

#include <string>
#include "Fecha.h"
class Alojamiento;
class Huesped;

class Reservacion {
private:
    std::string codigo;
    Fecha fechaEntrada;
    int duracion;
    std::string metodoPago;
    Fecha fechaPago;
    float monto;
    std::string anotacion;
    Alojamiento* alojamiento;
    Huesped* huesped;

public:
    Reservacion(const std::string& codigo,const Fecha& fechaEntrada, int duracion,const std::string& metodoPago,const Fecha& fechaPago, float monto,  const std::string& anotacion, Alojamiento* alojamiento, Huesped* huesped);

    // Getters
    std::string getCodigo() const;
    Fecha getFechaEntrada() const;
    int getDuracion() const;
    std::string getMetodoPago() const;
    Fecha getFechaPago() const;
    float getMonto() const;
    std::string getAnotacion() const;
    Alojamiento* getAlojamiento() const;
    Huesped* getHuesped() const;

    // Utilidades
    Fecha fechaSalida() const;
    std::string comprobante() const;
    std::string getResumen() const;
    bool seCruzaCon(const Fecha& otraEntrada, int otraDuracion) const;
    bool esDelHuesped(const std::string& doc) const;
};

#endif // RESERVACION_H
