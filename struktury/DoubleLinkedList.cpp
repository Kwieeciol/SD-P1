#include "DoubleLinkedList.h"
#include <stdexcept>
#include <iostream>

DoubleLinkedList::DoubleLinkedList() : header(nullptr), trailer(nullptr), size(0) {}

// Konstruktor kopiujący
DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other) : header(nullptr), trailer(nullptr), size(0) {
    Node* current = other.header;
    while (current != nullptr) {
        addBack(current->value);
        current = current->next;
    }
}

DoubleLinkedList::~DoubleLinkedList()  {
    while (header != nullptr) {
        removeFront();
    }
}



void DoubleLinkedList::addFront(int value) {
    Node* newNode = new Node(value);
    if (header == nullptr) {
        header = trailer = newNode;
    }
    else {
        newNode->next = header;
        header->prev = newNode;
        header = newNode;
    }
    size++;
}
void DoubleLinkedList::addBack(int value) {
    Node* newNode = new Node(value);
    if (trailer == nullptr) {
        header = trailer = newNode;
    }
    else {
        newNode->prev = trailer;
        trailer->next = newNode;
        trailer = newNode;
    }
    size++;
}
void DoubleLinkedList::addAt(int index, int value) {
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
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
        }
        else {
            // Idziemy od końca
            current = trailer;
            for (int i = size - 1; i > index - 1; i--) {
                current = current->prev;
            }
        }

        Node* newNode = new Node(value);
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        size++;
    }
}



void DoubleLinkedList::removeFront() {
    if (header == nullptr) return;

    Node* temp = header;
    header = header->next;
    if (header != nullptr) {
        header->prev = nullptr;
    }
    else {
        trailer = nullptr; // Lista stała się pusta
    }
    delete temp;
    size--;
}
void DoubleLinkedList::removeBack() {
    if (trailer == nullptr) return;

    Node* temp = trailer;
    trailer = trailer->prev;
    if (trailer != nullptr) {
        trailer->next = nullptr;
    }
    else {
        header = nullptr; // Lista stała się pusta
    }
    delete temp;
    size--;
}
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
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
        }
        else {
            current = trailer;
            for (int i = size - 1; i > index - 1; i--) {
                current = current->prev;
            }
        }

        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        size--;
    }
}



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

int DoubleLinkedList::getSize() const {
    return size;
}

void DoubleLinkedList::display() const {
    if (size == 0 || header == nullptr) {
        std::cout << "Lista jest pusta." << std::endl;
        return;
    }
    Node* curr = header;

    std::cout << "Zawartosc listy (rozmiar: " << size << "):" << std::endl;

    while (curr != nullptr) {
        std::cout << curr->value;

        if (curr->next != nullptr) {
            std::cout << ", ";
        }

        curr = curr->next;
    }
    std::cout << std::endl;
}
