#include "DoubleLinkedList.h"
#include <stdexcept>
#include <iostream>

// Konstruktor domyślny
DoubleLinkedList::DoubleLinkedList() : header(nullptr), trailer(nullptr), size(0) {}
// Konstruktor kopiujący
DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other) : header(nullptr), trailer(nullptr), size(0) {
    Node* current = other.header;
    while (current != nullptr) {
        addBack(current->value);
        current = current->next;
    }
}  
// Destruktor
DoubleLinkedList::~DoubleLinkedList()  {
    while (header != nullptr) {
        removeFront();
    }
}

// Dodawanie elementu na początek listy
void DoubleLinkedList::addFront(int value) {
    Node* newNode = new Node(value);
    if (header == nullptr) {
        header = trailer = newNode;
    }
    else {
        // Nowy węzeł wskazuje na stary header, który teraz bedzie wskazywał wstecz na nowy węzeł
        // i węzeł zostaje headerem
        newNode->next = header;
        header->prev = newNode;
        header = newNode;
    }
    size++;
}
// Dodawanie elementu na koniec listy
void DoubleLinkedList::addBack(int value) {
    Node* newNode = new Node(value);
    if (trailer == nullptr) {
        header = trailer = newNode;
    }
    else {
        // Podobnie jak w dodawaniu na początek nowy węzeł zamienia się z obecnym trailerem 
        newNode->prev = trailer;
        trailer->next = newNode;
        trailer = newNode;
    }
    size++;
}
// Dodawanie elementu w danym miejscu
void DoubleLinkedList::addAt(int index, int value) {
    // Sprawdzenie czy podany index jest poprawny
    if (index < 0 || index > size) throw std::out_of_range("Index out of bounds");

    if (index == 0) {
        addFront(value);
    }
    else if (index == size) {
        addBack(value);
    }

    else {
        Node* current;

        // Wybór kierunku przeszukiwania
        if (index < size / 2) {
            // Idziemy od początku
            current = header;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        }
        else {
            // Idziemy od końca
            current = trailer;
            for (int i = size - 1; i > index; i--) {
                current = current->prev;
            }
        }
        // Wstawienie nowego węzła przed wyszukany elemenent 'current'
        Node* newNode = new Node(value);
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        size++;
    }
}

// Usunięcie pierwszego elementu
void DoubleLinkedList::removeFront() {
    if (header == nullptr) return;

    Node* temp = header;
    // Przesunięcie headera na następny element
    header = header->next;
    if (header != nullptr) {
        // Zerowanie wskaźnika wstecz dla nowego headera
        header->prev = nullptr;
    }
    else {
        // Wyzerowanie trailera przy pustej liście
        trailer = nullptr; 
    }
    // Zwolnienie pamięci usuniętego węzła
    delete temp;
    size--;
}
// Usunięcie ostatniego elementu
void DoubleLinkedList::removeBack() {
    if (trailer == nullptr) return;
    // Podobne działanie jak w removeFront ale od drugiej strony
    Node* temp = trailer;
    trailer = trailer->prev;
    if (trailer != nullptr) {
        trailer->next = nullptr;
    }
    else {
        header = nullptr; 
    }
    delete temp;
    size--;
}
// Usunięcie elementu w danym miejscu - podobne działanie jak w dodawaniu
void DoubleLinkedList::removeAt(int index) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of bounds");
    if (index == 0) {
        removeFront();
    }
    else if (index == size - 1) {
        removeBack();
    }
    else {
        Node* current;

        //  Wybór kierunku przeszukiwania
        if (index < size / 2) {
            current = header;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        }
        else {
            current = trailer;
            for (int i = size - 1; i > index; i--) {
                current = current->prev;
            }
        }
        // Usunięcie znalezionego węzła z listy
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        size--;
    }
}


// Wyszukiwanie danej wartości i zwrócenie jej indexu
int DoubleLinkedList::find(int value) const {
    Node* current = header;
    int index = 1;
    while (current != nullptr) {
        if (current->value == value) return index;
        current = current->next;
        index++;
    }
    return -1; 
}
// Zwracanie wielkości listy
int DoubleLinkedList::getSize() const {
    return size;
}
// Wyświetlenie zawartości listy
void DoubleLinkedList::display() const {
    if (header == nullptr) {
        std::cout << "Lista jest pusta." << std::endl;
        return;
    }
    Node* curr = header;
    std::cout << "Zawartosc listy: ";
    // Przechodzenie przez liste od headera i wyświetlanie po kolei każdej wartości
    while (curr != nullptr) {
        std::cout << curr->value;
        if (curr->next != nullptr) {
            std::cout << ", ";
        }
        curr = curr->next;
    }
    std::cout << std::endl;
}
