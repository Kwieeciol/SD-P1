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
std::string operacja(Operation op) {
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
    // Inicjalizacja generatora liczb losowych z danym seedem
    srand(seed);
    for (int i = 0; i < size; i++) {
        int val = rand() % 100000;
        file << val << (i == size - 1 ? "" : " ");
    }

    file.close();
    std::cout << "Wygenerowano plik '" << fileName << "' (" << size << " liczb)." << std::endl;
}

// Funkcja mierząca czas wykonania konkretnej operacji
template <typename T>
int measure(int size, Operation op, unsigned int seed, int repetitions) {
    int totalDuration = 0;
    for (int r = 0; r < repetitions; r++) {
        T ds;
        srand(seed);
        // Wstepne wypelnianie struktury
        for (int j = 0; j < size; j++) ds.addBack(rand() % 100000);

        int randomIndex = (size > 0) ? rand() % size : 0;
        int val = rand() % 100000;
        // Start stopera 
        auto start = std::chrono::high_resolution_clock::now();
        // Wykonanie danej operacji
        if (op == ADD_FRONT) ds.addFront(val);
        else if (op == ADD_BACK) ds.addBack(val);
        else if (op == ADD_AT) ds.addAt(randomIndex, val);
        else if (op == REMOVE_FRONT) ds.removeFront();
        else if (op == REMOVE_BACK) ds.removeBack();
        else if (op == REMOVE_AT) ds.removeAt(randomIndex);
        else if (op == FIND) ds.find(val);
        // Stop stopera 
        auto end = std::chrono::high_resolution_clock::now();
        // Obliczanie czasu w nanosekundach i dodawanie do sumy
        totalDuration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }
    // Zwracanie średniego czasu z wykonanych powtórzeń
    return totalDuration / repetitions;
}

// Funkcja przeprowadzająca całą serię testów porównawczych i zapisująca je do plików CSV
void wszystkieBadania(const unsigned int seeds[10]) {
    std::vector<Operation> ops = { ADD_FRONT, ADD_BACK, ADD_AT, REMOVE_FRONT, REMOVE_BACK, REMOVE_AT, FIND };

    std::cout << "\nRozpoczecie badan" << std::endl;

    for (auto op : ops) {
        std::string fileName = "porownanie_" + operacja(op) + ".csv";
        std::ofstream file(fileName);
        file << "Rozmiar;DynamicArray [ns];SingleList [ns];DoubleList [ns]\n";

        std::cout << "Badanie: " << operacja(op) << " -> " << fileName << std::endl;
        // Wykonywanie badań dla wszystkich rozmiarów
        for (int i = 1; i <= 8; i++) {
            int N = i * 5000;
            int avgArr = 0, avgSing = 0, avgDoub = 0;
            // Pętla przechodząca przez wszystkie seedy
            for (int s = 0; s < 10; s++) {
                avgArr += measure<DynamicArray>(N, op, seeds[s], 10);
                avgSing += measure<SingleLinkedList>(N, op, seeds[s], 10);
                avgDoub += measure<DoubleLinkedList>(N, op, seeds[s], 10);
            }
            // Zapis wyników do pliku
            file << N << ";" << avgArr / 10 << ";" << avgSing / 10 << ";" << avgDoub / 10 << "\n";
        }
        file.close();
    }
    std::cout << "Wszystkie raporty zostaly wygenerowane pomyslnie.\n";
}

