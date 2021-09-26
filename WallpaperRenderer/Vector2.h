#pragma once
class Vector2
{
public:
	Vector2() {
		y = 0;
		x = 0;
	}
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2(const Vector2& vOther) {
		this->x = vOther.x;
		this->y = vOther.y;
	}
	Vector2 operator+(const Vector2& vOther);
	Vector2 operator-(const Vector2& vOther);
	Vector2 operator*(const Vector2& vOther);
	Vector2 operator/(const Vector2& vOther) noexcept;
	Vector2& operator+=(const Vector2& vOther);
	Vector2& operator-=(const Vector2& vOther);
	Vector2& operator*=(const Vector2& vOther);
	Vector2& operator/=(const Vector2& vOther) noexcept;
	Vector2 operator+(const float vOther);
	Vector2 operator-(const float vOther);
	Vector2 operator*(const float vOther);
	Vector2 operator/(const float vOther);
	Vector2& operator+=(const float vOther);
	Vector2& operator-=(const float vOther);
	Vector2& operator*=(const float vOther);
	Vector2& operator/=(const float vOther);
	bool operator==(const Vector2& vOther)  {
		bool result = (x == vOther.x) && (y == vOther.y);
		return  result;
	}
	float& getX();//будем без сеттера=>(lvalue = link(reference))
	float& getY();//
	float operator[](int index) const;
	float Length2D(const Vector2& pos) const;
	float Length(const Vector2& pos) const;

private:
	float x, y;
};

