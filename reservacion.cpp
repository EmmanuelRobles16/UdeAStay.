#include "Reservacion.h"
#include "Alojamiento.h"
#include "huesped.h"
#include "archivos.h"
#include <sstream>
#include <fstream>
#include <iostream>

using std::string;
using std::ostringstream;
using namespace std;

// Constructor
Reservacion::Reservacion(const string& codigo, const Fecha& fechaEntrada, int duracion, const string& metodoPago, const Fecha& fechaPago, float monto, const string& anotacion, Alojamiento* alojamiento, Huesped* huesped)
    : codigo(codigo),
    fechaEntrada(fechaEntrada),
    duracion(duracion),
    metodoPago(metodoPago),
    fechaPago(fechaPago),
    monto(monto),
    anotacion(anotacion),
    alojamiento(alojamiento),
    huesped(huesped)
{}

// Getters

// Devuelve el código de la reserva
string Reservacion::getCodigo() const             { return codigo; }
// Devuelve la fecha de entrada
Fecha  Reservacion::getFechaEntrada() const      { return fechaEntrada; }
// Devuelve la duración en noches
int    Reservacion::getDuracion() const          { return duracion; }
// Devuelve el método de pago
string Reservacion::getMetodoPago() const        { return metodoPago; }
// Devuelve la fecha de pago
Fecha  Reservacion::getFechaPago() const         { return fechaPago; }
// Devuelve el monto pagado
float  Reservacion::getMonto() const             { return monto; }
// Devuelve la anotación del huésped
string Reservacion::getAnotacion() const         { return anotacion; }
// Devuelve el puntero al alojamiento reservado
Alojamiento* Reservacion::getAlojamiento() const { return alojamiento; }
// Devuelve el puntero al huésped que hizo la reserva
Huesped*     Reservacion::getHuesped() const     { return huesped; }

// Calcula fecha de salida
Fecha Reservacion::fechaSalida() const {
    return fechaEntrada.sumarDias(duracion);
}

// Genera comprobante detallado
string Reservacion::comprobante() const {
    ostringstream ss;
    ss << "Reserva " << codigo
       << " | Huésped: " << huesped->getDocumento()
       << " | Alojamiento: " << alojamiento->getCodigo()
       << " | Entrada: " << fechaEntrada.toStringLargo()
       << " | Salida: "  << fechaSalida().toStringLargo()
       << " | Monto: "   << monto;
    return ss.str();
}

std::string Reservacion::getResumen() const {
    std::ostringstream ss;
    ss
        << codigo << " : "
        << fechaEntrada.toStringCorto()
        << " - "
        << fechaSalida().toStringCorto();
    return ss.str();
}
//para verificar si la reserva pertenece al hueped
bool Reservacion::esDelHuesped(const std::string& doc) const {
    return huesped->getDocumento() == doc;
}


RegistroReservas::RegistroReservas(Huesped** huespedes, int cntH)
    : total(0), capacidad(10), listaHues(huespedes), cantHues(cntH)
{
    // Reservar espacio inicial para el array global
    arreglo = new Reservacion*[capacidad];
    // Inicializar una lista semidinámica para cada huésped
    porHues = new Lista[cantHues];
    for (int i = 0; i < cantHues; ++i) {
        porHues[i].tam  = 0;    // sin reservas aún
        porHues[i].cap  = 4;    // capacidad inicial de 4
        porHues[i].datos = new Reservacion*[4];
    }
}
RegistroReservas::~RegistroReservas() {
    // liberar arrays de listas
    for (int i = 0; i < cantHues; ++i)
        delete[] porHues[i].datos;
    delete[] porHues;
    // liberar cada Reservacion creada
    for (int i = 0; i < total; ++i)
        delete arreglo[i];
    delete[] arreglo;
}
// Duplica la capacidad del array global si está lleno
void RegistroReservas::ensureGlobalCapacidad() {
    if (total < capacidad) return;
    int nc = capacidad * 2;
    Reservacion** tmp = new Reservacion*[nc];
    for (int i = 0; i < total; ++i) tmp[i] = arreglo[i];
    delete[] arreglo;
    arreglo = tmp;
    capacidad = nc;
}
// Duplica la capacidad de la lista de un huésped si está llena
void RegistroReservas::ensureListaCapacidad(int idx) {
    auto& L = porHues[idx];
    if (L.tam < L.cap) return;
    int nc = L.cap * 2;
    Reservacion** tmp = new Reservacion*[nc];
    for (int i = 0; i < L.tam; ++i) tmp[i] = L.datos[i];
    delete[] L.datos;
    L.datos = tmp;
    L.cap  = nc;
}

void RegistroReservas::cargarVigentes(const char* rutaArchivo,Alojamiento** alojamientos, int cantidadAloj)
{
    ifstream fin(rutaArchivo);
    if (!fin.is_open()) {
        cerr << "Error al abrir " << rutaArchivo << "\n";
        return;
    }
    string linea;
    while (getline(fin, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        int nTok = 0;
        string* tok = Archivo::split(linea, '|', nTok);
        if (nTok < 9) { delete[] tok; continue; }

        // campos
        string codR = tok[0], codA = tok[1], docH = tok[2];
        string sFE  = tok[3], sFP  = tok[6], anot = tok[8];
        int    dur  = stoi(tok[4]);
        string mPago = tok[5];
        float  mont  = stof(tok[7]);
        delete[] tok;

        // puntero a Alojamiento
        Alojamiento* alo = nullptr;
        for (int i = 0; i < cantidadAloj; ++i)
            if (alojamientos[i]->getCodigo() == codA) {
                alo = alojamientos[i]; break;
            }

        // índice del huésped
        int idxH = -1;
        for (int i = 0; i < cantHues; ++i)
            if (listaHues[i]->getDocumento() == docH) {
                idxH = i; break;
            }
        Huesped* h = (idxH >= 0 ? listaHues[idxH] : nullptr);

        Fecha fe = Fecha::desdeString(sFE);
        Fecha fp = Fecha::desdeString(sFP);

        // reservar global
        ensureGlobalCapacidad();
        Reservacion* r = new Reservacion(
            codR, fe, dur,
            mPago, fp, mont,
            anot, alo, h
            );
        arreglo[total++] = r;

        // añadir a lista de su huésped
        if (idxH >= 0) {
            ensureListaCapacidad(idxH);
            porHues[idxH].datos[ porHues[idxH].tam++ ] = r;
        }
    }
    fin.close();
}
// Devuelve el array de reservas para el huésped idx (y su cantidad)
Reservacion** RegistroReservas::getReservasPorHuesped(int idx, int& outCantidad) const
{
    if (idx < 0 || idx >= cantHues) {
        outCantidad = 0;
        return nullptr;
    }
    outCantidad = porHues[idx].tam;
    return porHues[idx].datos;
}
