#include "Plataforma.h"
#include "Alojamiento.h"
#include "Fecha.h"
#include <cstdio>
#include <cstring>

extern Reservacion** reservasVigentes;
extern int           cantidadVigentes;
extern Reservacion** reservasHistoricas;
extern int           cantidadHistoricas;

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
    int  opc = -1;
    char input[16];

    do {
        std::printf("\n=== Menú Anfitrión ===\n");
        std::printf("1) Mis alojamientos\n");
        std::printf("2) Reservas vigentes\n");
        std::printf("3) Reservas históricas\n");
        std::printf("4) Anular reserva\n");
        std::printf("5) Consultar reservas por rango de fechas\n");
        std::printf("6) Actualizar histórico\n");
        std::printf("0) Salir\n");
        std::printf("Opción: ");
        if (!std::fgets(input, sizeof(input), stdin)) break;
        opc = std::atoi(input);

        switch (opc) {
        case 1:
            anfitrion->mostrarAlojamientos();
            break;

        case 2: {
            // Verificar si este anfitrión tiene alguna reserva vigente
            bool tiene = false;
            for (int i = 0; i < cantidadVigentes; ++i) {
                if (reservasVigentes[i]->getAlojamiento()->getAnfitrion() == anfitrion) {
                    tiene = true;
                    break;
                }
            }
            if (!tiene) {
                std::printf("(No tienes reservas vigentes)\n");
            } else {
                anfitrion->mostrarAlojamientosYReservas(
                    alojamientos, cantidadAlojamientos, false
                    );
            }
            break;
        }

        case 3:
            anfitrion->mostrarAlojamientosYReservas(
                alojamientos, cantidadAlojamientos, true
                );
            break;

        case 4: {
            char codigo[64];
            std::printf("Código de reserva a anular: ");
            if (std::fgets(codigo, sizeof(codigo), stdin)) {
                codigo[std::strcspn(codigo, "\r\n")] = '\0';
                anfitrion->anularReservacion(
                    codigo,
                    reservasVigentes,
                    cantidadVigentes
                    );
            }
            break;
        }
        case 5: {
            char buf[16];
            // Leer fecha inicio
            printf("Fecha inicio (YYYY-MM-DD): ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\r\n")] = '\0';
            Fecha desde = Fecha::fromShortString(buf);

            // Leer fecha fin
            printf("Fecha fin    (YYYY-MM-DD): ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\r\n")] = '\0';
            Fecha hasta = Fecha::fromShortString(buf);

            // Si el usuario se confundió y puso fin < inicio, las intercambiamos
            if (hasta.esAnterior(desde)) {
                Fecha tmp = desde;
                desde = hasta;
                hasta = tmp;
                printf("↔ Fecha fin anterior a inicio: invirtiendo rango a [%04d-%02d-%02d … %04d-%02d-%02d]\n",
                       desde.getAnio(), desde.getMes(), desde.getDia(),
                       hasta.getAnio(), hasta.getMes(), hasta.getDia());
            }

            // Llamada a la Funcionalidad V
            anfitrion->mostrarReservasPorRango(
                alojamientos,
                cantidadAlojamientos,
                desde,
                hasta
                );
            break;
        }case 6:
            actualizarHistorico(
                anfitrion,
                reservasVigentes,   cantidadVigentes,
                reservasHistoricas, cantidadHistoricas
                );
            break;
        case 0:
            std::printf("Saliendo menú anfitrión.\n");
            break;

        default:
            std::printf("Opción inválida.\n");
        }
    } while (opc != 0);
}

