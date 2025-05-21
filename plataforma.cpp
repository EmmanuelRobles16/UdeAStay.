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
void Plataforma::runMenuAnfitrion(
    Anfitrion* anfitrion,
    Alojamiento** alojamientos, int cantL,
    Reservacion** vigentes,     int cantV,
    Reservacion** historico,    int cantHisto
    ) {
    int opc;
    do {
        cout << "\n=== Menú Anfitrión ===\n"
             << "1) Mis alojamientos\n"
             << "2) Reservas vigentes\n"
             << "3) Reservas históricas\n"
             << "4) Anular reserva vigente\n"
             << "0) Salir\n"
             << "Opción: ";
        cin >> opc;

        switch (opc) {
        case 1:
            anfitrion->mostrarAlojamientos();
            break;

        case 2:
            cout << "\n-- Vigentes --\n";
            for (int i = 0; i < cantV; ++i) {
                if (vigentes[i]->getAlojamiento()->getAnfitrion() == anfitrion)
                    cout << " • " << vigentes[i]->comprobante() << "\n";
            }
            break;

        case 3:
            cout << "\n-- Históricas --\n";
            for (int i = 0; i < cantHisto; ++i) {
                if (historico[i]->getAlojamiento()->getAnfitrion() == anfitrion)
                    cout << " • " << historico[i]->comprobante() << "\n";
            }
            break;

        case 4: {
            cout << "Código de reserva a anular: ";
            string cod; cin >> cod;
            bool exito = false;
            for (int i = 0; i < cantV; ++i) {
                if (vigentes[i]->getCodigo() == cod &&
                    vigentes[i]->getAlojamiento()->getAnfitrion() == anfitrion)
                {
                    anfitrion->anularReservacion(
                        cod, alojamientos, cantL
                        );
                    cout << "Reserva " << cod << " anulada.\n";
                    exito = true;
                    break;
                }
            }
            if (!exito) cout << "No existe esa reserva.\n";
            break;
        }

        case 0:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opción inválida.\n";
        }
    } while (opc != 0);
}

//menu huesped
void Plataforma::runMenuHuesped(Huesped* huesped, Reservacion** reservas, int cantidadRes) {
    int opc;
    do {
        cout << "\n=== Menú Huésped ===\n"
             << "1) Mostrar mis reservas activas\n"
             << "0) Salir\n"
             << "Elige opción: ";
        cin >> opc;

        switch (opc) {
        case 1: {
            cout << "\nTus reservas activas:\n";
            const string docUsu = huesped->getDocumento();
            bool found = false;
            for (int i = 0; i < cantidadRes; ++i) {
                if (reservas[i]->esDelHuesped(docUsu)) {
                    cout << "  - " << reservas[i]->getResumen() << "\n";
                    found = true;
                }
            }
            if (!found)
                cout << "  (no tienes reservas activas)\n";
            break;
        }
        case 0:
            cout << "Cerrando sesión de huésped...\n";
            break;

        default:
            cout << "Opción inválida, intenta de nuevo.\n";
        }
    } while (opc != 0);
}
