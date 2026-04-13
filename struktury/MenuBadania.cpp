#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

#include "DynamicArray.h"
#include "SingleLinkedList.h"
#include "DoubleLinkedList.h"

// Typy operacji
enum Operation {
    ADD_FRONT, ADD_BACK, ADD_AT,
    REMOVE_FRONT, REMOVE_BACK, REMOVE_AT,
    FIND
};

// Pomocnicza funkcja do nazewnictwa plików i komunikatów
std::string getOpName(Operation op) {
    switch (op) {
    case ADD_FRONT: return "Add_Front";
    case ADD_BACK: return "Add_Back";
    case ADD_AT: return "Add_At";
    case REMOVE_FRONT: return "Remove_Front";
    case REMOVE_BACK: return "Remove_Back";
    case REMOVE_AT: return "Remove_At";
    case FIND: return "Find";
    default: return "Unknown";
    }
}

// Generowanie pliku tekstowego z danymi na podstawie seeda
void generateTextFile(unsigned int seed, int size, std::string fileName) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cout << "Blad: Nie mozna utworzyc pliku!" << std::endl;
        return;
    }

    srand(seed);
    for (int i = 0; i < size; i++) {
        // Generujemy liczbe calkowita (int - 4 bajty)
        int val = rand() % 100000;
        file << val << (i == size - 1 ? "" : " ");
    }

    file.close();
    std::cout << "Wygenerowano plik '" << fileName << "' (" << size << " liczb)." << std::endl;
}

// Funkcja pomiarowa dla automatycznych testów
template <typename T>
long long measure(int size, Operation op, unsigned int seed, int repetitions) {
    long long totalDuration = 0;
    for (int r = 0; r < repetitions; r++) {
        T ds;
        srand(seed);
        // Wstepne wypelnianie struktury
        for (int j = 0; j < size; j++) ds.addBack(rand() % 100000);

        int randomIndex = (size > 0) ? rand() % size : 0;
        int val = rand() % 100000;

        auto start = std::chrono::high_resolution_clock::now();

        if (op == ADD_FRONT) ds.addFront(val);
        else if (op == ADD_BACK) ds.addBack(val);
        else if (op == ADD_AT) ds.addAt(randomIndex, val);
        else if (op == REMOVE_FRONT) ds.removeFront();
        else if (op == REMOVE_BACK) ds.removeBack();
        else if (op == REMOVE_AT) ds.removeAt(randomIndex);
        else if (op == FIND) ds.find(val);

        auto end = std::chrono::high_resolution_clock::now();
        totalDuration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }
    return totalDuration / repetitions;
}

// Automatyczne generowanie wszystkich raportów CSV
void runFullReport(const unsigned int seeds[10]) {
    std::vector<Operation> ops = { ADD_FRONT, ADD_BACK, ADD_AT, REMOVE_FRONT, REMOVE_BACK, REMOVE_AT, FIND };

    std::cout << "\nRozpoczynam pelne badania (8 krokow x 10 seedow x 10 powtorzen)..." << std::endl;

    for (auto op : ops) {
        std::string fileName = "porownanie_" + getOpName(op) + ".csv";
        std::ofstream file(fileName);
        file << "Rozmiar;DynamicArray [ns];SingleList [ns];DoubleList [ns]\n";

        std::cout << "Badanie: " << getOpName(op) << " -> " << fileName << std::endl;

        for (int i = 1; i <= 8; i++) {
            int N = i * 5000;
            long long avgArr = 0, avgSing = 0, avgDoub = 0;

            for (int s = 0; s < 10; s++) {
                avgArr += measure<DynamicArray>(N, op, seeds[s], 10);
                avgSing += measure<SingleLinkedList>(N, op, seeds[s], 10);
                avgDoub += measure<DoubleLinkedList>(N, op, seeds[s], 10);
            }
            file << N << ";" << avgArr / 10 << ";" << avgSing / 10 << ";" << avgDoub / 10 << "\n";
        }
        file.close();
    }
    std::cout << "Wszystkie raporty zostaly wygenerowane pomyslnie.\n";
}

