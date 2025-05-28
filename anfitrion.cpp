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

void Anfitrion::anularReservacion(
    const char* codigoReserva,
    Reservacion**& reservasVigentes,
    int& cantidadVigentes
    ) const {
    bool encontrado = false;

    // Buscar y eliminar de reservasVigentes
    for (int i = 0; i < cantidadVigentes; ++i) {
        char cod[64];
        reservasVigentes[i]->getCodigo(cod, sizeof(cod));
        if (std::strcmp(cod, codigoReserva) == 0) {
            // Verificar que la reserva sea de este anfitrión
            if (reservasVigentes[i]->getAlojamiento()->getAnfitrion() != this) {
                std::printf("No puedes cancelar una reserva que no es tuya.\n");
                return;
            }
            // Eliminar objeto y desplazar
            delete reservasVigentes[i];
            for (int j = i; j + 1 < cantidadVigentes; ++j) {
                reservasVigentes[j] = reservasVigentes[j+1];
            }
            cantidadVigentes--;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        std::printf("No existe reserva con código '%s'.\n", codigoReserva);
        return;
    }

    // Reescribir archivo de reservas vigentes
    FILE* fin  = std::fopen("reservas_vigentes.txt", "r");
    FILE* fout = std::fopen("reservas_vigentes.tmp", "w");
    if (fin && fout) {
        char linea[1024];
        while (std::fgets(linea, sizeof(linea), fin)) {
            char copia[1024];
            std::strcpy(copia, linea);
            char* token = std::strtok(linea, "|");
            if (token && std::strcmp(token, codigoReserva) == 0) {
                continue;  // saltar esta línea
            }
            std::fputs(copia, fout);
        }
        std::fclose(fin);
        std::fclose(fout);
        std::remove("reservas_vigentes.txt");
        std::rename("reservas_vigentes.tmp", "reservas_vigentes.txt");
    }

    std::printf("Reserva '%s' anulada correctamente.\n", codigoReserva);
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

void Anfitrion::mostrarReservasPorRango(
    Alojamiento** todosAlojs,
    int           total,
    const Fecha&  desde,
    const Fecha&  hasta
    ) const {
    // Cabecera
    char docBuf[64];
    getDocumento(docBuf, sizeof(docBuf));
    printf("=== Reservas vigentes de %s entre %04d-%02d-%02d y %04d-%02d-%02d ===\n",
           docBuf,
           desde.getAnio(), desde.getMes(), desde.getDia(),
           hasta.getAnio(), hasta.getMes(), hasta.getDia());

    // Para cada alojamiento del anfitrión…
    for (int i = 0; i < total; ++i) {
        Alojamiento* al = todosAlojs[i];
        if (al->getAnfitrion() != this) continue;

        // Mostrar encabezado de alojamiento
        char codAl[64], nomAl[128];
        al->getCodigo(codAl, sizeof(codAl));
        al->getNombre(nomAl, sizeof(nomAl));
        printf("- %s: %s\n", codAl, nomAl);

        int mostradas = 0;
        // Recorremos sus reservas vigentes
        // (su arreglo interno ‘reservas’ via al->mostrarReservas(), pero filtrado aquí)
        for (int r = 0; r < al->getCantidadReservas(); ++r) {
            Reservacion* res = al->getReserva(r);
            Fecha fEnt = res->getFechaEntrada();
            // Comprueba fEnt >= desde  &&  fEnt <= hasta
            bool geDesde = !fEnt.esAnterior(desde);
            bool leHasta = !hasta.esAnterior(fEnt);
            if (geDesde && leHasta) {
                char resumen[128];
                res->toResumen(resumen, sizeof(resumen));
                printf("    - %s\n", resumen);
                mostradas++;
            }
        }
        if (mostradas == 0) {
            printf("    (sin reservas en ese rango)\n");
        }
    }
    printf("------------------------------------------------\n");
}
