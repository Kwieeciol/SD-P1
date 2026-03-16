#include "DynamicArray.h"

class DynamicArray {
private:
    int* data;
    int size;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        int* newData = new int[newCapacity];
        for (int i = 0; i < size; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    };

public:
    DynamicArray(int initialCapacity = 4) {
        capacity = initialCapacity;
        size = 0;
        data = new int[capacity];
    };

    ~DynamicArray() {
        delete[] data;
    };

    int get(int index) const {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    };

    void insert(int index, int value) {
        if (index < 0 || index > size)
            throw std::out_of_range("Index out of range");
        if (size == capacity)
            resize();
        for (int i = size; i > index; i--)
            data[i] = data[i - 1];
        data[index] = value;
        size++;
    };

    void pushFront(int value) {
        insert(0, value);
    };

    void pushBack(int value) {
        insert(size, value);
    };

    void removeAt(int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        for (int i = index; i < size - 1; i++)
            data[i] = data[i + 1];
        size--;
    };

    void popFront() {
        removeAt(0);
    };

    void popBack() {
        removeAt(size - 1);
    };

    int search(int value) const {
        for (int i = 0; i < size; i++)
            if (data[i] == value)
                return i;
        return -1;
    };
};