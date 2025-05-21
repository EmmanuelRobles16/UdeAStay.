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

class RegistroReservas{
public:
    /**
     * Inicializa el gestor con el array de huéspedes.
     * Se crea internamente:
     *  - un array global de Reservacion* (capacidad inicial 10)
     *  - un arreglo de listas por huésped (capacidad inicial 4 cada una)
     *
     * @param huespedes     Punteros a los huéspedes existentes.
     * @param cantHuespedes Número de huéspedes en la plataforma.
     */
    RegistroReservas(Huesped** huespedes, int cantHuespedes);
    /** Libera toda la memoria asignada. */
    ~RegistroReservas();

    void cargarVigentes(const char* rutaArchivo,Alojamiento** alojamientos, int cantAlojamientos);

    /**
     * Devuelve el sub-array de Reservacion* para el huésped 'idx'.
     * outCantidad se fija al número de reservas de ese huésped.
     */
    Reservacion** getReservasPorHuesped(int idx, int& outCantidad) const;

private:
    // Array global de todas las reservas
    Reservacion** arreglo;
    int  total;
    int  capacidad;

    // Listas por huésped
    struct Lista {
        Reservacion** datos;
        int tam;    // cuántas reservas actualmente
        int cap;// capacidad del array 'datos'
    };
    Lista*    porHues;     // tamaño = cantHuespedes
    Huesped** listaHues;   // punteros originales
    int       cantHues;

    // Helpers para redimensionar
    /** Duplicar capacidad del array global cuando esté lleno. */
    void ensureGlobalCapacidad();
     /** Duplicar capacidad de la lista de un huésped cuando esté llena. */
    void ensureListaCapacidad(int idx);
};
#endif // RESERVACION_H
