#include "Plataforma.h"


ResultadoLogin Plataforma::autenticar(
    Huesped** huespedes, int cantidadHuespedes,
    Anfitrion** anfitriones, int cantidadAnfitriones
    ) {
    char doc[64];
    char pwd[64];

    // Leer documento y contraseña con scanf
    printf("Documento: ");
    scanf("%63s", doc);
    printf("Contraseña: ");
    scanf("%63s", pwd);

    // ✅ Limpiar el búfer para evitar problemas con fgets() luego
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Intentar login como huésped
    for (int i = 0; i < cantidadHuespedes; ++i) {
        char actualDoc[64], actualPwd[64];
        huespedes[i]->getDocumento(actualDoc, sizeof(actualDoc));
        huespedes[i]->getPassword(actualPwd, sizeof(actualPwd));

        actualDoc[strcspn(actualDoc, "\r\n")] = '\0';
        actualPwd[strcspn(actualPwd, "\r\n")] = '\0';

        if (strcmp(actualDoc, doc) == 0 && strcmp(actualPwd, pwd) == 0) {
            printf("✅ Bienvenido huésped %s\n", actualDoc);
            return { TipoUsuario::Huesped, huespedes[i], NULL };
        }
    }

    // Intentar login como anfitrión
    for (int i = 0; i < cantidadAnfitriones; ++i) {
        char actualDoc[64], actualPwd[64];
        anfitriones[i]->getDocumento(actualDoc, sizeof(actualDoc));
        anfitriones[i]->getPassword(actualPwd, sizeof(actualPwd));

        actualDoc[strcspn(actualDoc, "\r\n")] = '\0';
        actualPwd[strcspn(actualPwd, "\r\n")] = '\0';

        if (strcmp(actualDoc, doc) == 0 && strcmp(actualPwd, pwd) == 0) {
            printf("✅ Bienvenido anfitrión %s\n", actualDoc);
            return { TipoUsuario::Anfitrion, NULL, anfitriones[i] };
        }
    }

    printf("❌ Acceso denegado o usuario no encontrado.\n");
    return { TipoUsuario::Ninguno, NULL, NULL };
}

void Plataforma::runMenuAnfitrion(
    Anfitrion*    anfitrion,
    Alojamiento** alojamientos,
    int           cantidadAlojamientos
    ) {
    int opc = -1;
    char input[16];

    do {
        printf("\n=== Menú Anfitrión ===\n");
        printf("1) Mis alojamientos\n");
        printf("2) Reservas vigentes\n");
        printf("3) Reservas históricas\n");
        printf("4) Anular reserva\n");
        printf("0) Salir\n");
        printf("Opción: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        opc = atoi(input);

        switch (opc) {
        case 1:
            anfitrion->mostrarAlojamientos();
            break;
        case 2:
            // True = histórico, False = vigente
            anfitrion->mostrarAlojamientosYReservas(
                alojamientos, cantidadAlojamientos, false
                );
            break;
        case 3:
            anfitrion->mostrarAlojamientosYReservas(
                alojamientos, cantidadAlojamientos, true
                );
            break;
        case 4: {
            char codRes[64];
            printf("Código de reserva a anular: ");
            if (fgets(codRes, sizeof(codRes), stdin)) {
                codRes[strcspn(codRes, "\n")] = '\0';
                anfitrion->anularReservacion(
                    codRes, alojamientos, cantidadAlojamientos
                    );
            }
            break;
        }
        case 0:
            printf("Saliendo.\n");
            break;
        default:
            printf("Opción inválida.\n");
        }
    } while (opc != 0);
}

void Plataforma::runMenuHuesped(
    Huesped*        huesped,
    Reservacion**   reservas,     int cantidadRes,
    Reservacion**   historico,    int cantidadHist
    ) {
    int opc = -1;
    char input[16];

    do {
        printf("\n=== Menú Huésped ===\n");
        printf("1) Mostrar mis reservas activas\n");
        printf("2) Mostrar mis reservas históricas\n");
        printf("3) Anular reserva\n");               // <– nueva línea
        printf("0) Salir\n");
        printf("Elige opción: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        opc = atoi(input);

        char docUsu[64];
        huesped->getDocumento(docUsu, sizeof(docUsu));

        switch (opc) {
        case 1: {
            printf("\n>> Tus reservas activas:\n");
            int count = 0;
            for (int i = 0; i < cantidadRes; ++i) {
                if (reservas[i] && reservas[i]->esDelHuesped(docUsu)) {
                    char resumen[128];
                    reservas[i]->toResumen(resumen, sizeof(resumen));
                    printf("  - %s\n", resumen);
                    count++;
                }
            }
            if (count == 0) printf("  (no tienes reservas activas)\n");
            break;
        }

        case 2: {
            printf("\n>> Tus reservas históricas:\n");
            int count = 0;
            for (int i = 0; i < cantidadHist; ++i) {
                if (historico[i] && historico[i]->esDelHuesped(docUsu)) {
                    char resumen[128];
                    historico[i]->toResumen(resumen, sizeof(resumen));
                    printf("  - %s\n", resumen);
                    count++;
                }
            }
            if (count == 0) printf("  (no tienes reservas históricas)\n");
            break;
        }

        case 3: {  // <-- nueva opción
            char codRes[64];
            printf("Código de reserva a anular: ");
            if (fgets(codRes, sizeof(codRes), stdin)) {
                codRes[strcspn(codRes, "\r\n")] = '\0';
                huesped->anularReservacion(codRes, reservas, cantidadRes);
            }
            break;
        }

        case 0:
            printf("Cerrando sesión de huésped.\n");
            break;

        default:
            printf("Opción inválida, intenta de nuevo.\n");
        }

    } while (opc != 0);
}




