#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

const int META = 50;  // Distància a la meta
bool cursa_acabada = false;
mutex mtx;  // Mutex per controlar l'accés compartit

// Funció per moure el cursor a una posició específica a la consola
void mouCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Classe per representar un Animal
class Animal {
public:
    string nom;
    int velocitat;
    int posicio;
    int fila; // Posició en la consola
    Animal(string n, int v, int f) : nom(n), velocitat(v), posicio(0), fila(f) {}

    // Funció que simula el moviment de l'animal
    void correr() {
        while (!cursa_acabada) {
            // Avançament aleatori basat en la velocitat base
            int avancament = velocitat + (rand() % 3 - 1);  // variació aleatòria: velocitat ± 1
            posicio += max(avancament, 0);  // Evitar avançament negatiu

            // Mostra el progrés en forma de punts
            mtx.lock();
            mouCursor(0, fila);
            cout << nom << ": ";
            for (int i = 0; i < posicio && i < META; i++) cout << ".";
            cout << " " << posicio;
            mtx.unlock();

            // Verificar si l'animal ha arribat a la meta
            if (posicio >= META) {
                cursa_acabada = true;
                mtx.lock();
                mouCursor(0, fila + 1);
                cout << nom << " ha arribat a la meta i ha guanyat!" << endl;
                mtx.unlock();
                break;
            }

            // Pausa aleatòria per simular pauses inesperades
            this_thread::sleep_for(chrono::milliseconds(100 + rand() % 100));
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Inicialitzar la llavor de nombres aleatoris

    // Definició dels animals (nom, velocitat base, fila a la consola)
    Animal llebre("Llebre", 3, 2);
    Animal tortuga("Tortuga", 1, 3);
    Animal gos("Gos", 2, 4);

    // Crear els fils per a cada animal
    thread llebre_thread(&Animal::correr, &llebre);
    thread tortuga_thread(&Animal::correr, &tortuga);
    thread gos_thread(&Animal::correr, &gos);

    // Esperar que els fils acabin
    llebre_thread.join();
    tortuga_thread.join();
    gos_thread.join();

    return 0;
}
