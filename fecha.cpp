#include "fecha.h"
#include <sstream>
#include <iostream>

// Constructor por defecto
Fecha::Fecha() : dia(1), mes(1), anio(2025) {}

// Constructor con validación
Fecha::Fecha(int d, int m, int a) {
    // Validar año
    if (a < 1) {
        std::cerr << "Fecha inválida: año " << a
                  << ". Usando año 1.\n";
        anio = 1;
    } else {
        anio = a;
    }

    // Validar mes
    if (m < 1 || m > 12) {
        std::cerr << "Fecha inválida: mes " << m
                  << ". Usando mes 1.\n";
        mes = 1;
    } else {
        mes = m;
    }

    // Validar día según mes y año
    int dim = diasDelMes(mes, anio);
    if (d < 1 || d > dim) {
        std::cerr << "Fecha inválida: día " << d
                  << " para " << mes << "/" << anio
                  << ". Usando día 1.\n";
        dia = 1;
    } else {
        dia = d;
    }
}

// Getters
int Fecha::getDia()    const { return dia; }
int Fecha::getMes()    const { return mes; }
int Fecha::getAnio()   const { return anio; }

// Comparación cronológica
bool Fecha::esAnterior(const Fecha& otra) const {
    if (anio  < otra.anio) return true;
    if (anio == otra.anio && mes  < otra.mes) return true;
    if (anio == otra.anio && mes == otra.mes && dia < otra.dia) return true;
    return false;
}

// Día del año 0–364
int Fecha::toIndex() const {
    int idx = dia - 1;
    for (int mm = 1; mm < mes; ++mm)
        idx += diasDelMes(mm, anio);
    return idx;
}

// Sumar n días (respeta fin de mes y años bisiestos)
Fecha Fecha::sumarDias(int n) const {
    int d = dia, m = mes, a = anio;
    while (n > 0) {
        int dim = diasDelMes(m, a);
        if (d + n <= dim) {
            d += n;
            break;
        }
        n -= (dim - d + 1);
        d = 1;
        if (++m > 12) { m = 1; ++a; }
    }
    return Fecha(d, m, a);
}

// Formato "YYYY-MM-DD"
std::string Fecha::toStringCorto() const {
    std::ostringstream ss;
    ss << anio << '-'
       << (mes < 10 ? "0" : "") << mes << '-'
       << (dia < 10 ? "0" : "") << dia;
    return ss.str();
}

// Formato largo en español
std::string Fecha::toStringLargo() const {
    static const char* nombresDias[7] = {
        "domingo","lunes","martes","miércoles",
        "jueves","viernes","sábado"
    };
    static const char* nombresMeses[12] = {
        "enero","febrero","marzo","abril","mayo","junio",
        "julio","agosto","septiembre","octubre","noviembre","diciembre"
    };

    // Cálculo día de la semana
    static const int t[] = {0,3,2,5,0,3,5,1,4,6,2,4};
    int y = anio - (mes < 3);
    int w = (y + y/4 - y/100 + y/400 + t[mes-1] + dia) % 7;

    std::ostringstream ss;
    ss << nombresDias[w] << ' '
       << dia << " de "
       << nombresMeses[mes-1] << " del "
       << anio;
    return ss.str();
}

// Días en un mes (incluye bisiesto)
int Fecha::diasDelMes(int mes, int anio) {
    static const int tabla[] = {
        31,28,31,30,31,30,31,31,30,31,30,31
    };
    if (mes == 2) {
        bool bisiesto = (anio % 400 == 0) ||
                        (anio % 4 == 0 && anio % 100 != 0);
        return bisiesto ? 29 : 28;
    }
    return tabla[mes-1];
}

// Parsea "YYYY-MM-DD" y usa el constructor validado
Fecha Fecha::desdeString(const std::string& texto) {
    int y, mo, d;
    char sep;
    std::istringstream ss(texto);
    ss >> y >> sep >> mo >> sep >> d;
    return Fecha(d, mo, y);
}
//la funcionalidad de comparacion de disponibilidad
bool Fecha::seCruzanRangos(const Fecha& inicio1,
                           const Fecha& fin1,
                           const Fecha& inicio2,
                           const Fecha& fin2)
{
    // No se cruzan si uno termina **antes** de que empiece el otro
    if (fin1.esAnterior(inicio2) || fin2.esAnterior(inicio1))
        return false;
    return true;
}
