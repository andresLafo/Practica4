#include "Red.h"

Red::Red() {}

void Red::agregarEnrutador(char nombre) {

    Enrutador nuevo(nombre);
    nuevo.agregarVecino(nombre, 0);
    this->enrutadores.push_back(nuevo);
}

bool Red::existeEnrutador(char nombre) {
    for (auto &r : this->enrutadores) {
        if (r.obtenerNombre() == nombre)
            return true;
    }
    return false;
}

Enrutador Red::obtenerEnrutador(char nombre) {
    for (auto &r : this->enrutadores) {
        if (r.obtenerNombre() == nombre)
            return r;
    }
    return Enrutador('?');
}

void Red::actualizarEnrutador(Enrutador obj) {
    for (auto &r : this->enrutadores) {
        if (r.obtenerNombre() == obj.obtenerNombre()) {
            r = obj;
            break;
        }
    }
}


void Red::eliminarEnrutador(char nombre) {
    for (auto it = this->enrutadores.begin(); it != this->enrutadores.end(); ++it) {
        if (it->obtenerNombre() == nombre) {
            this->enrutadores.erase(it);
            break;
        }
    }

    this->actualizarRelaciones(nombre);
    this->crearTablas();
}


void Red::conectarEnrutadores(char origen, char destino, int costo) {
    Enrutador A = this->obtenerEnrutador(origen);
    Enrutador B = this->obtenerEnrutador(destino);

    A.agregarVecino(destino, costo);
    B.agregarVecino(origen, costo);

    this->actualizarEnrutador(A);
    this->actualizarEnrutador(B);

    this->crearTablas();
}

void Red::desconectarEnrutadores(char origen, char destino) {
    Enrutador A = this->obtenerEnrutador(origen);
    Enrutador B = this->obtenerEnrutador(destino);

    A.eliminarVecino(destino);
    B.eliminarVecino(origen);

    this->actualizarEnrutador(A);
    this->actualizarEnrutador(B);

    this->crearTablas();
}


void Red::actualizarRelaciones(char nombreEliminado) {
    for (auto &r : this->enrutadores) {
        if (r.existeVecino(nombreEliminado)) {
            r.eliminarVecino(nombreEliminado);
        }
    }
}


void Red::crearTablas() {
    vector<char> visitados;

    for (auto &routerOrigen : this->enrutadores) {
        routerOrigen.limpiarDestinos();

        for (auto &routerDestino : this->enrutadores) {
            visitados.clear();
            visitados.push_back(routerOrigen.obtenerNombre());

            map<char, int> resultado = this->calcularCamino(routerOrigen, routerDestino, visitados, 0, 0, true);

            if (resultado.begin()->second == 1000)
                routerOrigen.definirRuta(resultado.begin()->first, routerDestino.obtenerNombre(), -1);
            else
                routerOrigen.definirRuta(resultado.begin()->first, routerDestino.obtenerNombre(), resultado.begin()->second);
        }

        this->actualizarEnrutador(routerOrigen);
    }
}


map<char, int> Red::calcularCamino(Enrutador inicio, Enrutador fin, vector<char> visitados, int costoAcum, int anterior, bool primerPaso) {
    map<char, int> resultado;
    resultado[inicio.obtenerNombre()] = 1000; // valor grande por defecto

    if (inicio.obtenerNombre() == fin.obtenerNombre()) {
        resultado[inicio.obtenerNombre()] = 0;
        return resultado;
    }

    map<char, int> vecinos = inicio.obtenerVecinosFiltrados(visitados);

    int costoTemporal = 0;
    for (auto const &v : vecinos) {
        if (primerPaso && visitados.size() > 1)
            visitados.pop_back();

        Enrutador siguiente = this->obtenerEnrutador(v.first);

        // Si el vecino es el destino directo
        if (v.first == fin.obtenerNombre()) {
            if ((costoTemporal - anterior) > 0)
                costoAcum = (costoTemporal - anterior);

            costoAcum += (v.second + anterior);

            if (costoAcum < resultado.begin()->second) {
                resultado.clear();
                resultado[v.first] = costoAcum;
            }

            costoAcum = 0;
        }
        else {
            visitados.push_back(v.first);
            costoAcum += anterior;

            map<char, int> subRuta = this->calcularCamino(siguiente, fin, visitados, costoAcum, v.second, false);

            if (subRuta.begin()->second < resultado.begin()->second) {
                resultado.clear();
                resultado[v.first] = subRuta.begin()->second;
            }

            costoTemporal = costoAcum;
            costoAcum = 0;
        }
    }

    return resultado;
}

