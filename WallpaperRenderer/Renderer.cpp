#include "Renderer.h"
#include <cassert>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "Factory.h"
/*string encryption */
#include "StringEcnryption.h"
/*string encryption */

Renderer::~Renderer()
{
	this->Release();
}
bool Renderer::Init(const HWND hWnd, const HWND overlay, const int maxVerticies)
{
	/*class attr initialization*/
	this->screenSize.getX() = GetSystemMetrics(SM_CXSCREEN);
	this->screenSize.getY() = GetSystemMetrics(SM_CYSCREEN);
	this->aspectRatio = this->screenSize.getX() / this->screenSize.getY();
	/*direct3d9 initialization*/
	this->pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	assert(this->pD3D && ECRYPT("failed to create Direct3D"));
	D3DPRESENT_PARAMETERS d3dprmts;
	ZeroMemory(&d3dprmts, sizeof(D3DPRESENT_PARAMETERS));
	d3dprmts.BackBufferWidth = this->screenSize.getX();
	d3dprmts.BackBufferHeight = this->screenSize.getY();
	d3dprmts.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dprmts.Windowed = true;
	d3dprmts.BackBufferCount = 1;
	d3dprmts.EnableAutoDepthStencil = false;
	d3dprmts.FullScreen_RefreshRateInHz = 0;
	d3dprmts.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dprmts.MultiSampleQuality = 0;
	d3dprmts.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dprmts.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dprmts.Flags = 0;
	d3dprmts.hDeviceWindow = overlay;
	if (SUCCEEDED(pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, FALSE, D3DMULTISAMPLE_2_SAMPLES, NULL))) {
		d3dprmts.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}
	if (SUCCEEDED(pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, FALSE, D3DMULTISAMPLE_4_SAMPLES, NULL))) {
		d3dprmts.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	}
	this->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlay, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dprmts, &this->pDevice);
	assert(this->pDevice && ECRYPT("FAILED to  create a Direct3DD9Device device"));
	this->pDevice->CreateVertexBuffer(maxVerticies * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &this->vertexBuffer, nullptr);
	assert(this->vertexBuffer && ECRYPT("FAILED to create IDirect3D9VertexBuffer object, verticies = " + std::to_string(maxVerticies)));
	/*some win gdi initializations*/
	this->appDc = GetDC(overlay);
	this->memDc = CreateCompatibleDC(this->appDc);
	this->backupLayout = saveLayout();
	return true;
}
Renderer::Renderer()
{
	this->computeCirclePoints();
}

