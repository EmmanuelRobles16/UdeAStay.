#include "archivos.h"
#include "huesped.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Archivo::cargarHuespedes(Huesped**& huespedes, int& cantidad) {
    ifstream archivo("C:\\Users\\Administrator\\Desktop\\desafio2\\desafio_2\\build\\Desktop_Qt_6_8_2_MinGW_64_bit-Debug\\huespedes.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir huespedes.txt" << endl;
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

        stringstream ss(linea);
        string doc;
        int antig;
        float punt;

        getline(ss, doc, '|');
        ss >> antig;
        ss.ignore(); // ignorar '|'
        ss >> punt;

        if (cantidad == capacidad) {
            capacidad *= 2;
            Huesped** nuevo = new Huesped*[capacidad];
            for (int i = 0; i < cantidad; ++i)
                nuevo[i] = huespedes[i];
            delete[] huespedes;
            huespedes = nuevo;
        }

        huespedes[cantidad++] = new Huesped(doc, antig, punt);
    }

    archivo.close();
}
void Archivo::cargarAnfitriones(Anfitrion**& anfitriones, int& cantidad) {
    ifstream archivo("anfitriones.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir anfitriones.txt" << endl;
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

        int campos = 0;
        string* tokens = split(linea, '|', campos);
        if (campos < 4) {
            delete[] tokens;
            continue;
        }

        string doc    = tokens[0];
        int antig     = stoi(tokens[1]);
        float punt    = stof(tokens[2]);

        int numCods = 0;
        string* cods = split(tokens[3], ',', numCods);
        delete[] tokens;

        if (cantidad == capacidad) {
            capacidad *= 2;
            Anfitrion** tmp = new Anfitrion*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = anfitriones[i];
            delete[] anfitriones;
            anfitriones = tmp;
        }

        anfitriones[cantidad++] = new Anfitrion(doc, antig, punt, cods, numCods);
        delete[] cods;
    }

    archivo.close();
}


string* Archivo::split(const string& str, char delimitador, int& totalPartes) {
    totalPartes = 0;
    stringstream ss(str);
    string parte;
    string* resultado = new string[10];

    while (getline(ss, parte, delimitador) && totalPartes < 10) {
        resultado[totalPartes++] = parte;
    }

    return resultado;
}

void Archivo::cargarAlojamientos(Alojamiento**& alojamientos,
                                 int& cantidad,
                                 Anfitrion** anfitriones,
                                 int cantidadAnfitriones)
{
    ifstream archivo("alojamientos.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir alojamientos.txt" << endl;
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

        int nCampos = 0;
        string* tokens = split(linea, '|', nCampos);
        if (nCampos < 9) { delete[] tokens; continue; }

        // Campos:
        // 0: código
        // 1: nombre
        // 2: documento anfitrión
        // 3: departamento
        // 4: municipio
        // 5: tipo
        // 6: dirección
        // 7: precio
        // 8: amenidades CSV
        string codigo       = tokens[0];
        string nombre       = tokens[1];
        string docAnf       = tokens[2];
        string depto        = tokens[3];
        string municipio    = tokens[4];
        string tipo         = tokens[5];
        string direccion    = tokens[6];
        float precio        = stof(tokens[7]);

        // buscar puntero a anfitrión
        Anfitrion* anfit = nullptr;
        for (int i = 0; i < cantidadAnfitriones; ++i) {
            if (anfitriones[i]->getDocumento() == docAnf) {
                anfit = anfitriones[i];
                break;
            }
        }

        // partir amenidades
        int nAmen = 0;
        string* amenList = split(tokens[8], ',', nAmen);

        delete[] tokens;

        // asegurar capacidad
        if (cantidad == capacidad) {
            capacidad *= 2;
            Alojamiento** tmp = new Alojamiento*[capacidad];
            for (int i = 0; i < cantidad; ++i) tmp[i] = alojamientos[i];
            delete[] alojamientos;
            alojamientos = tmp;
        }

        // crear objeto
        alojamientos[cantidad++] =
            new Alojamiento(codigo, nombre, tipo, direccion,
                            depto, municipio, precio,
                            amenList, nAmen,
                            anfit);

        delete[] amenList;
    }

    archivo.close();
}
