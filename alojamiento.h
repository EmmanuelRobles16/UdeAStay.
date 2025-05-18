#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>
using namespace std;

class Anfitrion;
class Reservacion;

class Alojamiento {
private:
    string codigo;
    string nombre;
    string tipo;
    string direccion;
    string departamento;
    string municipio;
    float precioPorNoche;

    // Amenidades
    string* amenidades;
    int cantidadAmenidades;

    // Lista dinámica de reservas propias
    Reservacion** reservas;
    int cantidadReservas;
    int capacidadReservas;

    // Relación con anfitrión
    Anfitrion* anfitrion;

public:
    // Constructor principal
    Alojamiento(const string& codigo,
                const string& nombre,
                const string& tipo,
                const string& direccion,
                const string& departamento,
                const string& municipio,
                float precioPorNoche,
                string* amenidadesArr,
                int cantidadAmenArr,
                Anfitrion* anfitrion);

    // Destructor
    ~Alojamiento();

    // Getters básicos
    const string& getCodigo() const;
    const string& getNombre() const;
    const string& getTipo() const;
    const string& getDireccion() const;
    const string& getDepartamento() const;
    const string& getMunicipio() const;
    float getPrecioPorNoche() const;
    Anfitrion* getAnfitrion() const;

    // Amenidades
    string* getAmenidades() const;
    int getCantidadAmenidades() const;

    // Reservas
    void agregarReserva(Reservacion* r);
    void anularReserva(const string& codigoReserva);
};

#endif // ALOJAMIENTO_H
