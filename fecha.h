#ifndef FECHA_H
#define FECHA_H

class Fecha {
private:
    int dia;
    int mes;
    int anio;

    // Días por mes en año no bisiesto
    static const int diasMes[12];

    // Nombre de días y meses para formato largo
    static const char* nombresDias[7];
    static const char* nombresMeses[12];

    // Auxiliares
    static bool esBisiesto(int año);
    static int diasEnMes(int m, int a);
    static int calcularDiaSemana(int d, int m, int a);
    static int parseInt(const char* s, int len);

public:
    // Constructor por defecto y parametrizado
    Fecha(int d = 1, int m = 1, int a = 1970);

    static Fecha hoy();

    // Validación de fecha
    bool validar() const;

    // Comparación cronológica
    bool esAnterior(const Fecha& otra) const;

    // Aritmética de fechas
    void sumarDias(int n);

    // Formateo a cadena C
    // buffer corto: mínimo 11 bytes ("YYYY-MM-DD" + '\0')
    void toShortString(char* buffer) const;

    // buffer largo: mínimo 50 bytes
    void toLongString(char* buffer) const;

    // Parseo desde cadena corta
    static Fecha fromShortString(const char* s);

    // Chequeo de rangos
    static bool seCruzanRangos(
        const Fecha& inicio1, const Fecha& fin1,
        const Fecha& inicio2, const Fecha& fin2
        );

    int getDia() const { return dia; }
    int getMes() const { return mes; }
    int getAnio() const { return anio; }
};

#endif
