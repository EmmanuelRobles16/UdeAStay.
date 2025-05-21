#include "archivos.h"
#include "huesped.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "reservacion.h"
#include "Fecha.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Utilidad para partir cadenas
string* Archivo::split(const string& str, char delimitador, int& totalPartes) {
    totalPartes = 0;
    stringstream ss(str);
    string parte;
    // Reserva espacio para hasta 20 tokens
    string* resultado = new string[20];
    while (getline(ss, parte, delimitador) && totalPartes < 20) {
        resultado[totalPartes++] = parte;
    }
    return resultado;
}

// Carga de huéspedes
void Archivo::cargarHuespedes(Huesped**& huespedes, int& cantidad) {
    ifstream archivo("huespedes.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir huespedes.txt\n";
        cantidad = 0;
        huespedes = nullptr;
        return;
    }

    string linea;
    int capacidad = 10;
    cantidad = 0;
    huespedes = new Huesped*[capacidad];

    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        int nTokens = 0;
        string* tokens = split(linea, '|', nTokens);
        if (nTokens < 4) { delete[] tokens; continue; }

        string doc = tokens[0];
        int antig    = stoi(tokens[1]);
        float punt   = stof(tokens[2]);
        string pwd = tokens[3];
        delete[] tokens;

        if (cantidad == capacidad) {
            capacidad *= 2;
            Huesped** tmp = new Huesped*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = huespedes[i];
            delete[] huespedes;
            huespedes = tmp;
        }
        huespedes[cantidad++] = new Huesped(doc, antig, punt, pwd);
    }

    archivo.close();
}

// Carga de anfitriones
void Archivo::cargarAnfitriones(Anfitrion**& anfitriones, int& cantidad) {
    ifstream archivo("anfitriones.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir anfitriones.txt\n";
        cantidad = 0;
        anfitriones = nullptr;
        return;
    }

    string linea;
    int capacidad = 10;
    cantidad = 0;
    anfitriones = new Anfitrion*[capacidad];

    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        int nTokens = 0;
        string* tokens = split(linea, '|', nTokens);
        if (nTokens < 5) { delete[] tokens; continue; }

        string doc = tokens[0];
        int antig  = stoi(tokens[1]);
        float punt = stof(tokens[2]);
        string pwd = tokens[3];

        int nCods = 0;
        string* cods = split(tokens[4], ',', nCods);
        delete[] tokens;

        if (cantidad == capacidad) {
            capacidad *= 2;
            Anfitrion** tmp = new Anfitrion*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = anfitriones[i];
            delete[] anfitriones;
            anfitriones = tmp;
        }
        anfitriones[cantidad++] = new Anfitrion(doc, antig, punt, pwd, cods, nCods);
        delete[] cods;
    }

    archivo.close();
}

// Carga de alojamientos
void Archivo::cargarAlojamientos(Alojamiento**& alojamientos,
                                 int& cantidad,
                                 Anfitrion** anfitriones,
                                 int cantidadAnfitriones)
{
    ifstream archivo("alojamientos.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir alojamientos.txt\n";
        cantidad = 0;
        alojamientos = nullptr;
        return;
    }

    string linea;
    int capacidad = 10;
    cantidad = 0;
    alojamientos = new Alojamiento*[capacidad];

    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        int nTokens = 0;
        string* tokens = split(linea, '|', nTokens);
        if (nTokens < 9) { delete[] tokens; continue; }

        string codigo    = tokens[0];
        string nombre    = tokens[1];
        string docAnf    = tokens[2];
        string depto     = tokens[3];
        string municipio = tokens[4];
        string tipo      = tokens[5];
        string direccion = tokens[6];
        float precio     = stof(tokens[7]);

        int nAmen = 0;
        string* amenList = split(tokens[8], ',', nAmen);
        delete[] tokens;

        Anfitrion* anfit = nullptr;
        for (int i = 0; i < cantidadAnfitriones; ++i) {
            if (anfitriones[i]->getDocumento() == docAnf) {
                anfit = anfitriones[i];
                break;
            }
        }

        if (cantidad == capacidad) {
            capacidad *= 2;
            Alojamiento** tmp = new Alojamiento*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = alojamientos[i];
            delete[] alojamientos;
            alojamientos = tmp;
        }
        alojamientos[cantidad++] = new Alojamiento(
            codigo, nombre, tipo, direccion,
            depto, municipio, precio,
            amenList, nAmen,
            anfit
            );
        delete[] amenList;
    }

    archivo.close();
}

