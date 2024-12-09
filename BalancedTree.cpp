#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class Nodo {
public:
    T valor;
    Nodo* izquierdo;
    Nodo* derecho;
    int altura;

    Nodo(const T& val) : valor(val), izquierdo(nullptr), derecho(nullptr), altura(1) {}
};

template <typename T>
class AVL {
private:
    Nodo<T>* raiz;

    int obtenerAltura(Nodo<T>* nodo) {
        return nodo ? nodo->altura : 0;
    }

    int obtenerBalance(Nodo<T>* nodo) {
        return nodo ? obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho) : 0;
    }

    Nodo<T>* rotarDerecha(Nodo<T>* y) {
        Nodo<T>* x = y->izquierdo;
        Nodo<T>* T2 = x->derecho;
        x->derecho = y;
        y->izquierdo = T2;
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        return x;
    }

    Nodo<T>* rotarIzquierda(Nodo<T>* x) {
        Nodo<T>* y = x->derecho;
        Nodo<T>* T2 = y->izquierdo;
        y->izquierdo = x;
        x->derecho = T2;
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        return y;
    }

    Nodo<T>* balancear(Nodo<T>* nodo) {
        int balance = obtenerBalance(nodo);

        if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0)
            return rotarDerecha(nodo);

        if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }

        if (balance < -1 && obtenerBalance(nodo->derecho) <= 0)
            return rotarIzquierda(nodo);

        if (balance < -1 && obtenerBalance(nodo->derecho) > 0) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }

        return nodo; 
    }

    Nodo<T>* eliminarNodo(Nodo<T>* raiz, const T& valor) {
        if (raiz == nullptr)
            return raiz;

        if (valor < raiz->valor)
            raiz->izquierdo = eliminarNodo(raiz->izquierdo, valor);
        else if (valor > raiz->valor)
            raiz->derecho = eliminarNodo(raiz->derecho, valor);
        else {
            if ((raiz->izquierdo == nullptr) || (raiz->derecho == nullptr)) {
                Nodo<T>* temp = raiz->izquierdo ? raiz->izquierdo : raiz->derecho;
                if (temp == nullptr) {
                    temp = raiz;
                    raiz = nullptr;
                } else {
                    *raiz = *temp;
                }
                delete temp;
            } else {
                Nodo<T>* temp = raiz->derecho;
                while (temp->izquierdo != nullptr)
                    temp = temp->izquierdo;

                raiz->valor = temp->valor;
                raiz->derecho = eliminarNodo(raiz->derecho, temp->valor);
            }
        }

        if (raiz == nullptr)
            return raiz;

        raiz->altura = 1 + max(obtenerAltura(raiz->izquierdo), obtenerAltura(raiz->derecho));
        return balancear(raiz);
    }

    void insertarNoRecursivo(const T& valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);

        if (!raiz) {
            raiz = nuevoNodo;
            return;
        }

        Nodo<T>* actual = raiz;
        Nodo<T>* padre = nullptr;

        while (actual) {
            padre = actual;
            if (valor < actual->valor) {
                actual = actual->izquierdo;
            } else if (valor > actual->valor) {
                actual = actual->derecho;
            } else {
                return;
            }
        }

        if (valor < padre->valor)
            padre->izquierdo = nuevoNodo;
        else
            padre->derecho = nuevoNodo;

        Nodo<T>* temp = padre;
        while (temp != nullptr) {
            temp->altura = max(obtenerAltura(temp->izquierdo), obtenerAltura(temp->derecho)) + 1;
            temp = balancear(temp); 
            if (temp == raiz) break;
            temp = temp == padre ? nullptr : padre;
        }
    }

    void imprimirEnOrden(Nodo<T>* nodo) {
        if (nodo) {
            imprimirEnOrden(nodo->izquierdo);
            cout << nodo->valor << " ";
            imprimirEnOrden(nodo->derecho);
        }
    }

public:
    AVL() : raiz(nullptr) {}

    void insertar(const T& valor) {
        insertarNoRecursivo(valor);
    }

    void eliminar(const T& valor) {
        raiz = eliminarNodo(raiz, valor);
    }

    void imprimir() {
        imprimirEnOrden(raiz);
        cout << endl;
    }
};

int main() {
    AVL<int> arbol;

    int opcion, valor;
    do {
        cout << "\nMenu:\n";
        cout << "1. Insertar valor\n";
        cout << "2. Eliminar valor\n";
        cout << "3. Mostrar arbol en orden\n";
        cout << "Ingresa tu opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese el valor a insertar: ";
                cin >> valor;
                arbol.insertar(valor);
                break;

            case 2:
                cout << "Ingrese el valor a eliminar: ";
                cin >> valor;
                arbol.eliminar(valor);
                break;

            case 3:
                cout << "Arbol en orden: ";
                arbol.imprimir();
                break;

            default:
                cout << "Opcion no valida.\n";
        }
    } while (opcion != 4);

    return 0;
}
