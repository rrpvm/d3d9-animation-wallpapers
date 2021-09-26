#include "WallpaperStars.h"
#include "Factory.h"
#include "Renderer.h"
#include "Loader.h"
#include <iostream>
#include "Utilities.h"

WallpaperStars::~WallpaperStars()
{
	this->Release();
}
void WallpaperStars::Update(const float elapsedTime)
{
	Renderer* render = reinterpret_cast<Renderer*>(Factory::get().getObject(RENDERERINTERFACEVERSION));
	float eTime = elapsedTime / 10.f;//приемлемый вид
	for (Star* str : this->starsList) {
		str->position +=  str->velocity * eTime/LOOPFPS;
		if (str->position[0] - str->radius> render->getScreenSize()[0]) {//если зашли полностью краем
			str->position = Vector2(-str->radius, str->position[1]);
		}
		if (str->position[1] + str->radius <= 0) {
			str->velocity.getY() = !str->velocity[1];
		}
		if (str->position[1] - str->radius >= render->getScreenSize()[1]) {
			str->velocity.getY() = !str->velocity[1];
		}
		Utilities::clamp(str->position.getX(), -str->radius - 0.5f, render->getScreenSize()[0] + str->radius + 0.5f);
		Utilities::clamp(str->position.getY(), -str->radius - 0.5f, render->getScreenSize()[1] + str->radius + 0.5f);
	}
}
void WallpaperStars::Paint(const float elapsedTime)
{
	
}
void WallpaperStars::Paint(const float elapsedTime, void* pRenderer)
{
	Renderer* render = reinterpret_cast<Renderer*>(pRenderer);
	for (Star* str : this->starsList) {
		float whiteGradation = (float)(255 * (1.0f-(str->brightness / 60.f)));
		render->drawFilledCircle(str->position, str->radius , Color(whiteGradation, whiteGradation, whiteGradation,  255));
	}
}
void WallpaperStars::Release()
{
	for (short i = 0; i < this->starsMax; i++)
	{
		delete this->starsList.at(i);
	}
	this->starsList.clear();
}
WallpaperStars::WallpaperStars()
{
	Renderer* render = reinterpret_cast<Renderer*>(Factory::get().getObject(RENDERERINTERFACEVERSION));
	Vector2 screenSize = render->getScreenSize();//optimizing
	int sX, sY;
	sX = screenSize.getX();
	sY = screenSize.getY();
	for (short i = 0; i < this->starsMax; i++)
	{
		int pX = (rand()% static_cast<int>(sX - sX/5)) +sX/5;//ограниче области спавна
		int pY = (rand()% static_cast<int>(sY - sY/5)) +sY/5;//ограниче области спавна
		Vector2 position = Vector2(pX, pY);
		Vector2 velocity= Vector2(rand() % 5 + 5, (rand() % 2 - rand() % 2));
		float r = rand() % 2 + 1;	
		float dst = rand() % 55 + 5;
		Star* str = new Star(position, velocity, r,dst);
		this->starsList.push_back(str);
	}
}
std::vector<WallpaperStars::Star*>& WallpaperStars::getStarsList()
{
	return this->starsList;
}
short WallpaperStars::getStarsMax()
{
	return this->starsMax;
}
void WallpaperStars::setStarsMax(short max)
{
	this->starsMax = max;
}
/*void WallpaperStars::movement_scale(Vector2& pos)
{
	Renderer* render = reinterpret_cast<Renderer*>(Factory::get().getObject(RENDERERINTERFACEVERSION));
	Vector2 screenSize = render->getScreenSize();//optimizing
	float scaleX = 1.0f / screenSize.getX() * LOOPFPS;
	float scaleY = 1.0f / screenSize.getY() * LOOPFPS;
	pos.getX() *= scaleX;;
	pos.getY() *= scaleY;
}*/
Vector2 WallpaperStars::movement_scale(const Vector2& pos)
{
	Renderer* render = reinterpret_cast<Renderer*>(Factory::get().getObject(RENDERERINTERFACEVERSION));
	Vector2 screenSize = render->getScreenSize();//optimizing
	float scaleX = 1.0f / screenSize.getX() * LOOPFPS;
	float scaleY = 1.0f / screenSize.getY() * LOOPFPS;
	return Vector2(pos[0] * scaleX, pos[1] * scaleY);
}
void WallpaperStars::Backup()
{
}
WallpaperStars::Star::Star(const Vector2& pos, const Vector2& vel, float rad, float bri)
{
	this->position = pos;
	this->velocity = vel;
	this->radius = rad;
	this->brightness = bri;
	this->imZ = bri;
}
