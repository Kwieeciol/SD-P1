#include "SingleLinkedList.h"
#include <stdexcept>
#include <iostream>

SingleLinkedList::SingleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

SingleLinkedList::SingleLinkedList(const SingleLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* curr = other.head;
    while (curr != nullptr) {
        addBack(curr->value);
        curr = curr->next;
    }
}

SingleLinkedList::~SingleLinkedList() {
    Node* curr = head;
    while (curr != nullptr) {
        Node* next = curr->next; // Zapamiętaj następny przed usunięciem
        delete curr;
        curr = next;
    }
}

void SingleLinkedList::addFront(int value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

void SingleLinkedList::addBack(int value) {
    Node* newNode = new Node(value);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

void SingleLinkedList::addAt(int index, int value) {
    if (index < 0 || index > size)
        throw std::out_of_range("Index out of range");
    if (index == 0) { addFront(value); return; }
    if (index == size) { addBack(value); return; }

    // Przechodzenie do węzła poprzedzającego docelową pozycję
    Node* curr = head;
    for (int i = 0; i < index - 1; i++)
        curr = curr->next;

    Node* newNode = new Node(value);
    newNode->next = curr->next;
    curr->next = newNode;
    size++;
}

void SingleLinkedList::removeFront() {
    if (head == nullptr) return;
    Node* temp = head;
    head = head->next;
    if (head == nullptr) tail = nullptr; // Lista jest pusta
    delete temp;
    size--;
}

void SingleLinkedList::removeBack() {
    if (head == nullptr) return;
    if (head == tail) { // Jeden element
        delete head;
        head = tail = nullptr;
        size--;
        return;
    }

    // Przechodzenie do węzła przed końcem
    Node* curr = head;
    while (curr->next != tail)
        curr = curr->next;
    delete tail;
    tail = curr;
    tail->next = nullptr;
    size--;
}

void SingleLinkedList::removeAt(int index) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of range");
    if (index == 0) { removeFront(); return; }
    if (index == size - 1) { removeBack(); return; }

    // Przechodzenie do węzła poprzedzającego i usuwanie go
    Node* curr = head;
    for (int i = 0; i < index - 1; i++)
        curr = curr->next;

    Node* temp = curr->next;
    curr->next = temp->next;
    delete temp;
    size--;
}

int SingleLinkedList::find(int value) const {
    Node* curr = head;
    int index = 0;
    while (curr != nullptr) {
        if (curr->value == value) return index;
        curr = curr->next;
        index++;
    }
    return -1; // Nie znaleziono
}

int SingleLinkedList::getSize() const {
    return size;
}

void SingleLinkedList::display() const {
    std::cout << "{";
    Node* curr = head;
    while (curr != nullptr) {
        std::cout << curr->value;
        if (curr->next != nullptr) std::cout << ", ";
        curr = curr->next;
    }
    std::cout << "}" << std::endl;
}
