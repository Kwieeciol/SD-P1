#include "SingleLinkedList.h"

struct Node {
    int value;
    Node* next;

    Node(int val) : value(val), next(nullptr) {}
};


class SingleLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    SingleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    SingleLinkedList(const SingleLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        Node* curr = other.head;
        while (curr != nullptr) {
            addBack(curr->value);
            curr = curr->next;
        }
    }

    ~SingleLinkedList() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void addFront(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    void addBack(int value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void addAt(int index, int value) {
        if (index < 0 || index > size) 
            throw std::out_of_range("Index out of range");
        if (index == 0) { addFront(value); return; }
        if (index == size) { addBack(value); return; }

        Node* curr = head;
        for (int i = 0; i < index - 1; i++)
            curr = curr->next;

        Node* newNode = new Node(value);
        newNode->next = curr->next;
        curr->next = newNode;
        size++;
    }

    void removeFront() {
        if (head == nullptr) return;
        Node* temp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete temp;
        size--;
    }

    void removeBack() {
        if (head == nullptr) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            size--;
            return;
        }
        Node* curr = head;
        while (curr->next != tail)
            curr = curr->next;
        delete tail;
        tail = curr;
        tail->next = nullptr;
        size--;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        if (index == 0) { removeFront(); return; }
        if (index == size - 1) { removeBack(); return; }

        Node* curr = head;
        for (int i = 0; i < index - 1; i++)
            curr = curr->next;
    
        Node* temp = curr->next;
        curr->next = temp->next;
        delete temp;
        size--;
    }

    int find(int value) const {
        Node* curr = head;
        int index = 0;
        while (curr != nullptr) {
            if (curr->value == value) return index;
            curr = curr->next;
            index++;
        }
        return -1;
    }

    int getSize() const {
        return size;
    }
};