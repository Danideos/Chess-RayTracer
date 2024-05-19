#ifndef MAIN_CPP_VECTOR_H
#define MAIN_CPP_VECTOR_H

#include <iostream>
#include <array>
#include <initializer_list>
#include <stdexcept>
#include <math.h>
#include <random>

constexpr const int DIMS_3D = 3;
#define Vec3D Vector<double, DIMS_3D>

template<typename element, std::size_t n>
class Vector{
private:
    std::array<element, n> entries;
public:
    Vector();
    explicit Vector(std::initializer_list<element> list);
    Vector(const Vector& other);
    Vector(Vector&& other);

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    bool operator==(const Vector& other);
    const bool operator==(const Vector& other) const;

    element& operator[](size_t index);
    const element& operator[](size_t index) const;

    Vector<element, n> operator+(const Vector<element, n>& vector) const;
    Vector<element, n> operator-(const Vector<element, n>& vector) const;
    Vector<element, n> operator*(const element item) const;
    Vector<element, n> operator/(const element item) const;
    template<typename element2, std::size_t n2>
    friend Vector<element2, n2> operator*(const element2 item, const Vector<element2, n2>& vector);

    Vector<element, n> operator-() const;

    friend std::istream& operator>>(std::istream& stream, Vec3D& vector);

    element dot(const Vector<element, n> &other) const;
    Vector<element, n> operator*(const Vector<element, n>& vector) const;
    Vector<element, n> cross(const Vector<element, n> &other) const;
    element getNorm();
    void normalize();
    Vector<element, n> normalized() const;
    size_t size() const;

    void print(std::ostream& stream = std::cout) const;
};

template<typename element, std::size_t n>
Vector<element, n>::Vector() {
    std::fill(entries.begin(), entries.end(), element{});
}

template<typename element, std::size_t n>
Vector<element, n>::Vector(std::initializer_list<element> list) {
    if (list.size() != n) {
        throw std::invalid_argument("std::initializer_list size does not match Vector size");
    }
    std::copy(list.begin(), list.end(), entries.begin());
}

template<typename element, std::size_t n>
Vector<element, n>::Vector(const Vector &other) {
    entries = other.entries;
}

template<typename element, std::size_t n>
Vector<element, n>::Vector(Vector &&other) {
    entries = std::move(other.entries);
}

template<typename element, std::size_t n>
Vector<element, n>& Vector<element, n>::operator=(const Vector& other) {
    if (this != &other) {
        entries = other.entries;
    }
    return *this;
}

template<typename element, std::size_t n>
Vector<element, n>& Vector<element, n>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        entries = std::move(other.entries);
    }
    return *this;
}

template<typename element, std::size_t n>
void Vector<element, n>::print(std::ostream& stream) const {
    stream << "[";
    for (size_t i = 0; i < n; ++i){
        stream << entries[i];
        if (i != n - 1){
            stream << ", ";
        }
    }
    stream << "]";
}

template<typename element, std::size_t n>
element &Vector<element, n>::operator[](size_t index) {
    return entries[index];
}

template<typename element, std::size_t n>
const element &Vector<element, n>::operator[](size_t index) const {
    return entries[index];
}

template<typename element, std::size_t n>
element Vector<element, n>::dot(const Vector<element, n> &other) const {
    element result = {};
    for (std::size_t i = 0; i < n; ++i) {
        result += entries[i] * other.entries[i];
    }
    return result;
}

template<typename element, std::size_t n>
element dot(const Vector<element, n> &vector1, const Vector<element, n> &vector2) {
    element result = {};
    for (std::size_t i = 0; i < n; ++i) {
        result += vector1[i] * vector2[i];
    }
    return result;
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::cross(const Vector<element, n> &other) const {
    if (n != DIMS_3D){ //
        throw std::invalid_argument("Cross product only defined for 3D vectors");
    }
    Vector<element, n> result;
    result[0] = entries[1] * other[2] - entries[2] * other[1];
    result[1] = entries[2] * other[0] - entries[0] * other[2];
    result[2] = entries[0] * other[1] - entries[1] * other[0];

    return result;
}

template<typename element, std::size_t n>
element Vector<element, n>::getNorm() {
    element result = element{};
    for (auto entry : entries){
        result += entry * entry;
    }
    return sqrt(result);
}

template<typename element, std::size_t n>
void Vector<element, n>::normalize() {
    element norm = getNorm();
    for (size_t i = 0; i < n; ++i){
        entries[i] = entries[i] / norm;
    }
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::normalized() const{
    Vector<element, n> result = *this;
    result.normalize();
    return result;
}

template<typename element, std::size_t n>
Vector<element, n> cross(const Vector<element, n> &vec1, const Vector<element, n> &vec2) {
    if (n != DIMS_3D){ //
        throw std::invalid_argument("Cross product only defined for 3D vectors");
    }
    Vector<element, n> result;
    result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
    return result;
}

template<typename element, std::size_t n>
std::ostream& operator<<(std::ostream &stream, const Vector<element, n> &vector){
    vector.print(stream);
    return stream;
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::operator+(const Vector<element, n> &vector) const {
    Vector<element, n> result;
    for (size_t i = 0; i < n; ++i){
        result[i] = entries[i] + vector[i];
    }
    return result;
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::operator-(const Vector<element, n>& vector) const{
    Vector<element, n> result;
    for (size_t i = 0; i < n; ++i){
        result[i] = entries[i] - vector[i];
    }
    return result;
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::operator*(const Vector<element, n> &vector) const {
    return Vec3D{entries[0] * vector[0], entries[1] * vector[1], entries[2] * vector[2]};
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::operator*(const element item) const {
    Vector<element, n> result;
    for (size_t i = 0; i < n; ++i){
        result[i] = entries[i] * item;
    }
    return result;
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::operator/(const element item) const {
    Vector<element, n> result;
    for (size_t i = 0; i < n; ++i){
        result[i] = entries[i] / item;
    }
    return result;
}

template<typename element2, std::size_t n2>
Vector<element2, n2> operator*(const element2 item, const Vector<element2, n2> &vector) {
    return vector * item;
}

inline std::istream& operator>>(std::istream& stream, Vec3D& vector) {
    for (size_t i = 0; i < DIMS_3D; ++i) {
        stream >> vector[i];
    }
    return stream;
}

template<typename element, std::size_t n>
Vector<element, n> Vector<element, n>::operator-() const {
    Vector<element, n> result;
    for (size_t i = 0; i < n; ++i) {
        result[i] = -entries[i];
    }
    return result;
}

template<typename element, std::size_t n>
bool Vector<element, n>::operator==(const Vector &other) {
    bool result = true;
    for (size_t i = 0; i < n; ++i){
        result = result && (entries[i] == other[i]);
    }
    return result;
}

template<typename element, std::size_t n>
const bool Vector<element, n>::operator==(const Vector &other) const {
    bool result = true;
    for (size_t i = 0; i < n; ++i){
        result = result && (entries[i] == other[i]);
    }
    return result;
}

template<typename element, std::size_t n>
size_t Vector<element, n>::size() const {
    return n;
}

#endif