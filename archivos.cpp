#include "archivos.h"
#include "huesped.h"
#include "anfitrion.h"
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