// Carga de reservas vigentes
void Archivo::cargarReservasVigentes(Reservacion**& reservas, int& cantidad, Alojamiento** alojamientos, int cantidadAloj, Huesped** huespedes, int cantidadHues)
{
    ifstream archivo("reservas_vigentes.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir reservas_vigentes.txt\n";
        cantidad = 0;
        reservas = nullptr;
        return;
    }

    string linea;
    int capacidad = 10;
    cantidad = 0;
    reservas = new Reservacion*[capacidad];

    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        int nTokens = 0;
        string* tokens = split(linea, '|', nTokens);
        if (nTokens < 9) { delete[] tokens; continue; }

        string codRes    = tokens[0];
        string codAloj   = tokens[1];
        string docHues   = tokens[2];
        string strFechaE = tokens[3];
        int duracion     = stoi(tokens[4]);
        string metPago   = tokens[5];
        string strFechaP = tokens[6];
        float monto      = stof(tokens[7]);
        string anotacion = tokens[8];
        delete[] tokens;

        Alojamiento* aloj = nullptr;
        for (int i = 0; i < cantidadAloj; ++i) {
            if (alojamientos[i]->getCodigo() == codAloj) {
                aloj = alojamientos[i];
                break;
            }
        }
        Huesped* hues = nullptr;
        for (int i = 0; i < cantidadHues; ++i) {
            if (huespedes[i]->getDocumento() == docHues) {
                hues = huespedes[i];
                break;
            }
        }

        Fecha fechaE = Fecha::desdeString(strFechaE);
        Fecha fechaP = Fecha::desdeString(strFechaP);

        if (cantidad == capacidad) {
            capacidad *= 2;
            Reservacion** tmp = new Reservacion*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = reservas[i];
            delete[] reservas;
            reservas = tmp;
        }

        reservas[cantidad++] = new Reservacion(
            codRes, fechaE, duracion,
            metPago, fechaP, monto,
            anotacion, aloj, hues
            );

        Reservacion* r = new Reservacion(
            codRes, fechaE, duracion,
            metPago, fechaP, monto,
            anotacion, aloj, hues
            );

        // 2) Registramos esa reserva dentro de
        //    su alojamiento *y* en su huésped:
        if (aloj) aloj->agregarReserva(r);
        if (hues) hues->agregarReserva(r);

        // 3) Finalmente la guardamos en el array global
        reservas[cantidad++] = r;
    }

    archivo.close();
}

// Carga de reservas históricas
void Archivo::cargarReservasHistoricas(Reservacion**& reservas, int& cantidad,  Alojamiento** alojamientos, int cantidadAloj,  Huesped** huespedes, int cantidadHues)
{
    ifstream archivo("reservas_historico.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir reservas_historico.txt\n";
        cantidad = 0;
        reservas = nullptr;
        return;
    }

    string linea;
    int capacidad = 10;
    cantidad = 0;
    reservas = new Reservacion*[capacidad];

    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        int nTokens = 0;
        string* tokens = split(linea, '|', nTokens);
        if (nTokens < 9) { delete[] tokens; continue; }

        string codRes    = tokens[0];
        string codAloj   = tokens[1];
        string docHues   = tokens[2];
        string strFechaE = tokens[3];
        int duracion     = stoi(tokens[4]);
        string metPago   = tokens[5];
        string strFechaP = tokens[6];
        float monto      = stof(tokens[7]);
        string anotacion = tokens[8];
        delete[] tokens;

        Alojamiento* aloj = nullptr;
        for (int i = 0; i < cantidadAloj; ++i) {
            if (alojamientos[i]->getCodigo() == codAloj) {
                aloj = alojamientos[i];
                break;
            }
        }
        Huesped* hues = nullptr;
        for (int i = 0; i < cantidadHues; ++i) {
            if (huespedes[i]->getDocumento() == docHues) {
                hues = huespedes[i];
                break;
            }
        }

        Fecha fechaE = Fecha::desdeString(strFechaE);
        Fecha fechaP = Fecha::desdeString(strFechaP);

        if (cantidad == capacidad) {
            capacidad *= 2;
            Reservacion** tmp = new Reservacion*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = reservas[i];
            delete[] reservas;
            reservas = tmp;
        }

        reservas[cantidad++] = new Reservacion(
            codRes, fechaE, duracion,
            metPago, fechaP, monto,
            anotacion, aloj, hues
            );
    }

    archivo.close();
}

