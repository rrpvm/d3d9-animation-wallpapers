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
	for (WallpaperStars::Star* str : this->starsList) {
		str->position +=  str->velocity * (elapsedTime* SPEEDMULTIPLIERSTARS) /LOOPFPS;//do move
		int accumulateVX = rand() % 250 / render->getAspectRatio();//0.250 
		str->velocity.getX() += static_cast<float>(accumulateVX) / 1000.f;
		if (str->position[0] - str->radius> render->getScreenSize()[0]) {//если зашли полностью краем
			str->position = Vector2(-str->radius, str->position[1]);
			str->velocity.getX() = rand() % 5 + 5;
		}
		if (str->position[1] + str->radius <= 0) {
			str->velocity.getY() = !str->velocity[1];
		}
		if (str->position[1] - str->radius >= render->getScreenSize()[1]) {
			str->velocity.getY() = !str->velocity[1];
		}
		Utilities::clamp(str->position.getX(), -str->radius - 0.5f, render->getScreenSize()[0] + str->radius + 0.5f);
		Utilities::clamp(str->position.getY(), -str->radius - 0.5f, render->getScreenSize()[1] + str->radius + 0.5f);
		if (str->hasEffect) {
			if (str->animationCycle < 1.0f && !str->inverseCycle) {//1.0f == 100%
				str->animationCycle += (elapsedTime / 1000.f)/EFFECTTIME;//ms to s
				if (str->animationCycle >= 1.0f)str->inverseCycle = true;
			}
			else {				
				str->animationCycle -= (elapsedTime / 1000.f) / EFFECTTIME;//ms to s
				if (str->animationCycle <= 0.f)str->inverseCycle = false;
			}
			Utilities::clamp(str->animationCycle,0.0f, 1.0f);
		}
		//TO DO:добавить акуммулятивную скорость по рандому(чтобы было динамичней)
	}
}
void WallpaperStars::Paint(const float elapsedTime)
{
	
}
void WallpaperStars::Paint(const float elapsedTime, void* pRenderer)
{
	Renderer* render = reinterpret_cast<Renderer*>(pRenderer);
	for (WallpaperStars::Star* str : this->starsList) {
		float whiteGradation = (float)(255 * (1.0f-(str->brightness / 35.f)));//if animcycle==1.0f;;;;;;; 35.f -> max dst(see WallpaperStars::WallpaperStars());
		if (str->hasEffect) {
			whiteGradation = whiteGradation * str->animationCycle;//now it is blue gradation
			render->drawFilledCircle(str->position, str->radius, Color(whiteGradation/2, whiteGradation/2, whiteGradation, 255));
		}
		else render->drawFilledCircle(str->position, str->radius , Color(whiteGradation, whiteGradation, whiteGradation,  255));
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
		int pX = (rand() % static_cast<int>(sX) - 4);
		int pY = (rand() % static_cast<int>(sY) - 4);
		Vector2 position = Vector2(pX, pY);
		Vector2 velocity= Vector2(rand() % 5 + 5, (rand() % 2 - rand() % 1));
		float r = rand() % 2 + 2;	
		float dst = rand() % 30 + 5;
		bool effect = (rand() % 4 == 0);//25%
		Star* str = new Star(position, velocity, r,dst,effect);
		float animationCycle = static_cast<float>(rand() % 1000)/1000.f;
		str->animationCycle = animationCycle;
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
WallpaperStars::Star::Star(const Vector2& pos, const Vector2& vel, float rad, float bri,bool effect)
{
	this->position = pos;
	this->velocity = vel;
	this->radius = rad;
	this->brightness = bri;
	this->imZ = bri;
	this->hasEffect = effect;
}
