#pragma once
namespace WallpaperRenderer {
	class Color
	{
	public:
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);
		Color(const Color& cV);
		float& operator[](int index) ;
		unsigned long convert() const;//d3dcolor convert
		const float* get(int i) const;
	private:
		float r, g, b, a;
	};
}


