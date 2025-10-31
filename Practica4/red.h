#ifndef RED_H
#define RED_H

#include "Enrutador.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

class Red {
private:
    vector<Enrutador> enrutadores;  // Lista de todos los enrutadores que pertenecen a la red

public:
    Red();

    void agregarEnrutador(char nombre);           // Agrega un nuevo enrutador a la red

    void eliminarEnrutador(char nombre);          // Elimina un enrutador existente

    bool existeEnrutador(char nombre);            // Verifica si un enrutador ya existe

    Enrutador obtenerEnrutador(char nombre);      // Devuelve un enrutador específico

    void actualizarEnrutador(Enrutador obj);      // Reemplaza los datos de un enrutador existente

    void conectarEnrutadores(char origen, char destino, int costo);   // Crea conexión entre dos enrutadores

    void desconectarEnrutadores(char origen, char destino);           // Elimina enlace entre dos enrutadores

    void actualizarRelaciones(char nombreEliminado);                  // Quita referencias a un enrutador eliminado

    void crearTablas();                       // Genera o actualiza todas las tablas de enrutamiento

    map<char, int> calcularCamino(Enrutador inicio, Enrutador fin, vector<char> visitados, int costoAcum, int anterior, bool primerPaso);

    void verRed();                            // Muestra todos los enrutadores y sus vecinos

    void verTablas();                         // Muestra las tablas de enrutamiento de todos los enrutadores

    int obtenerCosto(char origen, char destino);   // Devuelve el costo total entre dos enrutadores

    void obtenerCaminoMasCorto(char origen, char destino, string &camino); // Devuelve el camino más corto en forma de texto

    int cargarDesdeArchivo(string nombreArchivo);  // Carga topología desde archivo (formato: A B 4)

    void generarAleatoria();                       // Crea red aleatoria para pruebas
};


#endif // RED_H
