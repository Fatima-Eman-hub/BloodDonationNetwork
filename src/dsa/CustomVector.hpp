#ifndef CUSTOM_VECTOR_HPP
#define CUSTOM_VECTOR_HPP

#include <stdexcept>
#include <cstring>

// Dynamic array implementation with automatic capacity management
template<typename T>
class CustomVector {
private:
    T* data;
    size_t capacity;
    size_t size;
    
    // Reallocate memory when capacity is exceeded
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
    
public:
    CustomVector() : data(nullptr), capacity(0), size(0) {}
    
    CustomVector(size_t initial_capacity) : capacity(initial_capacity), size(0) {
        data = new T[capacity];
    }
    
    ~CustomVector() {
        delete[] data;
    }
    
    // Copy constructor
    CustomVector(const CustomVector& other) : capacity(other.capacity), size(other.size) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Assignment operator
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    void push_back(const T& value) {
        if (size >= capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
            resize(new_capacity);
        }
        data[size++] = value;
    }
    
    void pop_back() {
        if (size > 0) {
            --size;
        }
    }
    
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    bool empty() const { return size == 0; }
    void clear() { size = 0; }
    
    T* begin() { return data; }
    T* end() { return data + size; }
    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};

#endif // CUSTOM_VECTOR_HPP
