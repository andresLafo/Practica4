#include <iostream>
#include "Red.h"
#include "Enrutador.h"

using namespace std;

Red redGlobal;

bool pedirEnrutadores(char *nombres) {
    bool existe = true;

    for (int i = 0; i < 2; i++) {
        cout << "Ingrese el nombre del enrutador " << (i + 1) << ": ";
        cin >> nombres[i];

        if (!redGlobal.existeEnrutador(nombres[i])) {
            cout << "El enrutador '" << nombres[i] << "' no existe en la red." << endl;
            existe = false;
            break;
        }
    }

    return existe;
}

// ------------------------------------------------------------
// Programa principal
// ------------------------------------------------------------
int main() {
    int opcion = 0;

    do {
        cout << "\n==================== MENU PRINCIPAL ====================\n";
        cout << "1. Agregar enrutador\n";
        cout << "2. Eliminar enrutador\n";
        cout << "3. Ver enrutadores y vecinos\n";
        cout << "4. Ver tablas de enrutamiento\n";
        cout << "5. Agregar relacion (enlace)\n";
        cout << "6. Ver costo entre dos enrutadores\n";
        cout << "7. Ver ruta mas corta entre dos enrutadores\n";
        cout << "8. Cargar red desde archivo\n";
        cout << "9. Cambiar costo de un enlace\n";
        cout << "10. Generar red aleatoria\n";
        cout << "0. Salir\n";
        cout << "=========================================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cout << endl;

        switch (opcion) {
        case 1: {
            char nombre;
            cout << "Ingrese el nombre del nuevo enrutador: ";
            cin >> nombre;

            if (redGlobal.existeEnrutador(nombre)) {
                cout << "El enrutador " << nombre << " ya existe.\n";
            } else {
                redGlobal.agregarEnrutador(nombre);
                cout << "Enrutador '" << nombre << "' agregado correctamente.\n";
            }
            break;
        }

        case 2: {
            char nombre;
            cout << "Ingrese el nombre del enrutador a eliminar: ";
            cin >> nombre;

            if (redGlobal.existeEnrutador(nombre)) {
                redGlobal.eliminarEnrutador(nombre);
                cout << "Enrutador '" << nombre << "' eliminado correctamente.\n";
            } else {
                cout << "El enrutador '" << nombre << "' no existe.\n";
            }
            break;
        }

        case 3: { // Ver enrutadores
            redGlobal.verRed();
            break;
        }

        case 4: { // Ver tablas de enrutamiento
            redGlobal.verTablas();
            break;
        }

        case 5: { // Agregar relación
            char nombres[2];
            if (!pedirEnrutadores(nombres)) break;

            int costo;
            cout << "Ingrese el costo del enlace entre " << nombres[0] << " y " << nombres[1] << ": ";
            cin >> costo;

            if (costo > 0) {
                redGlobal.conectarEnrutadores(nombres[0], nombres[1], costo);
                cout << "Relacion agregada correctamente.\n";
            } else {
                cout << "El costo debe ser mayor que 0.\n";
            }
            break;
        }

        case 6: { // Ver costo entre dos routers
            char nombres[2];
            if (!pedirEnrutadores(nombres)) break;

            int costo = redGlobal.obtenerCosto(nombres[0], nombres[1]);
            if (costo == -1)
                cout << "No hay conexion entre " << nombres[0] << " y " << nombres[1] << ".\n";
            else
                cout << "Costo total de " << nombres[0] << " a " << nombres[1] << ": " << costo << endl;
            break;
        }

        case 7: { // Ver ruta más corta
            char nombres[2];
            if (!pedirEnrutadores(nombres)) break;

            string camino = "";
            redGlobal.obtenerCaminoMasCorto(nombres[0], nombres[1], camino);

            if (camino.empty())
                cout << "No hay ruta disponible entre esos enrutadores.\n";
            else
                cout << "La ruta mas corta de " << nombres[0] << " a " << nombres[1] << " es: "
                     << nombres[0] << " -> " << camino << endl;
            break;
        }

        case 8: { // Cargar red desde archivo
            string archivo;
            cout << "Ingrese el nombre del archivo (con extension .txt): ";
            cin >> archivo;

            int resultado = redGlobal.cargarDesdeArchivo(archivo);
            if (resultado == 1)
                cout << "Red cargada exitosamente desde el archivo.\n";
            else
                cout << "Error al intentar cargar el archivo.\n";
            break;
        }

        case 9: { // Cambiar costo de un enlace
            char nombres[2];
            if (!pedirEnrutadores(nombres)) break;

            int nuevoCosto;
            cout << "Ingrese el nuevo costo para la conexion entre " << nombres[0] << " y " << nombres[1] << ": ";
            cin >> nuevoCosto;

            if (nuevoCosto <= 0) {
                cout << "El costo debe ser mayor que 0.\n";
            } else {
                Enrutador A = redGlobal.obtenerEnrutador(nombres[0]);
                Enrutador B = redGlobal.obtenerEnrutador(nombres[1]);

                if (!A.existeVecino(nombres[1])) {
                    cout << "Los enrutadores no estan conectados.\n";
                } else {
                    A.actualizarCostoVecino(nombres[1], nuevoCosto);
                    B.actualizarCostoVecino(nombres[0], nuevoCosto);

                    redGlobal.actualizarEnrutador(A);
                    redGlobal.actualizarEnrutador(B);
                    redGlobal.crearTablas();

                    cout << "Costo actualizado correctamente.\n";
                }
            }
            break;
        }

        case 10: { // Generar red aleatoria
            redGlobal.generarAleatoria();
            break;
        }

        case 0:
            cout << "Saliendo del programa...\n";
            break;

        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
            break;
        }

    } while (opcion != 0);

    return 0;
}
