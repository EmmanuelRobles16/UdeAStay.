#include "archivos.h"
#include "huesped.h"
#include <fstream>
#include <sstream>
#include <iostream>

void ArchivoManager::cargarHuespedes(Huesped**& huespedes, int& cantidad) {
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

string* ArchivoManager::split(const string& str, char delimitador, int& totalPartes) {
    totalPartes = 0;
    stringstream ss(str);
    string parte;
    string* resultado = new string[10];

    while (getline(ss, parte, delimitador) && totalPartes < 10) {
        resultado[totalPartes++] = parte;
    }

    return resultado;
}
