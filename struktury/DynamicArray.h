#pragma once

class DynamicArray {
private:
    int* data;
    int size;
    int capacity;

    void resize();

public:
    DynamicArray(int initialCapacity = 4);
    ~DynamicArray();

    int get(int index) const;

    void insert(int index, int value);
    void pushFront(int value);
    void pushBack(int value);

    void removeAt(int index);
    void popFront();
    void popBack();

    int search(int value) const;
};
