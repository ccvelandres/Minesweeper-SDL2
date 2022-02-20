#pragma once
#include <cstddef>
#include <vector>

class Point {
public:
    int x, y;

    Point() {}
    Point(int x, int y) : x(x), y(y) {};
};

class Vector2 {
public:
    float x, y;

    Vector2() {}
    Vector2(float x, float y) : x(x), y(y) {};

    Vector2& operator+(Vector2 &vect);
    Vector2& operator-(Vector2 &vect);
    Vector2& operator*(Vector2 &vect);
    Vector2& operator/(Vector2 &vect);
};

class Vector3 {
public:
    float x, y, z;

    Vector3() {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

    Vector3& operator+(Vector3 &vect);
    Vector3& operator-(Vector3 &vect);
    Vector3& operator*(Vector3 &vect);
    Vector3& operator/(Vector3 &vect);
};


template<typename T>
class Array2D {
    std::size_t m_rows, m_cols;
    std::vector<T> m_array;
public:
    Array2D(std::size_t rows, std::size_t cols) : 
        m_rows(rows), 
        m_cols(cols), 
        m_array(rows*cols) {}

    ~Array2D() { }

    inline T& at(std::ptrdiff_t row, std::ptrdiff_t col) {
        return m_array[(row*m_cols) + col];
    }

    std::vector<T> &vector() {
        return m_array;
    }
};