void Renderer::Paint(const float eTime)
{
	BeginFrame();
	
	//Factory::get().getObject(WPSTARSINTERFACEVERSION)->Paint(eTime, static_cast<void*>(this));
	Factory::get().getObject(FUNNYDOTSITERFACEVERSION)->Paint(eTime, static_cast<void*>(this));
	EndFrame();
}
void Renderer::Update(const float eTime)
{
	this->elapsedTime = eTime;
	POINT cursorPos;
	GetCursorPos(&cursorPos);//cuz we drawing at desktop->rect = full window=> we dont need any transforms
	this->mouseV = Vector2(cursorPos.x - this->mousePos[0], cursorPos.y - this->mousePos[1]);
	this->mousePos = Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
}
void Renderer::BeginFrame()
{
	pDevice->SetSamplerState(D3DVERTEXTEXTURESAMPLER0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	pDevice->SetSamplerState(D3DVERTEXTEXTURESAMPLER0, D3DSAMP_MAXANISOTROPY, D3DTEXF_ANISOTROPIC);
	pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//mode of blend comparing
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//how we calculate src blend
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//(destination)blend
//	pDevice->SetRenderState(D3DRS_LASTPIXEL, TRUE);
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	this->pDevice->SetStreamSource(NULL, nullptr, NULL, NULL);
	this->pDevice->SetFVF(CUSTOMFVF);
	this->pDevice->SetTexture(NULL, nullptr);
	//this->pDevice->Clear(0L, 0, D3DCLEAR_TARGET, 0, 0, 0);
	this->pDevice->Clear(0L, 0, D3DCLEAR_TARGET, Color(255,255,255).convert(), 0, 0);
	this->pDevice->BeginScene();
}
void Renderer::EndFrame()
{
	if (this->renderCursor) {
		this->drawCircle(this->mousePos, this->curcorRadius, Color(0, 255, 0));
	}
	this->pDevice->EndScene();
	this->pDevice->Present(0, 0, 0, 0);
}
void Renderer::Release()
{
	SelectObject(memDc, backupLayout);
	StretchBlt(appDc, 0, 0, screenSize.getX(), screenSize.getY(), memDc, 0, 0, screenSize.getX(), screenSize.getY(), SRCCOPY);
	vertexBuffer->Release();
	pDevice->Release();
	pD3D->Release();
}
void Renderer::Backup()
{
}
/*tools functions*/
void Renderer::computeCirclePoints()
{
	precomputedCirclePoints.clear();
	float step = 2.0 * D3DX_PI / (float)this->circleDensity;
	precomputedCirclePoints.reserve(circleDensity);
	for (float angle = 0.0f; angle < 2.0 * D3DX_PI; angle += step) {
		precomputedCirclePoints.push_back(Vector2(0 + 1 * sin(angle), 0 - 1 * cos(angle)));
	}
}
HBITMAP Renderer::saveLayout()
{
	HBITMAP layout = nullptr;
	layout = CreateCompatibleBitmap(appDc, screenSize.getX(), screenSize.getY());
	SelectObject(memDc, layout);
	StretchBlt(this->memDc, 0, 0, screenSize.getX(), screenSize.getY(), this->appDc, 0, 0, screenSize.getX(), screenSize.getY(), SRCCOPY);
	return layout;
}
/*draw functions*/
void Renderer::drawRectangle(const Vector2& position, const Vector2& size, const Color& color)
{
	const DWORD d_color = color.convert();
	CUSTOMVERTEX verticies[4];
	verticies[0] = { position[0], position[1], 0.f,1.0f,  d_color };
	verticies[1] = { position[0] + size[0], position[1], 0.f,1.0f,  d_color };
	verticies[2] = { position[0], position[1] + size[1], 0.f,1.0f,  d_color };
	verticies[3] = { position[0] + size[0], position[1] + size[1], 0.f,1.0f,  d_color };
	void* p_lock = nullptr;
	this->vertexBuffer->Lock(0, 0, (void**)&p_lock, 0);
	memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
	this->vertexBuffer->Unlock();
	p_lock = nullptr;
	this->pDevice->SetStreamSource(0, this->vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	this->pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	this->pDevice->SetStreamSource(0, nullptr, 0, 0);
}
void Renderer::drawLine(const Vector2& position, const Vector2& to, const Color& color)
{
	const DWORD d_color = color.convert();
	CUSTOMVERTEX verticies[2];
	verticies[0] = { position[0], position[1], 0.f,1.0f,  d_color };
	verticies[1] = { to[0],to[1], 0.f,1.0f,  d_color };
	void* p_lock = nullptr;
	this->vertexBuffer->Lock(0, 0, (void**)&p_lock, 0);
	memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 2);
	this->vertexBuffer->Unlock();
	this->pDevice->SetStreamSource(0, this->vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	this->pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	this->pDevice->SetStreamSource(0, nullptr, 0, 0);
}
void Renderer::drawCircle(const Vector2& position, float r, const Color& color)
{
	void* p_lock = nullptr;
	CUSTOMVERTEX* vertex = new CUSTOMVERTEX[this->circleDensity+1 ];
	const auto _color = color.convert();
	for (int i = 0; i < circleDensity; i++)
	{
		 const auto& data = precomputedCirclePoints.at(i);
		 vertex[i] = {position[0]+ data[0]*r, position[1] +data[1]*r,0.0f, 1.0f, _color };
	}	
	vertex[this->circleDensity] = { position[0] + (float)sin(2 * D3DX_PI) * r, position[1] - (float)cos(2 * D3DX_PI) * r,0.0f, 1.0f, _color };
	this->vertexBuffer->Lock(0, 0, (void**)&p_lock, 0);
	memcpy(p_lock, vertex, sizeof(CUSTOMVERTEX)*(this->circleDensity+1));
	this->vertexBuffer->Unlock();
	this->pDevice->SetStreamSource(0, this->vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	this->pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, this->circleDensity );
	this->pDevice->SetStreamSource(0, 0, 0, 0);
	delete[] vertex;
}
void Renderer::drawFilledCircle(const Vector2& position, float r, const Color& color)
{
	void* p_lock = nullptr;
	CUSTOMVERTEX* vertex = new CUSTOMVERTEX[this->circleDensity + 1];
	const auto _color = color.convert();
	for (int i = 0; i < circleDensity; i++)
	{
		const auto& data = precomputedCirclePoints.at(i);
		vertex[i] = { position[0] + data[0] * r, position[1] + data[1] * r,0.0f, 1.0f, _color };
	}
	vertex[this->circleDensity] = { position[0] + (float)sin(2 * D3DX_PI) * r, position[1] - (float)cos(2 * D3DX_PI) * r,0.0f, 1.0f, _color };
	this->vertexBuffer->Lock(0, 0, (void**)&p_lock, 0);
	memcpy(p_lock, vertex, sizeof(CUSTOMVERTEX) * (this->circleDensity + 1));
	this->vertexBuffer->Unlock();
	this->pDevice->SetStreamSource(0, this->vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	this->pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, this->circleDensity-2);
	this->pDevice->SetStreamSource(0, 0, 0, 0);
	delete[] vertex;
}
float Renderer::getAspectRatio()
{
	return this->aspectRatio;
}
/*getters + setters*/
const Vector2& Renderer::getScreenSize() const
{
	return this->screenSize;
}
const Vector2& Renderer::getMousePos() const
{
	return this->mousePos;
}
const Vector2& Renderer::getMouseAcceleration() const
{
	return this->mouseV;
}
const float Renderer::getMouseRadius()
{
	return this->curcorRadius;
}
void Renderer::setDensity(const unsigned int nDensity)
{
	this->circleDensity = nDensity;
	computeCirclePoints();
}