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

    DynamicArray(const DynamicArray& other) {
        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; i++)
            data[i] = other.data[i];
    }

    ~DynamicArray() {
        delete[] data;
    };

    int get(int index) const {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    };

    void addAt(int index, int value) {
        if (index < 0 || index > size)
            throw std::out_of_range("Index out of range");
        if (size == capacity)
            resize();
        for (int i = size; i > index; i--)
            data[i] = data[i - 1];
        data[index] = value;
        size++;
    };

    void addFront(int value) {
        addAt(0, value);
    };

    void addBack(int value) {
        addAt(size, value);
    };

    void removeAt(int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        for (int i = index; i < size - 1; i++)
            data[i] = data[i + 1];
        size--;
    };

    void removeFront() {
        removeAt(0);
    };

    void removeBack() {
        removeAt(size - 1);
    };

    int find(int value) const {
        for (int i = 0; i < size; i++)
            if (data[i] == value)
                return i;
        return -1;
    };
};