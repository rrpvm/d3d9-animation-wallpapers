#pragma once
class Utilities
{
public:
	template<typename T>
	static void clamp(T& v, T min, T max); 
private:
};

template<typename T>
inline void Utilities::clamp(T& v, T min, T max)
{
	if (v < min)v = min;
	if (v > max)v = max;
}