void Plataforma::actualizarHistorico(
    Anfitrion*  anfitrion,
    Reservacion**&  reservasVig,     int& cantidadVig,
    Reservacion**&  reservasHist,    int& cantidadHist
    ) {
    // 1) Leer fecha de corte
    char buf[16];
    printf("Fecha de corte (YYYY-MM-DD): ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    buf[strcspn(buf, "\r\n")] = '\0';
    Fecha corte = Fecha::fromShortString(buf);

    // 2) Validar que corte ≥ última histórica
    Fecha ultimaHist = corte;
    if (cantidadHist > 0) {
        // buscar la máxima fechaSalida en historico
        ultimaHist = reservasHist[0]->fechaSalida();
        for (int i = 1; i < cantidadHist; ++i) {
            Fecha fS = reservasHist[i]->fechaSalida();
            if (ultimaHist.esAnterior(fS))
                ultimaHist = fS;
        }
    }
    if (corte.esAnterior(ultimaHist)) {
        printf("❌ Fecha de corte debe ser ≥ última histórica (%04d-%02d-%02d).\n",
               ultimaHist.getAnio(), ultimaHist.getMes(), ultimaHist.getDia());
        return;
    }

    // 3) Iterar sobre reservas vigentes y seleccionar a mover
    Reservacion** nuevasVig = nullptr;
    int nuevasCap = 0, nuevasCnt = 0;
    Reservacion** moved   = nullptr;
    int movedCap = 0, movedCnt = 0;

    for (int i = 0; i < cantidadVig; ++i) {
        Reservacion* r = reservasVig[i];
        Fecha fSal    = r->fechaSalida();

        if (fSal.esAnterior(corte)) {
            //  – A mover a histórico
            if (movedCnt == movedCap) {
                movedCap = movedCap? movedCap*2 : 4;
                moved = (Reservacion**)std::realloc(moved,
                                                      sizeof(Reservacion*) * movedCap);
            }
            moved[movedCnt++] = r;
        } else {
            //  – Queda en vigentes
            if (nuevasCnt == nuevasCap) {
                nuevasCap = nuevasCap? nuevasCap*2 : 4;
                nuevasVig = (Reservacion**)std::realloc(nuevasVig,
                                                          sizeof(Reservacion*) * nuevasCap);
            }
            nuevasVig[nuevasCnt++] = r;
        }
    }

    // 4) Actualizar arrays en memoria
    std::free(reservasVig);
    reservasVig  = nuevasVig;
    cantidadVig  = nuevasCnt;

    // a) añadir moved[] a reservasHist
    for (int i = 0; i < movedCnt; ++i) {
        Reservacion* r = moved[i];
        // append to historicas
        if (cantidadHist && reservasHist == nullptr) cantidadHist = 0;
        int newCap = cantidadHist? cantidadHist*2 : 4;
        reservasHist = (Reservacion**)std::realloc(reservasHist,
                                                     sizeof(Reservacion*) * newCap);
        reservasHist[cantidadHist++] = r;
    }
    std::free(moved);

    // 5) Apendea moved[] al archivo histórico
    FILE* fHist = std::fopen("reservas_historico.txt", "a");
    if (fHist) {
        for (int i = 0; i < cantidadHist; ++i) {
            Reservacion* r = reservasHist[i];
            // Sólo apendeamos las recién movidas
            // (su código comienza con R)
            char cod[16], alo[32], hue[32], met[16], nota[1001];
            r->getCodigo(cod, sizeof(cod));
            r->getAlojamiento()->getCodigo(alo, sizeof(alo));
            r->getHuesped()->getDocumento(hue, sizeof(hue));
            Fecha fe = r->getFechaEntrada();
            int dur = r->getDuracion();
            r->getMetodoPago(met, sizeof(met));
            Fecha fp = r->getFechaPago();
            float monto = r->getMonto();
            r->getAnotacion(nota, sizeof(nota));

            std::fprintf(fHist,
                         "%s|%s|%s|%04d-%02d-%02d|%d|%s|%04d-%02d-%02d|%.2f|%s\n",
                         cod, alo, hue,
                         fe.getAnio(), fe.getMes(), fe.getDia(),
                         dur,
                         met,
                         fp.getAnio(), fp.getMes(), fp.getDia(),
                         monto,
                         nota
                         );
        }
        std::fclose(fHist);
    }

    // 6) Reescribir el archivo de vigentes con reservasVig[]
    FILE* fVig = std::fopen("reservas_vigentes.txt", "w");
    if (fVig) {
        for (int i = 0; i < cantidadVig; ++i) {
            Reservacion* r = reservasVig[i];
            char cod[16], alo[32], hue[32], met[16], nota[1001];
            r->getCodigo(cod, sizeof(cod));
            r->getAlojamiento()->getCodigo(alo, sizeof(alo));
            r->getHuesped()->getDocumento(hue, sizeof(hue));
            Fecha fe = r->getFechaEntrada();
            int dur = r->getDuracion();
            r->getMetodoPago(met, sizeof(met));
            Fecha fp = r->getFechaPago();
            float monto = r->getMonto();
            r->getAnotacion(nota, sizeof(nota));

            std::fprintf(fVig,
                         "%s|%s|%s|%04d-%02d-%02d|%d|%s|%04d-%02d-%02d|%.2f|%s\n",
                         cod, alo, hue,
                         fe.getAnio(), fe.getMes(), fe.getDia(),
                         dur,
                         met,
                         fp.getAnio(), fp.getMes(), fp.getDia(),
                         monto,
                         nota
                         );
        }
        std::fclose(fVig);
    }

    printf("✅ Histórico actualizado: %d movidas.\n", movedCnt);
}

void Plataforma::reservarAlojamiento(
    Huesped*        huesped,
    Reservacion**&  reservas,     int& cantidadRes,
    Alojamiento**   alojamientos, int cantidadAl
    ) {
    char buf[256];

    // 1) Criterios de búsqueda
    char municipio[64];
    printf("Municipio: ");
    fgets(municipio, sizeof(municipio), stdin);
    municipio[strcspn(municipio,"\r\n")] = '\0';

    char fechaBuf[16];
    printf("Fecha entrada (YYYY-MM-DD): ");
    fgets(fechaBuf, sizeof(fechaBuf), stdin);
    fechaBuf[strcspn(fechaBuf,"\r\n")] = '\0';
    Fecha fechaEntrada = Fecha::fromShortString(fechaBuf);

    char durBuf[8];
    printf("Noches de estadía: ");
    fgets(durBuf, sizeof(durBuf), stdin);
    int duracion = atoi(durBuf);

    // 2) Filtros opcionales
    float maxPrecio = -1.0f;
    printf("¿Filtro precio máximo por noche? (s/n): ");
    fgets(buf,sizeof(buf),stdin);
    if (buf[0]=='s'||buf[0]=='S') {
        printf("Precio máximo: ");
        fgets(buf,sizeof(buf),stdin);
        maxPrecio = atof(buf);
    }

    float minPunt = -1.0f;
    printf("¿Filtro puntuación mínima anfitrión? (s/n): ");
    fgets(buf,sizeof(buf),stdin);
    if (buf[0]=='s'||buf[0]=='S') {
        printf("Puntuación mínima: ");
        fgets(buf,sizeof(buf),stdin);
        minPunt = atof(buf);
    }

    // 3) Listar candidatos
    printf("\n--- Alojamientos disponibles ---\n");
    Alojamiento** lista = nullptr;
    int encontrados = 0, capacidad = 0;
    for (int i = 0; i < cantidadAl; ++i) {
        // municipio
        char muniBuf[64]; alojamientos[i]->getMunicipio(muniBuf,sizeof(muniBuf));
        if (strcmp(muniBuf,municipio)!=0) continue;
        // disponibilidad
        if (!alojamientos[i]->estaDisponible(fechaEntrada,duracion)) continue;
        // precio
        float p = alojamientos[i]->getPrecioPorNoche();
        if (maxPrecio>=0 && p>maxPrecio) continue;
        // puntuación
        float pt = alojamientos[i]->getAnfitrion()->getPuntuacion();
        if (minPunt>=0 && pt<minPunt) continue;
        // añadir
        if (encontrados==capacidad) {
            capacidad = capacidad?capacidad*2:4;
            lista = (Alojamiento**)realloc(lista,sizeof(Alojamiento*)*capacidad);
        }
        lista[encontrados++] = alojamientos[i];
        // mostrar provisional
        char cod[32];
        alojamientos[i]->getCodigo(cod,sizeof(cod));
        printf("%d) %s — %.2f por noche — Host: %.2f\n",
               encontrados, cod, p, pt);
    }
    if (encontrados==0) {
        printf("No hay alojamientos que cumplan esos criterios.\n\n");
        free(lista);
        return;
    }

    // 4) Selección
    printf("Elige número (1-%d): ", encontrados);
    fgets(buf,sizeof(buf),stdin);
    int sel = atoi(buf);
    if (sel<1 || sel>encontrados) {
        printf("Selección inválida.\n\n");
        free(lista);
        return;
    }
    Alojamiento* elegido = lista[sel-1];
    free(lista);

    // 5) Datos de pago y anotación
    char metodo[16], pagoBuf[16], anot[1001];
    printf("Método de pago (PSE/TCrédito): ");
    fgets(metodo,sizeof(metodo),stdin);
    metodo[strcspn(metodo,"\r\n")] = '\0';
    printf("Fecha pago (YYYY-MM-DD): ");
    fgets(pagoBuf,sizeof(pagoBuf),stdin);
    pagoBuf[strcspn(pagoBuf,"\r\n")] = '\0';
    Fecha fechaPago = Fecha::fromShortString(pagoBuf);
    printf("Anotaciones (hasta 1000 chars): ");
    fgets(anot,sizeof(anot),stdin);
    anot[strcspn(anot,"\r\n")] = '\0';

    // 6) Código y monto
    char codRes[16];
    snprintf(codRes,sizeof(codRes),"R%03d",cantidadRes+1);
    float monto = elegido->getPrecioPorNoche() * duracion;

    // 7) Crear y almacenar en memoria
    Reservacion* nueva = new Reservacion(
        codRes, fechaEntrada, duracion,
        metodo, fechaPago, monto, anot,
        elegido, huesped
        );
    // global vigentes
    if (cantidadRes==0 && reservas==nullptr) { /* nada */ }
    {
        int newCap = cantidadRes? cantidadRes*2:4;
        reservas = (Reservacion**)realloc(reservas,sizeof(Reservacion*)*newCap);
    }
    reservas[cantidadRes++] = nueva;
    // en alojamiento y huésped
    elegido->agregarReserva(nueva);
    huesped->agregarReserva(nueva);

    // 8) Persistir
    FILE* f = fopen("reservas_vigentes.txt","a");
    if (f) {
        char alo[32], doc[32];
        elegido->getCodigo(alo,sizeof(alo));
        huesped->getDocumento(doc,sizeof(doc));
        fprintf(f,"%s|%s|%s|%04d-%02d-%02d|%d|%s|%04d-%02d-%02d|%.2f|%s\n",
                codRes, alo, doc,
                fechaEntrada.getAnio(),fechaEntrada.getMes(),fechaEntrada.getDia(),
                duracion,
                metodo,
                fechaPago.getAnio(),fechaPago.getMes(),fechaPago.getDia(),
                monto,
                anot
                );
        fclose(f);
    }

    // 9) Comprobante
    char comp[256];
    nueva->toComprobante(comp,sizeof(comp));
    printf("\n=== Reserva confirmada ===\n%s\n\n", comp);
}

void Plataforma::runMenuHuesped(
    Huesped*        huesped,
    Reservacion**   reservas,     int cantidadRes,
    Reservacion**   historico,    int cantidadHist,
    Alojamiento**   alojamientos,  int cantidadAl
    ) {
    int opc = -1;
    char input[16];

    do {
        printf("\n=== Menú Huésped ===\n");
        printf("1) Mostrar mis reservas activas\n");
        printf("2) Mostrar mis reservas históricas\n");
        printf("3) Anular reserva\n");
        printf("4) Reservar alojamiento\n");
        printf("0) Salir\n");
        printf("Opción: ");
        if (!fgets(input,sizeof(input),stdin)) break;
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
        case 4:
            Plataforma::reservarAlojamiento(
                huesped,
                reservas,    cantidadRes,
                alojamientos, cantidadAl
                );
            break;
        case 0:
            printf("Cerrando sesión de huésped.\n");
            break;

        default:
            printf("Opción inválida, intenta de nuevo.\n");
        }

    } while (opc != 0);
}

