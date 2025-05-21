#include "Anfitrion.h"
#include "alojamiento.h"
#include "reservacion.h"
#include <iostream>

Anfitrion::Anfitrion(const string& doc, int antig, float punt,
                     const string& pwd, string* codigos, int numCodigos)
    : documento(doc), antiguedad(antig), puntuacion(punt),
    password(pwd), cantidadAlojamientos(numCodigos),
    capacidadAlojamientos(numCodigos)
{
    codigosAlojamientos = new string[capacidadAlojamientos];
    for (int i = 0; i < cantidadAlojamientos; ++i)
        codigosAlojamientos[i] = codigos[i];
}

Anfitrion::~Anfitrion() {
    delete[] codigosAlojamientos;
}

string Anfitrion::getDocumento() const { return documento; }
int    Anfitrion::getAntiguedad() const { return antiguedad; }
float  Anfitrion::getPuntuacion() const { return puntuacion; }
string Anfitrion::getPassword() const  { return password; }  // ← impl.


void Anfitrion::mostrarAlojamientos() const {
    cout << "Alojamientos del anfitrión " << documento << ":\n";
    for (int i = 0; i < cantidadAlojamientos; ++i)
        cout << "  - " << codigosAlojamientos[i] << "\n";
}
//mostrar alojamientos con sus reservas

void Anfitrion::mostrarAlojamientosYReservas(Alojamiento** todosAlojamientos,
                                             int cantidadTotal) const
{
    cout << "=== Alojamientos de " << documento
         << " (puntuación " << puntuacion << ") ===\n";

    // Por cada código que administra este anfitrión
    for (int k = 0; k < cantidadAlojamientos; ++k) {
        const string& cod = codigosAlojamientos[k];

        // Buscar en el array global el objeto Alojamiento*
        for (int i = 0; i < cantidadTotal; ++i) {
            if (todosAlojamientos[i]->getCodigo() == cod) {
                Alojamiento* alo = todosAlojamientos[i];
                cout << "- " << alo->getCodigo()
                     << ": " << alo->getNombre() << "\n";
                // Delegamos a Alojamiento sus reservas
                alo->mostrarReservas();
                break;
            }
        }
    }
    cout << "----------------------------------------\n";
}

void Anfitrion::anularReservacion(const string& codigoReserva,
                                  Alojamiento** todosAloj,
                                  int cantidadAloj) const
{
    bool encontrado = false;

    // Recorro cada código de alojamiento que maneja este anfitrión
    for (int k = 0; k < cantidadAlojamientos && !encontrado; ++k) {
        const string& codAloj = codigosAlojamientos[k];

        // Busco el objeto Alojamiento* en el array global
        for (int i = 0; i < cantidadAloj; ++i) {
            if (todosAloj[i]->getCodigo() == codAloj) {
                // Intento anular la reserva dentro del Alojamiento
                todosAloj[i]->anularReserva(codigoReserva);
                encontrado = true;
                break;
            }
        }
    }

    if (encontrado) {
        cout << "Reserva \"" << codigoReserva
             << "\" anulada correctamente.\n";
    } else {
        cout << "No se encontró la reserva \"" << codigoReserva
             << "\" en ninguno de tus alojamientos.\n";
    }
}
