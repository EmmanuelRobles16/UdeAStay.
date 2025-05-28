#include "Huesped.h"
#include <cstdio>  // printf
#include <cstdlib> // malloc, free, realloc
#include <cstring> // strlen, strcpy, strcmp

// Constructor parametrizado
Huesped::Huesped(const char* doc, int antig, float punt, const char* pwd)
    : antiguedad(antig), puntuacion(punt), cantidadReservas(0), capacidadReservas(5)
{
    // Copiar documento
    int lenDoc = std::strlen(doc) + 1;
    documento = (char*)std::malloc(lenDoc);
    std::strcpy(documento, doc);

    // Copiar password
    int lenPwd = std::strlen(pwd) + 1;
    password = (char*)std::malloc(lenPwd);
    std::strcpy(password, pwd);

    // Inicializar arreglo de reservaciones
    reservaciones = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidadReservas);
}

// Constructor de copia (deep copy)
Huesped::Huesped(const Huesped& otro)
    : antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    cantidadReservas(otro.cantidadReservas), capacidadReservas(otro.capacidadReservas)
{
    // Copiar documento
    int lenDoc = std::strlen(otro.documento) + 1;
    documento = (char*)std::malloc(lenDoc);
    std::strcpy(documento, otro.documento);

    // Copiar password
    int lenPwd = std::strlen(otro.password) + 1;
    password = (char*)std::malloc(lenPwd);
    std::strcpy(password, otro.password);

    // Copiar arreglo de reservaciones (shallow copy de punteros)
    reservaciones = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidadReservas);
    for (int i = 0; i < cantidadReservas; ++i) {
        reservaciones[i] = otro.reservaciones[i];
    }
}

// Operador de asignación (deep copy + self-check)
Huesped& Huesped::operator=(const Huesped& otro)
{
    if (this == &otro) return *this;

    // Liberar recursos actuales
    std::free(documento);
    std::free(password);
    std::free(reservaciones);

    // Copiar información básica
    antiguedad = otro.antiguedad;
    puntuacion = otro.puntuacion;
    cantidadReservas = otro.cantidadReservas;
    capacidadReservas = otro.capacidadReservas;

    // Copiar documento
    int lenDoc = std::strlen(otro.documento) + 1;
    documento = (char*)std::malloc(lenDoc);
    std::strcpy(documento, otro.documento);

    // Copiar password
    int lenPwd = std::strlen(otro.password) + 1;
    password = (char*)std::malloc(lenPwd);
    std::strcpy(password, otro.password);

    // Copiar arreglo de reservaciones
    reservaciones = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidadReservas);
    for (int i = 0; i < cantidadReservas; ++i) {
        reservaciones[i] = otro.reservaciones[i];
    }

    return *this;
}

// Destructor
Huesped::~Huesped()
{
    std::free(documento);
    std::free(password);
    std::free(reservaciones);
}

// Getters
void Huesped::getDocumento(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", documento);
}

int Huesped::getAntiguedad() const {
    return antiguedad;
}

float Huesped::getPuntuacion() const {
    return puntuacion;
}

void Huesped::getPassword(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", password);
}

// Mostrar reservaciones
void Huesped::mostrarReservas() const {
    char resumen[256];
    for (int i = 0; i < cantidadReservas; ++i) {
        reservaciones[i]->toResumen(resumen, sizeof(resumen));
        std::printf("%s\n", resumen);
    }
}

// Añadir reserva
void Huesped::agregarReserva(Reservacion* r) {
    if (cantidadReservas == capacidadReservas) {
        int nuevaCap = capacidadReservas * 2;
        reservaciones = (Reservacion**)std::realloc(reservaciones, sizeof(Reservacion*) * nuevaCap);
        capacidadReservas = nuevaCap;
    }
    reservaciones[cantidadReservas++] = r;
}

// Anular reserva activa
void Huesped::anularReservacion(const char* codigoReserva, Reservacion**& reservasGlobal, int& cantidadGlobal)
{
    bool encontrado = false;

    // 1) Quitar del arreglo interno del huésped
    for (int i = 0; i < cantidadReservas; ++i) {
        char cod[64];
        reservaciones[i]->getCodigo(cod, sizeof(cod));
        if (std::strcmp(cod, codigoReserva) == 0) {
            for (int j = i; j + 1 < cantidadReservas; ++j)
                reservaciones[j] = reservaciones[j+1];
            cantidadReservas--;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        std::printf("No tienes ninguna reserva con código '%s'.\n", codigoReserva);
        return;
    }

    // 2) Quitar del arreglo global de reservas vigentes
    for (int i = 0; i < cantidadGlobal; ++i) {
        char cod[64];
        reservasGlobal[i]->getCodigo(cod, sizeof(cod));
        if (std::strcmp(cod, codigoReserva) == 0) {
            delete reservasGlobal[i];
            for (int j = i; j + 1 < cantidadGlobal; ++j)
                reservasGlobal[j] = reservasGlobal[j+1];
            cantidadGlobal--;
            break;
        }
    }

    // 3) Reescribir archivo excluyendo la reserva
    FILE* fin  = std::fopen("reservas_vigentes.txt", "r");
    FILE* fout = std::fopen("reservas_vigentes.tmp", "w");
    if (fin && fout) {
        char linea[1024];
        while (std::fgets(linea, sizeof(linea), fin)) {
            char copia[1024];
            std::strcpy(copia, linea);
            char* token = std::strtok(linea, "|");
            if (token && std::strcmp(token, codigoReserva) == 0)
                continue;
            std::fputs(copia, fout);
        }
        std::fclose(fin);
        std::fclose(fout);
        std::remove("reservas_vigentes.txt");
        std::rename("reservas_vigentes.tmp", "reservas_vigentes.txt");
    }

    std::printf("Reserva '%s' anulada correctamente.\n", codigoReserva);
}
