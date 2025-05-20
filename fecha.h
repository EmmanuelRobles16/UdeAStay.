#ifndef FECHA_H
#define FECHA_H

#include <string>

class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    // Constructores
    Fecha();
    Fecha(int d, int m, int a);

    // Getters
    int getDia() const;
    int getMes() const;
    int getAnio() const;

    // Comparación
    bool esAnterior(const Fecha& otra) const;

    // Operaciones
    Fecha sumarDias(int n) const;
    int toIndex() const;  // 0–364

    // Representaciones textuales
    std::string toStringCorto() const; // "YYYY-MM-DD"
    std::string toStringLargo() const; // "jueves 15 de mayo del 2025"

    // Utilidades estáticas
    static int diasDelMes(int mes, int anio);
    static std::string nombreMes(int mes);
    static std::string nombreDiaSemana(int d, int m, int a);
    static Fecha desdeString(const std::string& texto); // "YYYY-MM-DD"

    // despues de hablar con el profesor decidi quer voy a hacer la comprobacion de disponibilidad de alojamientos en el modulo fecha
    static bool seCruzanRangos(const Fecha& inicio1, const Fecha& fin1, const Fecha& inicio2, const Fecha& fin2);
};

#endif // FECHA_H