// Menu dla danej struktury
template <typename T>
void structureMenu(std::string name) {
    T* ds = new T();
    int choice = -1;
    std::cout << std::endl;
    while (choice != 0) {
        std::cout << "\n--- MENU: " << name << " ---" << std::endl; 
        std::cout << "1. Zbuduj z pliku\n2. Wypelnij strukture\n3. Dodaj\n4. Usun\n5. Znajdz\n6. Wyswietl rozmiar\n" 
                  << "7. Wyswietl zawartosc\n8. Wyczysc strukture\n0. Powrot\nWybor: ";
        // Zabezpieczenie przed wpisaniem znaku zamiast liczby
        if (!(std::cin >> choice)) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            choice = -1;
            continue;
        }
        std::system("cls");
        switch (choice) {
        case 1: { // Wczytywanie danych z pliku tekstowego
            std::string path;
            std::cout << "Podaj nazwe pliku: "; std::cin >> path;
            std::ifstream file(path);
            if (file.is_open()) {
                delete ds; ds = new T(); // Reset struktury przed wczytaniem
                int val;
                while (file >> val) ds->addBack(val);
                file.close();
                std::system("cls");
                std::cout << "Wczytano dane z pliku: " << path << std::endl;
            }
            else {
                std::system("cls");
                std::cout << "Blad pliku!" << std::endl; 
            }
            break;
        }
        case 2: { // Wypełnianie struktury określoną liczbą losowych elementów
            int n; std::cout << "Podaj ilosc elementow: "; std::cin >> n;
            delete ds; ds = new T();
            for (int i = 0; i < n; i++) ds->addBack(rand() % 100000);
            std::system("cls");
            std::cout << "Wypelnianie zakonczone. Ilosc dodanych elementow: " << n << std::endl;
            break;
        }
        case 3: { // Dodawanie elementu w wybrane miejsce
            int val, pos, sub;
            std::cout << "\n\n--- Dodawanie: " << name << " ---" << std::endl;
            std::cout << "1. Front\n2. Back\n3. Wybrana pozycja\nWybor: "; std::cin >> sub;
            std::cout << "Wartosc: "; std::cin >> val;

            if (sub == 1) ds->addFront(val);
            else if (sub == 2) ds->addBack(val);
            else { std::cout << "Pozycja: "; std::cin >> pos; ds->addAt(pos - 1, val); }
            std::system("cls");

            if (sub == 1) std::cout << "Dodano '" << val << "' na poczatek struktury" << std::endl;
            else if (sub == 2) std::cout << "Dodano '" << val << "' na koniec struktury" << std::endl; 
            else { std::cout << "Dodano '" << val << "' na pozycje " << pos << std::endl; }
            break;
        }
        case 4: { // Usuwanie elementu z wybranego miejsca
            int pos, sub;
            std::cout << "\n--- Usuwanie: " << name << " ---" << std::endl;
            std::cout << "1. Front\n2. Back\n3. Wybrana pozycja\nWybor: "; std::cin >> sub;
            if (sub == 1) ds->removeFront();
            else if (sub == 2) ds->removeBack();
            else { std::cout << "Pozycja: "; std::cin >> pos; ds->removeAt(pos - 1); }
            std::system("cls");
            if (sub == 1) std::cout << "Usunieto pierwszy element struktury" << std::endl;
            else if (sub == 2) std::cout << "Usunieto ostatni element struktury" << std::endl;
            else { std::cout << "Usunieto element z pozycji: " << pos << std::endl; }
            break;
        }
        case 5: { // Wyszukiwanie wartości
            int val;
            std::cout << "Podaj wartosc do znalezienia: ";
            if (!(std::cin >> val)) {
                std::cin.clear();
                while (std::cin.get() != '\n');
                break;
            }

            int position = ds->find(val);
            std::system("cls");
            if (position != -1) {
                std::cout << "Znaleziono wartosc " << val << " na pozycji: " << position << std::endl;
            }
            else {
                std::cout << "Wartosc " << val << " nie znajduje sie w strukturze." << std::endl;
            }
            break;
        }
        case 6: // Wyświetlanie aktualnej liczby elementów
            if (ds->getSize() == 0) std::cout << "Struktura jest pusta" << std::endl;
            else {
                std::cout << "Aktualny rozmiar struktury: " << ds->getSize() << std::endl;
                break;
            }
        case 7: // Wyświetlanie zawartości struktury
            ds->display();
            break;
        case 8: // Wyczyszczenie całej struktury
            if (ds->getSize() == 0) std::cout << "Struktura jest pusta" << std::endl;
            else {
                delete ds; ds = new T();
                std::cout << "Struktura zostala wyczyszczona" << std::endl;
            }
        case 0:
            break;
        default:
            std::cout << "Niepoprawny wybor.";
            break;
        }
    }
    // Zwolnienie pamięci przed powrotem do menu głównego
    delete ds;
}

int main() {
    const unsigned int seeds[10] = { 12345, 67890, 11223, 44556, 77889, 99001, 13579, 24680, 98765, 54321 };
    int mainChoice = -1;
    std::cout << std::endl;
    while (mainChoice != 0) {
        std::cout << "\n========== MENU GLOWNE ==========" << std::endl;
        std::cout << "1. Tablica Dynamiczna" << std::endl;
        std::cout << "2. Lista Jednokierunkowa" << std::endl;
        std::cout << "3. Lista Dwukierunkowa" << std::endl;
        std::cout << "4. Generuj plik tekstowy (txt)" << std::endl;
        std::cout << "5. Pelne badania porownawcze (csv)" << std::endl;
        std::cout << "0. Wyjscie" << std::endl;
        std::cout << "Wybor: ";

        // Zabezpieczenie przed wpisaniem znaku zamiast liczby
        if (!(std::cin >> mainChoice)) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            mainChoice = -1;
            std::system("cls");
            continue;
        }
        std::system("cls");
        switch (mainChoice) {
        case 1: // Wybranie tablicy dynamicznej
            structureMenu<DynamicArray>("Tablica Dynamiczna");
            break;
        case 2: // Wybranie listy jednokierunkowej
            structureMenu<SingleLinkedList>("Lista Jednokierunkowa");
            break;
        case 3: // Wybranie listy dwukierunkowej
            structureMenu<DoubleLinkedList>("Lista Dwukierunkowa");
            break;
        case 4: { // Ręczne generowanie pliku z danymi
            int sIdx, n; std::string name;
            std::cout << "Indeks seeda (0-9): "; std::cin >> sIdx;
            std::cout << "Liczba elementow: "; std::cin >> n;
            std::cout << "Nazwa pliku: "; std::cin >> name;
            generateTextFile(seeds[sIdx % 10], n, name);
            break;
        }
        case 5: // Wykonywanie wszystkich badań
            wszystkieBadania(seeds);
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
