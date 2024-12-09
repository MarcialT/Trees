#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Nodo {
    string nombre;
    string apellido;
    Nodo* hijo;
    Nodo* hermano;

    Nodo(const string& nombre, const string& apellido) : nombre(nombre), apellido(apellido), hijo(nullptr), hermano(nullptr) {}
};

class ArbolGenealogico {
private:
    Nodo* raiz;

    void guardarEnCSV(ofstream& archivo, Nodo* nodo) {
        if (nodo == nullptr) return;
        
        archivo << nodo->nombre << "," << nodo->apellido << "\n";
        guardarEnCSV(archivo, nodo->hijo);
        guardarEnCSV(archivo, nodo->hermano);
    }

    void guardarCSV() {
        ofstream archivo("family.csv");
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo.\n";
            return;
        }

        archivo << "Nombre,Apellido\n";
        guardarEnCSV(archivo, raiz);

        archivo.close();
        cout << "Arbol genealogico guardado en family.csv\n";
    }

public:
    ArbolGenealogico(const string& nombreRaiz, const string& apellidoRaiz) {
        raiz = new Nodo(nombreRaiz, apellidoRaiz);
        guardarCSV();
    }

    ~ArbolGenealogico() {
        limpiar(raiz);
    }

    void limpiar(Nodo* nodo) {
        if (nodo == nullptr) return;
        limpiar(nodo->hijo);
        limpiar(nodo->hermano);
        delete nodo;
    }

    Nodo* buscarNodo(Nodo* nodo, const string& nombre, const string& apellido) {
        if (nodo == nullptr) return nullptr;
        if (nodo->nombre == nombre && nodo->apellido == apellido) return nodo;
        
        Nodo* encontrado = buscarNodo(nodo->hijo, nombre, apellido);
        if (encontrado) return encontrado;

        return buscarNodo(nodo->hermano, nombre, apellido);
    }

    void agregarHijo(const string& nombrePadre, const string& apellidoPadre, const string& nombreHijo, const string& apellidoHijo) {
        Nodo* padre = buscarNodo(raiz, nombrePadre, apellidoPadre);
        if (padre == nullptr) {
            cout << "Padre no encontrado.\n";
            return;
        }

        if (padre->hijo == nullptr) {
            padre->hijo = new Nodo(nombreHijo, apellidoHijo);
        } else {
            Nodo* actual = padre->hijo;
            while (actual->hermano != nullptr) {
                actual = actual->hermano;
            }
            actual->hermano = new Nodo(nombreHijo, apellidoHijo);
        }
        guardarCSV();
    }

    void agregarHermano(const string& nombreHermanoMayor, const string& apellidoHermanoMayor, const string& nombreHermano, const string& apellidoHermano) {
        Nodo* hermanoMayor = buscarNodo(raiz, nombreHermanoMayor, apellidoHermanoMayor);
        if (hermanoMayor == nullptr) {
            cout << "Hermano mayor no encontrado.\n";
            return;
        }

        while (hermanoMayor->hermano != nullptr) {
            hermanoMayor = hermanoMayor->hermano;
        }
        hermanoMayor->hermano = new Nodo(nombreHermano, apellidoHermano);
        guardarCSV();
    }

    void mostrar(Nodo* nodo, int nivel = 0) {
        if (nodo == nullptr) return;
        
        for (int i = 0; i < nivel; ++i) cout << "  ";
        cout << nodo->nombre << " " << nodo->apellido << "\n";

        mostrar(nodo->hijo, nivel + 1);
        mostrar(nodo->hermano, nivel);
    }

    void mostrarArbol() {
        mostrar(raiz);
    }
};

int main() {
    string nombreUsuario, apellidoUsuario;
    cout << "Ingrese su nombre para iniciar el arbol genealogico: ";
    getline(cin, nombreUsuario);
    cout << "Ingrese su apellido: ";
    getline(cin, apellidoUsuario);

    ArbolGenealogico arbol(nombreUsuario, apellidoUsuario);

    int opcion;
    do {
        cout << "\nMenu:\n";
        cout << "1. Agregar hijo\n";
        cout << "2. Agregar hermano\n";
        cout << "3. Mostrar arbol\n";
        cout << "4. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            string nombrePadre, apellidoPadre, nombreHijo, apellidoHijo;
            cout << "Ingrese el nombre del padre: ";
            getline(cin, nombrePadre);
            cout << "Ingrese el apellido del padre: ";
            getline(cin, apellidoPadre);
            cout << "Ingrese el nombre del hijo: ";
            getline(cin, nombreHijo);
            cout << "Ingrese el apellido del hijo: ";
            getline(cin, apellidoHijo);
            arbol.agregarHijo(nombrePadre, apellidoPadre, nombreHijo, apellidoHijo);
        } else if (opcion == 2) {
            string nombreHermanoMayor, apellidoHermanoMayor, nombreHermano, apellidoHermano;
            cout << "Ingrese el nombre del hermano mayor: ";
            getline(cin, nombreHermanoMayor);
            cout << "Ingrese el apellido del hermano mayor: ";
            getline(cin, apellidoHermanoMayor);
            cout << "Ingrese el nombre del nuevo hermano: ";
            getline(cin, nombreHermano);
            cout << "Ingrese el apellido del nuevo hermano: ";
            getline(cin, apellidoHermano);
            arbol.agregarHermano(nombreHermanoMayor, apellidoHermanoMayor, nombreHermano, apellidoHermano);
        } else if (opcion == 3) {
            arbol.mostrarArbol();
        }
    } while (opcion != 4);

    cout << "Saliendo del programa.\n";
    return 0;
}