void Red::verRed() {
    cout << endl << "La red contiene " << this->enrutadores.size() << " enrutador(es)." << endl;

    for (auto &r : this->enrutadores) {
        cout << endl << "Enrutador " << r.obtenerNombre() << ":" << endl;

        map<char, int> vecinos = r.obtenerVecinosFiltrados({});
        for (auto const &v : vecinos) {
            cout << "   - " << v.first << " con costo de: " << v.second << endl;
        }
    }
}

void Red::verTablas() {
    for (auto &r : this->enrutadores) {
        cout << endl << "Tabla de enrutamiento de " << r.obtenerNombre() << ":" << endl;

        map<char, map<char, int>> tabla = r.obtenerDestinos();

        for (auto const &fila : tabla) {
            cout << "| Destino " << fila.first << ": ";

            map<char, int> entrada = fila.second;
            if (entrada.begin()->second == -1)
                cout << "sin ruta";
            else
                cout << "siguiente salto " << entrada.begin()->first << ", costo total " << entrada.begin()->second;

            cout << endl;
        }
    }
}


int Red::obtenerCosto(char origen, char destino) {
    Enrutador r = this->obtenerEnrutador(origen);
    map<char, map<char, int>> tabla = r.obtenerDestinos();

    for (auto const &fila : tabla) {
        if (fila.first == destino)
            return fila.second.begin()->second;
    }

    return -1;
}


void Red::obtenerCaminoMasCorto(char origen, char destino, string &camino) {
    Enrutador r = this->obtenerEnrutador(origen);
    map<char, map<char, int>> tabla = r.obtenerDestinos();

    for (auto const &fila : tabla) {
        if (fila.first == destino) {
            char siguiente = fila.second.begin()->first;
            camino += siguiente;

            if (siguiente == destino)
                break;

            obtenerCaminoMasCorto(siguiente, destino, camino);
        }
    }
}

int Red::cargarDesdeArchivo(string nombreArchivo) {
    ifstream archivo("C:/Users/Andres Felipe/OneDrive/Escritorio/informatica II Andres Lafaurie/INFORMATICA 2/Practica4/Practica4/red1.txt");
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el archivo." << endl;
        return -1;
    }

    char origen, destino;
    int costo;

    while (archivo >> origen >> destino >> costo) {
        if (!this->existeEnrutador(origen))
            this->agregarEnrutador(origen);
        if (!this->existeEnrutador(destino))
            this->agregarEnrutador(destino);

        this->conectarEnrutadores(origen, destino, costo);
    }

    archivo.close();
    cout << "Archivo cargado correctamente." << endl;
    return 1;
}

void Red::generarAleatoria() {
    string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(NULL));

    int cantidad = 2 + rand() % 9; // entre 2 y 10 routers
    vector<char> nombres;

    for (int i = 0; i < cantidad; ++i) {
        char nombre = letras[i];
        this->agregarEnrutador(nombre);
        nombres.push_back(nombre);
    }

    int conexiones = cantidad + rand() % (cantidad * 2);

    for (int i = 0; i < conexiones; ++i) {
        char a = nombres[rand() % cantidad];
        char b = nombres[rand() % cantidad];
        if (a == b) continue;
        int costo = 1 + rand() % 19;

        this->conectarEnrutadores(a, b, costo);
    }

    cout << "Red aleatoria generada con " << cantidad << " enrutadores." << endl;
}
