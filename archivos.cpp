#include "Archivos.h"
#include "Huesped.h"
#include "Anfitrion.h"
#include "Alojamiento.h"
#include "Reservacion.h"
#include "Fecha.h"

// split: utiliza malloc para asignar array de punteros a char
char** Archivo::split(const char* str, char delimitador, int& totalPartes) {
    // Calcula máximo de tokens (estimado)
    int maxTokens = 20;
    char** resultado = (char**)std::malloc(sizeof(char*) * maxTokens);
    totalPartes = 0;

    // Copiar cadena para modificar
    char* buffer = (char*)std::malloc(std::strlen(str) + 1);
    std::strcpy(buffer, str);
    char* token = std::strtok(buffer, &delimitador);
    while (token && totalPartes < maxTokens) {
        int len = std::strlen(token) + 1;
        resultado[totalPartes] = (char*)std::malloc(len);  // malloc para cada token
        std::strcpy(resultado[totalPartes], token);
        totalPartes++;
        token = std::strtok(NULL, &delimitador);
    }
    std::free(buffer);
    return resultado;
}

// Ejemplo de lectura de Huespedes desde "huespedes.txt"
void Archivo::cargarHuespedes(Huesped**& huespedes, int& cantidad) {
    FILE* fp = std::fopen("huespedes.txt", "r");
    if (!fp) {
        printf("Error al abrir huespedes.txt\n");
        huespedes = NULL; cantidad = 0;
        return;
    }

    int capacidad = 10;
    huespedes = (Huesped**)std::malloc(sizeof(Huesped*) * capacidad);
    cantidad = 0;

    char linea[256];
    while (std::fgets(linea, sizeof(linea), fp)) {
        if (linea[0] == '#' || std::strlen(linea) < 2) continue;

        int nTokens;
        char** tokens = split(linea, '|', nTokens);
        if (nTokens < 4) {
            for (int i = 0; i < nTokens; ++i) std::free(tokens[i]);
            std::free(tokens);
            continue;
        }

        // Copiar valores antes de liberar los tokens
        char* docCpy = (char*)std::malloc(std::strlen(tokens[0]) + 1);
        std::strcpy(docCpy, tokens[0]);

        int antig = std::atoi(tokens[1]);
        float punt = std::atof(tokens[2]);

        char* pwdCpy = (char*)std::malloc(std::strlen(tokens[3]) + 1);
        std::strcpy(pwdCpy, tokens[3]);

        for (int i = 0; i < nTokens; ++i) std::free(tokens[i]);
        std::free(tokens);

        // Redimensionar si es necesario
        if (cantidad == capacidad) {
            capacidad *= 2;
            huespedes = (Huesped**)std::realloc(huespedes, sizeof(Huesped*) * capacidad);
        }

        // Crear huésped con copia segura
        huespedes[cantidad++] = new Huesped(docCpy, antig, punt, pwdCpy);

        std::free(docCpy);
        std::free(pwdCpy);
    }

    std::fclose(fp);
}
void Archivo::cargarAnfitriones(Anfitrion**& anfitriones, int& cantidad) {
    FILE* fp = std::fopen("anfitriones.txt", "r");
    if (!fp) {
        printf("Error al abrir anfitriones.txt\n");
        anfitriones = nullptr;
        cantidad = 0;
        return;
    }
    int capacidad = 10;
    anfitriones = (Anfitrion**)std::malloc(sizeof(Anfitrion*) * capacidad);
    cantidad = 0;
    char linea[512];

    while (std::fgets(linea, sizeof(linea), fp)) {
        if (linea[0] == '#' || std::strlen(linea) < 2) continue;
        int nTok;
        char** tok = split(linea, '|', nTok);
        if (nTok < 5) {
            for (int i = 0; i < nTok; ++i) std::free(tok[i]);
            std::free(tok);
            continue;
        }
        const char* doc   = tok[0];
        int         antig = std::atoi(tok[1]);
        float       punt  = std::atof(tok[2]);
        const char* pwd   = tok[3];

        // parse lista de códigos de alojamiento
        int numCod;
        char** codigos = split(tok[4], ',', numCod);

        // liberar tokens intermedios
        for (int i = 0; i < nTok; ++i) std::free(tok[i]);
        std::free(tok);

        // crecer arreglo si hace falta
        if (cantidad == capacidad) {
            capacidad *= 2;
            anfitriones = (Anfitrion**)std::realloc(anfitriones, sizeof(Anfitrion*) * capacidad);
        }
        // crear instancia
        anfitriones[cantidad++] =
            new Anfitrion(doc, antig, punt, pwd, (const char**)codigos, numCod);

        // liberar listaCod
        for (int i = 0; i < numCod; ++i) std::free(codigos[i]);
        std::free(codigos);
    }
    std::fclose(fp);
}
void Archivo::cargarAlojamientos(Alojamiento**& alojamientos, int& cantidad, Anfitrion** anfitriones, int cantidadAnf) {
    FILE* fp = std::fopen("alojamientos.txt", "r");
    if (!fp) {
        printf("Error al abrir alojamientos.txt\n");
        alojamientos = nullptr;
        cantidad = 0;
        return;
    }

    int capacidad = 10;
    alojamientos = (Alojamiento**)std::malloc(sizeof(Alojamiento*) * capacidad);
    cantidad = 0;
    char linea[1024];

    while (std::fgets(linea, sizeof(linea), fp)) {
        if (linea[0] == '#' || std::strlen(linea) < 2) continue;

        int nTok;
        char** tok = split(linea, '|', nTok);
        if (nTok < 9) {
            for (int i = 0; i < nTok; ++i) std::free(tok[i]);
            std::free(tok);
            continue;
        }

        const char* cod      = tok[0];
        const char* nom      = tok[1];
        const char* tip      = tok[2];
        const char* dir      = tok[3];
        const char* dep      = tok[4];
        const char* muni     = tok[5];
        float       precio   = std::atof(tok[6]);

        // lista de amenidades
        int numAm;
        char** ams = split(tok[7], ',', numAm);

        // documento del anfitrión con limpieza de saltos de línea
        const char* docAnfRaw = tok[8];
        char* docAnf = (char*)std::malloc(strlen(docAnfRaw) + 1);
        std::strcpy(docAnf, docAnfRaw);
        Archivo::trimNewline(docAnf);  // ✅ Limpia '\n' y '\r'

        // buscar anfitrión correspondiente
        Anfitrion* ptrAnf = nullptr;
        char buf[64];
        for (int i = 0; i < cantidadAnf; ++i) {
            anfitriones[i]->getDocumento(buf, sizeof(buf));
            Archivo::trimNewline(buf);  // Limpieza también en el comparado
            if (std::strcmp(buf, docAnf) == 0) {
                ptrAnf = anfitriones[i];
                break;
            }
        }
        std::free(docAnf);

        for (int i = 0; i < nTok; ++i) std::free(tok[i]);
        std::free(tok);

        if (cantidad == capacidad) {
            capacidad *= 2;
            alojamientos = (Alojamiento**)std::realloc(alojamientos, sizeof(Alojamiento*) * capacidad);
        }

        alojamientos[cantidad++] =
            new Alojamiento(cod, nom, tip, dir, dep, muni, precio, ams, numAm, ptrAnf);

        for (int i = 0; i < numAm; ++i) std::free(ams[i]);
        std::free(ams);
    }

    std::fclose(fp);
}
void Archivo::cargarReservasVigentes(
    Reservacion**& reservas, int& cantidad,
    Alojamiento** alojamientos, int cantAl,
    Huesped** huespedes, int cantH
    ) {
    FILE* fp = std::fopen("reservas_vigentes.txt", "r");
    if (!fp) {
        printf("Error al abrir reservas_vigentes.txt\n");
        reservas = nullptr;
        cantidad = 0;
        return;
    }

    int capacidad = 10;
    reservas = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidad);
    cantidad = 0;

    char linea[1024];
    while (std::fgets(linea, sizeof(linea), fp)) {
        if (linea[0] == '#' || std::strlen(linea) < 2) continue;

        int nTok;
        char** t = split(linea, '|', nTok);
        if (nTok < 9) {
            for (int i = 0; i < nTok; ++i) std::free(t[i]);
            std::free(t);
            continue;
        }

        const char* codR   = t[0];
        const char* codAl  = t[1];
        const char* docH   = t[2];
        Fecha       fEnt   = Fecha::fromShortString(t[3]);
        int         dur    = std::atoi(t[4]);
        const char* met    = t[5];
        Fecha       fPago  = Fecha::fromShortString(t[6]);
        float       monto  = std::atof(t[7]);
        const char* nota   = t[8];

        // Buscar puntero al alojamiento
        Alojamiento* pa = nullptr;
        for (int i = 0; i < cantAl; ++i) {
            char buf[64];
            alojamientos[i]->getCodigo(buf, sizeof(buf));
            if (std::strcmp(buf, codAl) == 0) {
                pa = alojamientos[i];
                break;
            }
        }

        // Buscar puntero al huésped
        Huesped* ph = nullptr;
        for (int j = 0; j < cantH; ++j) {
            char buf[64];
            huespedes[j]->getDocumento(buf, sizeof(buf));
            if (std::strcmp(buf, docH) == 0) {
                ph = huespedes[j];
                break;
            }
        }

        for (int i = 0; i < nTok; ++i) std::free(t[i]);
        std::free(t);

        // Si no se encuentra alguno de los punteros, se ignora la reserva
        if (!pa || !ph) continue;

        // Redimensionar si hace falta
        if (cantidad == capacidad) {
            capacidad *= 2;
            reservas = (Reservacion**)std::realloc(reservas, sizeof(Reservacion*) * capacidad);
        }

        // Crear la reserva
        Reservacion* nueva = new Reservacion(codR, fEnt, dur, met, fPago, monto, nota, pa, ph);

        // Agregar a arreglo global
        reservas[cantidad++] = nueva;

        // ✅ Agregar al alojamiento y al huésped
        pa->agregarReserva(nueva);
        ph->agregarReserva(nueva);
    }

    std::fclose(fp);
}