void Plataforma::buscarAlojamientosDisponibles(
    Alojamiento** alojamientos,
    int           cantidadAl,
    const char*   municipio,
    const Fecha&  fechaEntrada,
    int           duracion
    ) {
    // 1) Obtener la fecha de hoy
    Fecha hoy = Fecha::hoy();

    // 2) Validar que fechaEntrada sea válida y NO anterior a hoy
    if (!fechaEntrada.validar() || fechaEntrada.esAnterior(hoy)) {
        std::printf(
            "❌ Fecha inválida: debe ser hoy (%04d-%02d-%02d) o posterior.\n",
            hoy.getAnio(), hoy.getMes(), hoy.getDia()
            );
        return;
    }

    // 3) Calcular límite de 365 días desde fechaEntrada
    Fecha tope = fechaEntrada;
    tope.sumarDias(365);

    std::printf(
        "🔍 Disponibilidad en %s desde %04d-%02d-%02d por %d noches:\n",
        municipio,
        fechaEntrada.getAnio(), fechaEntrada.getMes(), fechaEntrada.getDia(),
        duracion
        );

    bool alguno = false;

    // 4) Recorrer todos los alojamientos
    for (int i = 0; i < cantidadAl; ++i) {
        // 4.1) Filtro por municipio
        char muniBuf[64];
        alojamientos[i]->getMunicipio(muniBuf, sizeof(muniBuf));
        if (std::strcmp(muniBuf, municipio) != 0)
            continue;

        // 4.2) Comprobar que la fecha de salida no supere los 365 días
        Fecha finReserva = fechaEntrada;
        finReserva.sumarDias(duracion);
        if (tope.esAnterior(finReserva))
            continue;

        // —— DEBUG ——
        // Muestra para cada alojamiento si pasa la disponibilidad
        char codDebug[32];
        alojamientos[i]->getCodigo(codDebug, sizeof(codDebug));
        int disp = alojamientos[i]->estaDisponible(fechaEntrada, duracion);
        std::printf("DEBUG: probando %s → estaDisponible=%d\n", codDebug, disp);

        // 4.3) Si no está disponible, lo descartamos
        if (!disp)
            continue;

        // 4.4) Si llegó hasta aquí, imprimir el alojamiento libre
        char cod[32], nom[128];
        alojamientos[i]->getCodigo(cod, sizeof(cod));
        alojamientos[i]->getNombre(nom, sizeof(nom));
        std::printf(
            "  • %s — %s (%.2f por noche)\n",
            cod, nom,
            alojamientos[i]->getPrecioPorNoche()
            );
        alguno = true;
    }

    if (!alguno) {
        std::printf("😔 No hay alojamientos disponibles con esos criterios.\n");
    }
}


