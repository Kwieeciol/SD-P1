#pragma once

struct Node {
    int value;
    Node* prev;
    Node* next;

    Node(int val) : value(val), prev(nullptr), next(nullptr) {}
};

class DoubleLinkedList {
private:
    Node* header;
    Node* trailer;
    int size;

public:

    DoubleLinkedList();
    DoubleLinkedList(const DoubleLinkedList& other);
    ~DoubleLinkedList();

    void addFront(int value);
    void addBack(int value);
    void addAt(int index, int value);

    void removeFront();
    void removeBack();
    void removeAt(int index);

    int find(int value) const;

    int getSize() const;
};