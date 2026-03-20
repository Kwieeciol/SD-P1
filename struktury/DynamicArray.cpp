#include "DynamicArray.h"
#include <stdexcept>

void DynamicArray::resize() {
    int newCapacity = capacity * 2;
    int* newData = new int[newCapacity];
    for (int i = 0; i < size; i++)
        newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

DynamicArray::DynamicArray(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    data = new int[capacity];
}

DynamicArray::DynamicArray(const DynamicArray& other) {
    size = other.size;
    capacity = other.capacity;
    data = new int[capacity];
    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

int DynamicArray::get(int index) const {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of range");
    return data[index];
}

void DynamicArray::addAt(int index, int value) {
    if (index < 0 || index > size)
        throw std::out_of_range("Index out of range");
    if (size == capacity)
        resize();
    for (int i = size; i > index; i--)
        data[i] = data[i - 1];
    data[index] = value;
    size++;
}

void DynamicArray::addFront(int value) {
    addAt(0, value);
}

void DynamicArray::addBack(int value) {
    addAt(size, value);
}

void DynamicArray::removeAt(int index) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of range");
    for (int i = index; i < size - 1; i++)
        data[i] = data[i + 1];
    size--;
}

void DynamicArray::removeFront() {
    removeAt(0);
}

void DynamicArray::removeBack() {
    removeAt(size - 1);
}

int DynamicArray::find(int value) const {
    for (int i = 0; i < size; i++)
        if (data[i] == value)
            return i;
    return -1;
}
