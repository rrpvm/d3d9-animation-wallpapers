#include "Vector2.h"
#include <cassert>
#include <cmath>
Vector2 Vector2::operator+(const Vector2& vOther)
{
    return Vector2(this->x + vOther.x , this->y + vOther.y);
}
Vector2 Vector2::operator-(const Vector2& vOther)
{
    return Vector2(this->x - vOther.x, this->y - vOther.y);
}
Vector2 Vector2::operator*(const Vector2& vOther)
{
    return Vector2(this->x * vOther.x, this->y * vOther.y);
}
Vector2 Vector2::operator/(const Vector2& vOther) noexcept
{
    return Vector2(this->x / vOther.x, this->y / vOther.y);
}
Vector2& Vector2::operator+=(const Vector2& vOther)
{
    this->x += vOther.x;
    this->y += vOther.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& vOther)
{
    this->x -= vOther.x;
    this->y -= vOther.y;
    return *this;
}
Vector2& Vector2::operator*=(const Vector2& vOther)
{
    this->x *= vOther.x;
    this->y *= vOther.y;
    return *this;
}
Vector2& Vector2::operator/=(const Vector2& vOther) noexcept
{
    this->x /= vOther.x;
    this->y /= vOther.y;
    return *this;
}
Vector2 Vector2::operator+(const float vOther)
{
    return Vector2(this->x + vOther, this->y + vOther);
}
Vector2 Vector2::operator-(const float vOther)
{
    return Vector2(this->x - vOther, this->y - vOther);
}
Vector2 Vector2::operator*(const float vOther)
{
    return Vector2(this->x * vOther, this->y * vOther);
}
Vector2 Vector2::operator/(const float vOther)
{
    return Vector2(this->x / vOther, this->y / vOther);
}
Vector2& Vector2::operator+=(const float vOther)
{
    this->x += vOther;
    this->y += vOther;
    return *this;
}
Vector2& Vector2::operator-=(const float vOther)
{
    this->x -= vOther;
    this->y -= vOther;
    return *this;
}
Vector2& Vector2::operator*=(const float vOther)
{
    this->x *= vOther;
    this->y *= vOther;
    return *this;
}
Vector2& Vector2::operator/=(const float vOther)
{
    this->x /= vOther;
    this->y /= vOther;
    return *this;
}
float& Vector2::getX() 
{
    return this->x;
}
float& Vector2::getY() 
{
    return this->y;
}
float Vector2::operator[](int index) const
{
    assert((index == 0 || index == 1) && "out of range");
    return *(float*)((unsigned __int32)this + index * sizeof(float));
}

float Vector2::Length2D(const Vector2& pos) const
{
    return ((this->x - pos.x) * (this->x - pos.x)) + ((this->y - pos.y) * (this->y - pos.y));
}

float Vector2::Length(const Vector2& pos) const
{
    return sqrt(pow(pos.x-this->x,2) + pow(pos.y - this->y,2));
}
