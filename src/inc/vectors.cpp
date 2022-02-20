#include <vectors.hpp>

// -----------------------------------------
// Point



// -----------------------------------------
// Vector2

Vector2& Vector2::operator+(Vector2 &vect) {
    this->x += vect.x;
    this->y += vect.y;
    return *this;
}

Vector2& Vector2::operator-(Vector2 &vect) {
    this->x -= vect.x;
    this->y -= vect.y;
    return *this;
}

Vector2& Vector2::operator*(Vector2 &vect) {
    this->x *= vect.x;
    this->y *= vect.y;
    return *this;
}

Vector2& Vector2::operator/(Vector2 &vect) {
    this->x /= vect.x;
    this->y /= vect.y;
    return *this;
}

// -----------------------------------------
// Vector3

Vector3& Vector3::operator+(Vector3 &vect) {
    this->x += vect.x;
    this->y += vect.y;
    this->z += vect.z;
    return *this;
}

Vector3& Vector3::operator-(Vector3 &vect) {
    this->x -= vect.x;
    this->y -= vect.y;
    this->z -= vect.z;
    return *this;
}
Vector3& Vector3::operator*(Vector3 &vect) {
    this->x *= vect.x;
    this->y *= vect.y;
    this->z *= vect.z;
    return *this;
}
Vector3& Vector3::operator/(Vector3 &vect) {
    this->x /= vect.x;
    this->y /= vect.y;
    this->z /= vect.z;
    return *this;
}