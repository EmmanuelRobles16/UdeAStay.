#include "Anfitrion.h"
#include "archivos.h"
#include <cstring>
#include <cstdio>
extern Reservacion** reservasHistoricas;
extern int cantidadHistoricas;

// Constructor: copia cadenas y arreglo de códigos
Anfitrion::Anfitrion(const char* doc, int antig, float punt, const char* pwd, const char** codigos, int numCodigos)
    : antiguedad(antig), puntuacion(punt), cantidadAlojamientos(numCodigos), capacidadAlojamientos(numCodigos)
{
    // Copiar documento
    int lenDoc = std::strlen(doc) + 1;
    documento = (char*)std::malloc(lenDoc);         // std::malloc: reserva memoria en bytes
    std::strcpy(documento, doc);

    // Copiar password
    int lenPwd = std::strlen(pwd) + 1;
    password = (char*)std::malloc(lenPwd);
    std::strcpy(password, pwd);

    // Crear arreglo de punteros a cadenas para códigos
    codigosAlojamientos = (char**)std::malloc(sizeof(char*) * capacidadAlojamientos);
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        int len = std::strlen(codigos[i]) + 1;
        codigosAlojamientos[i] = (char*)std::malloc(len);
        std::strcpy(codigosAlojamientos[i], codigos[i]);
    }
}

// Destructor: libera cada bloque de memoria
Anfitrion::~Anfitrion() {
    // Liberar cadenas de códigos
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        std::free(codigosAlojamientos[i]);
    }
    std::free(codigosAlojamientos);
    // Liberar documento y password
    std::free(documento);   // std::free: libera memoria reservada con std::malloc
    std::free(password);
}

void Anfitrion::getDocumento(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", documento);  // std::snprintf: formatea seguro en buffer
}

int Anfitrion::getAntiguedad() const { return antiguedad; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

void Anfitrion::getPassword(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", password);
}

void Anfitrion::agregarAlojamiento(const char* codigo) {
    // Si lleno, duplicar capacidad
    if (cantidadAlojamientos == capacidadAlojamientos) {
        int nuevaCap = capacidadAlojamientos * 2;
        // std::realloc: ajusta tamaño del bloque, conserva datos
        codigosAlojamientos = (char**)std::realloc(codigosAlojamientos, sizeof(char*) * nuevaCap);
        capacidadAlojamientos = nuevaCap;
    }
    // Copiar nuevo código
    int len = std::strlen(codigo) + 1;
    codigosAlojamientos[cantidadAlojamientos] = (char*)std::malloc(len);
    std::strcpy(codigosAlojamientos[cantidadAlojamientos], codigo);
    cantidadAlojamientos++;
}

void Anfitrion::mostrarAlojamientos() const {
    char docBuf[64];
    std::snprintf(docBuf, sizeof(docBuf), "%s", documento);
    std::printf("Alojamientos de %s:\n", docBuf);
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        std::printf("  - %s\n", codigosAlojamientos[i]);
    }
}

void Anfitrion::anularReservacion(const char* codigoReserva, Alojamiento** todosAlojs, int total) const {
    bool encontrado = false;
    for (int k = 0; k < cantidadAlojamientos && !encontrado; ++k) {
        for (int i = 0; i < total; ++i) {
            char aloCod[64];
            todosAlojs[i]->getCodigo(aloCod, sizeof(aloCod));
            if (std::strcmp(aloCod, codigosAlojamientos[k]) == 0) {
                // Alojamiento gestiona la anulación
                todosAlojs[i]->anularReserva(codigoReserva);
                encontrado = true;
                break;
            }
        }
    }
    if (encontrado) {
        std::printf("Reserva '%s' anulada correctamente.\n", codigoReserva);
    } else {
        std::printf("No se encontró la reserva '%s' en tus alojamientos.\n", codigoReserva);
    }
}
void Anfitrion::mostrarAlojamientosYReservas(
    Alojamiento** todosAlojs,
    int           total,
    bool          historicas
    ) const {
    char docBuf[64];
    getDocumento(docBuf, sizeof(docBuf));
    printf("=== %s de %s (puntuación: %.2f) ===\n",
           historicas ? "Reservas históricas" : "Reservas vigentes",
           docBuf, puntuacion);

    // Recorremos TODOS los alojamientos de la plataforma...
    for (int i = 0; i < total; ++i) {
        Alojamiento* al = todosAlojs[i];
        // ...pero solo mostramos los que son de este anfitrión:
        if (al->getAnfitrion() != this) continue;

        char codAl[64], nomAl[128];
        al->getCodigo(codAl, sizeof(codAl));
        al->getNombre(nomAl, sizeof(nomAl));
        printf("- %s: %s\n", codAl, nomAl);

        if (!historicas) {
            // Reservas vigentes ya vinculadas al alojamiento
            al->mostrarReservas();
        } else {
            // Recorremos el arreglo global de históricas
            int mostradas = 0;
            for (int r = 0; r < cantidadHistoricas; ++r) {
                Reservacion* res = reservasHistoricas[r];
                if (!res) continue;
                // Solo las que pertenezcan a este anfitrión
                if (res->getAlojamiento()->getAnfitrion() != this) continue;
                char resumen[128];
                res->toResumen(resumen, sizeof(resumen));
                printf("    - %s\n", resumen);
                ++mostradas;
            }
            if (mostradas == 0) {
                printf("    (sin reservas históricas)\n");
            }
        }
    }
    printf("----------------------------------------\n");
}

