#include "fecha.h"
#include <cstdio>   // sprintf
#include <cstdlib>  // atoi, malloc/free si fuese necesario

// Definición de días por mes (no bisiesto)
const int Fecha::diasMes[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
// Mapas de nombres para día de la semana y mes
const char* Fecha::nombresDias[7] = {
    "domingo", "lunes", "martes", "miércoles",
    "jueves", "viernes", "sábado"
};
const char* Fecha::nombresMeses[12] = {
    "enero","febrero","marzo","abril","mayo","junio",
    "julio","agosto","septiembre","octubre","noviembre","diciembre"
};

// Constructor: asigna campos internos
Fecha::Fecha(int d, int m, int a)
    : dia(d), mes(m), anio(a) {}

// Verifica año bisiesto: divisible por 4, no por 100 a menos que sea por 400
bool Fecha::esBisiesto(int a) {
    return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
}

// Días en mes: febrero con 29 si bisiesto
int Fecha::diasEnMes(int m, int a) {
    if (m == 2 && esBisiesto(a)) return 29;
    return diasMes[m - 1];
}

// Valida rangos de año, mes y día
bool Fecha::validar() const {
    if (anio < 1 || mes < 1 || mes > 12) return false;
    int dm = diasEnMes(mes, anio);
    return dia >= 1 && dia <= dm;
}

// Comparación lexicográfica: año, luego mes, luego día
bool Fecha::esAnterior(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

// Suma/resta días ajustando a nuevo mes/año automáticamente
void Fecha::sumarDias(int n) {
    dia += n;
    // Ajuste positivo
    while (dia > diasEnMes(mes, anio)) {
        dia -= diasEnMes(mes, anio);
        mes++;
        if (mes > 12) { mes = 1; anio++; }
    }
    // Ajuste negativo
    while (dia < 1) {
        mes--;
        if (mes < 1) { mes = 12; anio--; }
        dia += diasEnMes(mes, anio);
    }
}

// Parsea dígitos de s[0..len-1] a entero
int Fecha::parseInt(const char* s, int len) {
    int v = 0;
    for (int i = 0; i < len; ++i) {
        char c = s[i]; if (c < '0' || c > '9') break;
        v = v * 10 + (c - '0');
    }
    return v;
}

// Crea fecha desde string corto YYYY-MM-DD
Fecha Fecha::fromShortString(const char* s) {
    int a = parseInt(s + 0, 4);
    int m = parseInt(s + 5, 2);
    int d = parseInt(s + 8, 2);
    return Fecha(d, m, a);
}

// Escribe "YYYY-MM-DD" en buffer
void Fecha::toShortString(char* buffer) const {
    // %04d => rellena con ceros a 4 dígitos
    std::sprintf(buffer, "%04d-%02d-%02d", anio, mes, dia);
}

// Calcula día de la semana con Zeller (0=domingo)
int Fecha::calcularDiaSemana(int d, int m, int a) {
    if (m < 3) { m += 12; a -= 1; }
    int K = a % 100;
    int J = a / 100;
    int h = (d + (13*(m+1))/5 + K + K/4 + J/4 + 5*J) % 7;
    return (h + 6) % 7; // convertir 0=sábado a 0=domingo
}

// Escribe formato largo "jueves 15 de mayo del 2025"
void Fecha::toLongString(char* buffer) const {
    int dow = calcularDiaSemana(dia, mes, anio);
    const char* nd = nombresDias[dow];
    const char* nm = nombresMeses[mes - 1];
    std::sprintf(buffer, "%s %d de %s del %d", nd, dia, nm, anio);
}

// Verifica si dos rangos de fechas se solapan
bool Fecha::seCruzanRangos(
    const Fecha& i1, const Fecha& f1,
    const Fecha& i2, const Fecha& f2
    ) {
    // No se cruzan si fin1 < inicio2 o fin2 < inicio1
    return !(f1.esAnterior(i2) || f2.esAnterior(i1));
}
