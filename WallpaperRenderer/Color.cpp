#include "Color.h"
#include <cassert>
WallpaperRenderer::Color::Color(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255.0f;
}
WallpaperRenderer::Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
WallpaperRenderer::Color::Color(const Color& cV)
{
	this->r = cV.r;
	this->g = cV.g;
	this->b = cV.b;
	this->a = cV.a;
}
float& WallpaperRenderer::Color::operator[](int index) 
{
	assert(index >= 0 && index < 4 && "out of range");
	return *(float*)((unsigned int)(this) + index * sizeof(float));
}
unsigned long WallpaperRenderer::Color::convert() const 
{
	return (unsigned long)(((((int)a) & 0xff) << 24) | ((((int)r) & 0xff) << 16) | ((((int)g) & 0xff) << 8) | (((int)b) & 0xff));
}

const float* WallpaperRenderer::Color::get(int i) const
{
	assert(i >= 0 && i < 4 && "out of range");
	return (float*)((unsigned int)(this) + i * sizeof(float));
}
