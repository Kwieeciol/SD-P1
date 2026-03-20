#pragma once

struct Node {
    int value;
    Node* next;

    Node(int, val) : value(val), next(nullptr) {}
};

class SingleLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    SingleLinkedList();
    SingleLinkedList(const SingleLinkedList& other);
    ~SingleLinkedList();

    int get(int index) const;

    void addFront(int value);
    void addBack(int value);
    void addAt(int index, int value);

    void removeFront();
    void removeBack();
    void removeAt(int index);

    int find(int value) const;

    int getSize() const;
}