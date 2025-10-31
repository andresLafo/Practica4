#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Enrutador {
private:
    char nombre;
    map<char, int> vecinos;
    map<char, map<char, int>> destinos;

public:
    Enrutador(char nombre_);

    char obtenerNombre();

    int cantidadDeVecinos();

    void agregarVecino(char nombreVecino, int costo);

    void eliminarVecino(char nombreVecino);

    bool existeVecino(char nombreVecino);

    void actualizarCostoVecino(char nombreVecino, int nuevoCosto);

    void limpiarDestinos();

    void definirRuta(char siguienteSalto, char destino, int costo);

    map<char, map<char, int>> obtenerDestinos();

    map<char, int> obtenerVecinosFiltrados(const vector<char> &visitados);

};

#endif // ENRUTADOR_H
