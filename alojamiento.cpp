#include "Alojamiento.h"

// Helper para copiar cadena C a memoria dinámica
static char* dupCadena(const char* src) {
    int len = std::strlen(src) + 1;              // std::strlen obtiene longitud sin '\0'
    char* dst = (char*)std::malloc(len);        // Reserva len bytes
    std::strcpy(dst, src);                      // Copia incluyendo '\0'
    return dst;
}

// Constructor: copia todos los campos y crea arreglos
Alojamiento::Alojamiento(const char* cod, const char* nom, const char* tip, const char* dir, const char* depto, const char* muni, float precio, char** amenArr, int cantAmen, Anfitrion* Anf)
    : cantidadAmenidades(cantAmen), cantidadReservas(0), capacidadReservas(5), anfitrion(Anf)
{
    // Copiar cadenas con dupCadena (usa malloc y strcpy)
    codigo      = dupCadena(cod);
    nombre      = dupCadena(nom);
    tipo        = dupCadena(tip);
    direccion   = dupCadena(dir);
    departamento = dupCadena(depto);
    municipio   = dupCadena(muni);
    precioPorNoche = precio;

    // Crear arreglo de amenidades y copiar cada cadena
    amenidades = (char**)std::malloc(sizeof(char*) * cantidadAmenidades);
    for (int i = 0; i < cantidadAmenidades; ++i) {
        amenidades[i] = dupCadena(amenArr[i]);
    }

    // Inicializar arreglo de reservas con malloc
    reservas = (Reservacion**)std::malloc(sizeof(Reservacion*) * capacidadReservas);
}

// Destructor: libera amenidades, reservas y cadenas
Alojamiento::~Alojamiento() {
    // Liberar amenidades
    for (int i = 0; i < cantidadAmenidades; ++i) {
        std::free(amenidades[i]);                  // free libera malloc
    }
    std::free(amenidades);

    // Liberar reservas array (no libera objetos Reservacion)
    std::free(reservas);

    // Liberar cadenas básicas
    std::free(codigo);
    std::free(nombre);
    std::free(tipo);
    std::free(direccion);
    std::free(departamento);
    std::free(municipio);
}

// Getters: snprintf para copiar en buffer de tamaño bufSize
void Alojamiento::getCodigo(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", codigo);
}
void Alojamiento::getNombre(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", nombre);
}
void Alojamiento::getTipo(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", tipo);
}
void Alojamiento::getDireccion(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", direccion);
}
void Alojamiento::getDepartamento(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", departamento);
}
void Alojamiento::getMunicipio(char* buffer, int bufSize) const {
    std::snprintf(buffer, bufSize, "%s", municipio);
}
float Alojamiento::getPrecioPorNoche() const {
    return precioPorNoche;
}
Anfitrion* Alojamiento::getAnfitrion() const {
    return anfitrion;
}

// Amenidades
int Alojamiento::getCantidadAmenidades() const {
    return cantidadAmenidades;
}
void Alojamiento::mostrarAmenidades() const {
    printf("Amenidades (%d):\n", cantidadAmenidades);
    for (int i = 0; i < cantidadAmenidades; ++i) {
        printf("  - %s\n", amenidades[i]);
    }
}

// Agrega reserva: duplica capacidad si es necesario
void Alojamiento::agregarReserva(Reservacion* r) {
    if (cantidadReservas == capacidadReservas) {
        int nuevaCap = capacidadReservas * 2;
        // std::realloc ajusta tamaño del bloque, preserva datos previos
        reservas = (Reservacion**)std::realloc(reservas, sizeof(Reservacion*) * nuevaCap);
        capacidadReservas = nuevaCap;
    }
    reservas[cantidadReservas++] = r;
}

// Anular reserva por código: busca y desplaza elementos
void Alojamiento::anularReserva(const char* codRes) {
    char actual[64];
    for (int i = 0; i < cantidadReservas; ++i) {
        reservas[i]->getCodigo(actual, sizeof(actual));
        // strcmp compara cadenas C, devuelve 0 si iguales
        if (std::strcmp(actual, codRes) == 0) {
            for (int j = i; j < cantidadReservas - 1; ++j) {
                reservas[j] = reservas[j + 1];
            }
            cantidadReservas--;
            return;
        }
    }
}

// Mostrar reservas vigentes
void Alojamiento::mostrarReservas() const {
    if (cantidadReservas == 0) {
        printf("    (no hay reservas vigentes)\n");
        return;
    }
    for (int i = 0; i < cantidadReservas; ++i) {
        char resumen[128];
        reservas[i]->toResumen(resumen, sizeof(resumen));
        printf("    - %s\n", resumen);
    }
}