// Menu dla pojedynczej struktury
template <typename T>
void structureMenu(std::string name) {
    T* ds = new T();
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n--- MENU: " << name << " ---" << std::endl;
        std::cout << "1. Zbuduj z pliku\n2. Utworz losowo\n3. Dodaj\n4. Usun\n5. Znajdz\n6. Wyswietl rozmiar\n7. Wyswietl zawartosc\n0. Powrot\nWybor: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            choice = -1;
            continue;
        }
        std::system("cls");
        switch (choice) {
        case 1: {
            std::string path;
            std::cout << "Podaj nazwe pliku: "; std::cin >> path;
            std::ifstream file(path);
            if (file.is_open()) {
                delete ds; ds = new T();
                int val;
                while (file >> val) ds->addBack(val);
                file.close();
                std::cout << "Wczytano." << std::endl;
            }
            else std::cout << "Blad pliku!" << std::endl;
            break;
        }
        case 2: {
            int n; std::cout << "Ile elementow? "; std::cin >> n;
            delete ds; ds = new T();
            for (int i = 0; i < n; i++) ds->addBack(rand() % 100000);
            std::cout << "Utworzono losowo." << std::endl;
            break;
        }
        case 3: {
            int val, pos, sub;
            std::cout << "1.Front 2.Back 3.Pozycja: "; std::cin >> sub;
            std::cout << "Wartosc: "; std::cin >> val;
            if (sub == 1) ds->addFront(val);
            else if (sub == 2) ds->addBack(val);
            else { std::cout << "Pozycja: "; std::cin >> pos; ds->addAt(pos, val); }
            break;
        }
        case 4: {
            int pos, sub;
            std::cout << "1.Front 2.Back 3.Pozycja: "; std::cin >> sub;
            if (sub == 1) ds->removeFront();
            else if (sub == 2) ds->removeBack();
            else { std::cout << "Pozycja: "; std::cin >> pos; ds->removeAt(pos); }
            break;
        }
        case 5: {
            int val;
            std::cout << "Podaj wartosc do znalezienia: ";
            if (!(std::cin >> val)) {
                std::cin.clear();
                while (std::cin.get() != '\n');
                break;
            }

            int position = ds->find(val); 

            if (position != -1) {
                std::cout << "Znaleziono wartosc " << val << " na pozycji: " << position << std::endl;
            }
            else {
                std::cout << "Wartosc " << val << " nie znajduje sie w strukturze." << std::endl;
            }
            break;
        }
        case 6:
            std::cout << "Aktualny rozmiar struktury: " << ds->getSize() << std::endl;
            break;
        case 7:
            ds->display();
            break;
        case 0:
            break;
        default:
            std::cout << "Niepoprawny wybor." << std::endl;
            break;
        }
    }
    delete ds;
}

int main() {
    const unsigned int seeds[10] = { 12345, 67890, 11223, 44556, 77889, 99001, 13579, 24680, 98765, 54321 };
    int mainChoice = -1;

    while (mainChoice != 0) {
        std::cout << "\n========== MENU GLOWNE ==========" << std::endl;
        std::cout << "1. Tablica Dynamiczna" << std::endl;
        std::cout << "2. Lista Jednokierunkowa" << std::endl;
        std::cout << "3. Lista Dwukierunkowa" << std::endl;
        std::cout << "4. Generuj plik tekstowy (txt)" << std::endl;
        std::cout << "5. Pelne badania porownawcze (csv)" << std::endl;
        std::cout << "0. Wyjscie" << std::endl;
        std::cout << "Wybor: ";

        // Zabezpieczenie przed blednymi danymi (np. znakami zamiast liczb)
        if (!(std::cin >> mainChoice)) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            mainChoice = -1; // Wartosc neutralna, aby menu wyswietlilo sie ponownie
            continue;
        }
        std::system("cls");
        switch (mainChoice) {
        case 1:
            structureMenu<DynamicArray>("Tablica Dynamiczna");
            break;
        case 2:
            structureMenu<SingleLinkedList>("Lista Jednokierunkowa");
            break;
        case 3:
            structureMenu<DoubleLinkedList>("Lista Dwukierunkowa");
            break;
        case 4: {
            int sIdx, n; std::string name;
            std::cout << "Indeks seeda (0-9): "; std::cin >> sIdx;
            std::cout << "Liczba elementow: "; std::cin >> n;
            std::cout << "Nazwa pliku: "; std::cin >> name;
            generateTextFile(seeds[sIdx % 10], n, name);
            break;
        }
        case 5:
            runFullReport(seeds);
            break;
        case 0:
            std::cout << "Koniec programu." << std::endl;
            break;
        default:
            std::cout << "Bledny wybor!" << std::endl;
            break;
        }
    }
    return 0;
}