// ---------------------------------------------------------------
// Implementación de cargarReservasHistoricas
// lee “reservas_historicas.txt” (idéntico al anterior)
// ---------------------------------------------------------------
void Archivo::cargarReservasHistoricas(
    Reservacion**& reservas, int& cantidad,
    Alojamiento** alojamientos, int cantAl,
    Huesped** huespedes, int cantH
    ) {
    FILE* fp = std::fopen("reservas_historico.txt", "r");
    if (!fp) {
        printf("Error al abrir reservas_historico.txt\n");
        reservas = nullptr;
        cantidad = 0;
        return;
    }

    int capacidad = 10;
    reservas = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidad);
    cantidad = 0;

    char linea[1024];
    while (std::fgets(linea, sizeof(linea), fp)) {
        if (linea[0] == '#' || std::strlen(linea) < 2) continue;

        int nTok;
        char** t = split(linea, '|', nTok);
        if (nTok < 9) {
            for (int i = 0; i < nTok; ++i) std::free(t[i]);
            std::free(t);
            continue;
        }

        const char* codR   = t[0];
        const char* codAl  = t[1];
        const char* docH   = t[2];
        Fecha       fEnt   = Fecha::fromShortString(t[3]);
        int         dur    = std::atoi(t[4]);
        const char* met    = t[5];
        Fecha       fPago  = Fecha::fromShortString(t[6]);
        float       monto  = std::atof(t[7]);
        const char* nota   = t[8];

        Alojamiento* pa = nullptr;
        for (int i = 0; i < cantAl; ++i) {
            char buf[64];
            alojamientos[i]->getCodigo(buf, sizeof(buf));
            if (std::strcmp(buf, codAl) == 0) {
                pa = alojamientos[i];
                break;
            }
        }

        Huesped* ph = nullptr;
        for (int j = 0; j < cantH; ++j) {
            char buf[64];
            huespedes[j]->getDocumento(buf, sizeof(buf));
            if (std::strcmp(buf, docH) == 0) {
                ph = huespedes[j];
                break;
            }
        }

        for (int i = 0; i < nTok; ++i) std::free(t[i]);
        std::free(t);

        if (!pa || !ph || !pa->getAnfitrion()) {
            printf("❌ RH %s NO CARGADA\n", codR);
            continue;
        }

        if (cantidad == capacidad) {
            capacidad *= 2;
            reservas = (Reservacion**)std::realloc(reservas, sizeof(Reservacion*) * capacidad);
        }

        Reservacion* nueva = new Reservacion(codR, fEnt, dur, met, fPago, monto, nota, pa, ph);
        reservas[cantidad++] = nueva;
    }

    std::fclose(fp);
}


void Archivo::trimNewline(char* s) {
    int len = std::strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}
