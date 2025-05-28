#include "Huesped.h"
#include <cstring> // strlen, strcpy

// Constructor: copia cadenas y crea arreglo inicial
Huesped::Huesped(const char* doc, int antig, float punt, const char* pwd)
    : antiguedad(antig), puntuacion(punt), cantidadReservas(0), capacidadReservas(5)
{
    // Copiar documento a memoria dinámica
    int lenDoc = std::strlen(doc) + 1;
    documento = (char*)std::malloc(lenDoc);
    std::strcpy(documento, doc);

    // Copiar password a memoria dinámica
    int lenPwd = std::strlen(pwd) + 1;
    password = (char*)std::malloc(lenPwd);
    std::strcpy(password, pwd);

    // Crear arreglo de Reservacion* con malloc
    reservaciones = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidadReservas);
}

// Destructor: libera cadenas y arreglo
Huesped::~Huesped() {
    std::free(documento);
    std::free(password);
    std::free(reservaciones);
}


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

void Huesped::mostrarReservas() const {
    char resumen[256];
    for (int i = 0; i < cantidadReservas; ++i) {
        // Suponiendo Reservacion tiene método toResumen(char*, int)
        reservaciones[i]->toResumen(resumen, sizeof(resumen));
        std::printf("%s\n", resumen);
    }
}

void Huesped::agregarReserva(Reservacion* r) {
    if (cantidadReservas == capacidadReservas) {
        int nuevaCap = capacidadReservas * 2;
        reservaciones = (Reservacion**)std::realloc(reservaciones, sizeof(Reservacion*) * nuevaCap);
        capacidadReservas = nuevaCap;
    }
    reservaciones[cantidadReservas++] = r;
}

void Huesped::anularReservacion(const char* codigoReserva, Reservacion**& reservasGlobal, int& cantidadGlobal)
{
    bool encontrado = false;

    // 1) Quitar del arreglo del huésped
    for (int i = 0; i < cantidadReservas; ++i) {
        char cod[64];
        reservaciones[i]->getCodigo(cod, sizeof(cod));
        if (std::strcmp(cod, codigoReserva) == 0) {
            // desplazar a la izquierda
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
            // liberar el objeto
            delete reservasGlobal[i];
            // desplazar
            for (int j = i; j + 1 < cantidadGlobal; ++j)
                reservasGlobal[j] = reservasGlobal[j+1];
            cantidadGlobal--;
            break;
        }
    }

    // 3) Reescribir el archivo excluyendo la línea cancelada
    FILE* fin  = std::fopen("reservas_vigentes.txt", "r");
    FILE* fout = std::fopen("reservas_vigentes.tmp", "w");
    if (fin && fout) {
        char linea[1024];
        while (std::fgets(linea, sizeof(linea), fin)) {
            char copia[1024];
            std::strcpy(copia, linea);
            char* token = std::strtok(linea, "|");
            if (token && std::strcmp(token, codigoReserva) == 0)
                continue;  // saltar línea
            std::fputs(copia, fout);
        }
        std::fclose(fin);
        std::fclose(fout);
        std::remove("reservas_vigentes.txt");
        std::rename("reservas_vigentes.tmp", "reservas_vigentes.txt");
    }

    std::printf("Reserva '%s' anulada correctamente.\n", codigoReserva);
}
