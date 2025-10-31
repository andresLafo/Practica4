#include "Enrutador.h"

Enrutador::Enrutador(char nombre_) {
    this->nombre = nombre_;
}

char Enrutador::obtenerNombre() {
    return this->nombre;
}

int Enrutador::cantidadDeVecinos() {
    map<char, int> copia = this->vecinos;
    copia.erase(this->obtenerNombre());  // por si el router se auto-relaciona
    return copia.size();
}

void Enrutador::agregarVecino(char nombreVecino, int costo) {
    this->vecinos[nombreVecino] = costo;
}

void Enrutador::eliminarVecino(char nombreVecino) {
    this->vecinos.erase(nombreVecino);
}

bool Enrutador::existeVecino(char nombreVecino) {
    bool existe = false;
    for (auto const &par : this->vecinos) {
        if (par.first == nombreVecino) {
            existe = true;
            break;
        }
    }
    return existe;
}

void Enrutador::actualizarCostoVecino(char nombreVecino, int nuevoCosto) {
    this->vecinos[nombreVecino] = nuevoCosto;
}

void Enrutador::limpiarDestinos() {
    this->destinos.clear();
}

void Enrutador::definirRuta(char siguienteSalto, char destino, int costo) {
    map<char, int> temp;
    temp[siguienteSalto] = costo;
    this->destinos[destino] = temp;
}

map<char, map<char, int>> Enrutador::obtenerDestinos() {
    return this->destinos;
}

map<char, int> Enrutador::obtenerVecinosFiltrados(const vector<char> &visitados) {
    map<char, int> resultado = this->vecinos;

    for (auto it = visitados.begin(); it != visitados.end(); ++it) {
        resultado.erase(*it);
    }

    return resultado;
}
