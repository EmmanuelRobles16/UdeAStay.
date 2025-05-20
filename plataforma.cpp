#include "PLATAFORMA.H"
#include <iostream>

using namespace std;

ResultadoLogin Plataforma::autenticar(Huesped** huespedes, int cantidadHuespedes, Anfitrion** anfitriones, int cantidadAnfitriones)
{
    string doc, pwd;
    cout << "Documento: ";
    cin  >> doc;
    cout << "Contraseña: ";
    cin  >> pwd;

    // Intentar login como huésped
    for (int i = 0; i < cantidadHuespedes; ++i) {
        if (huespedes[i]->getDocumento() == doc &&
            huespedes[i]->getPassword()  == pwd)
        {
            return { TipoUsuario::Huesped, huespedes[i], nullptr };
        }
    }

    // Intentar login como anfitrión
    for (int i = 0; i < cantidadAnfitriones; ++i) {
        if (anfitriones[i]->getDocumento() == doc &&
            anfitriones[i]->getPassword()  == pwd)
        {
            return { TipoUsuario::Anfitrion, nullptr, anfitriones[i] };
        }
    }

    // Ninguno coincide
    return { TipoUsuario::Ninguno, nullptr, nullptr };
}
//ahora por motivos de legilibilidad se iran a añadir dos menus, anfitiron y huesped
//se llama anfitrion y alojamientos porque son los datos para mostrar en este menu
void Plataforma::runMenuAnfitrion(Anfitrion* anfitrion, Alojamiento** alojamientos, int cantidadAlojamientos)
{
    int opc;
    do {
        cout << "\n=== Menú Anfitrión ===\n"
             << "1) Mostrar mis alojamientos\n"
             << "2) Mostrar mis alojamientos y reservas vigentes\n"
             << "0) Salir\n"
             << "Elige opción: ";
        cin >> opc;

        switch (opc) {
        case 1:
            anfitrion->mostrarAlojamientos();
            break;

        case 2:
            anfitrion->mostrarAlojamientosYReservas(
                alojamientos, cantidadAlojamientos
                );
            break;

        case 0:
            cout << "Cerrando sesión...\n";
            break;

        default:
            cout << "Opción inválida.\n";
        }
    } while (opc != 0);
}
