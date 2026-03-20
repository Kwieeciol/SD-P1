#pragma once

class DynamicArray {
private:
    int* data;
    int size;
    int capacity;

    void resize();

public:
    DynamicArray(int initialCapacity = 4);
    DynamicArray(const DynamicArray& other);
    ~DynamicArray();
 
    int get(int index) const;

    void addAt(int index, int value);
    void addFront(int value);
    void addBack(int value);

    void removeAt(int index);
    void removeFront();
    void removeBack();

    int find(int value) const;
};
