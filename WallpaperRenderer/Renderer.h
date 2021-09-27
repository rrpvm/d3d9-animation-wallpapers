#pragma once
#include "WallpaperController.h"
#include "Vector2.h"
#include "Color.h"
#include <vector>
#include <string>
#include <Windows.h>
#define MAXVERTICIES 512
#define CUSTOMFVF (D3DFVF_XYZRHW| D3DFVF_DIFFUSE)
class IDirect3DDevice9;
class IDirect3D9;
class IDirect3DVertexBuffer9;
class IDirect3DSurface9;
using namespace WallpaperRenderer;
class Renderer : public WallpaperController//можно выделить весь рендер d3d9  во вложенный класс, после чего возвращать именно вложенный класс
{
public:
	~Renderer() override;
	Renderer();
	virtual void Paint(const float eTime) override;
	virtual void Update(const float eTime) override;
	virtual void Release() override;
	virtual void Backup() override;//empty function
	bool Init(const HWND hWnd, const HWND overlay, const int maxVerticies);
	void BeginFrame();/*set the render setting and etc.*/
	void EndFrame();/*present buffer, u can switch buffers and render next frame*/
	__forceinline HBITMAP saveLayout();
	__forceinline void setDensity(const unsigned int nDensity);
	__forceinline void computeCirclePoints();



	/*как бы должны быть приватными,но из-за зависимостей других классов оставлю в паблике.ѕервое решение,что пришло в голову:
	 1.наследовать контроллерам рендер, помести функции отрисовки в протектед
	 2. сделать инициализатор к вершнинным буферам + д3д устройству
	*/
	void drawRectangle(const Vector2& position, const Vector2& size, const Color& color);
	void drawLine(const Vector2& position, const Vector2& to, const Color& color);
	void drawCircle(const Vector2& position, float r, const Color& color);
	void drawFilledCircle(const Vector2& position, float r, const Color& color);
	float getAspectRatio();
	const Vector2& getScreenSize() const;
private:
	IDirect3D9* pD3D{ nullptr };
	IDirect3DDevice9* pDevice{ nullptr };
	IDirect3DVertexBuffer9* vertexBuffer{ nullptr };
	unsigned __int32 circleDensity{ 60 };
	HDC appDc{ NULL };
	HDC memDc{ NULL };
	HBITMAP backupLayout{ nullptr };
	Vector2 screenSize{ 0,0 };//движок должен содержать в себе размер экрана, а не рендер. Ќо ,увы,движка нет
	Vector2 mousePos{ 0,0 };
	float elapsedTime{ 0 };
	float aspectRatio{ 0 };
	std::vector<Vector2>precomputedCirclePoints;
};
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};