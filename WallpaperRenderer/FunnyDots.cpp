#include "FunnyDots.h"
#include "Renderer.h"
#include "Factory.h"
#include "Loader.h"
#include "Utilities.h"
#include <cassert>
#include <iostream>
FunnyDots::FunnyDots(uint16_t nObjects)
{
	//alert:funnydots object must be created after RENDERER. because of GetScreenSize call
	Renderer* renderPtr = reinterpret_cast<Renderer*>(Factory::get().getObject(RENDERERINTERFACEVERSION));
	assert(renderPtr && "FunnyDots::FunnyDots()=> Renderer == nullptr");
	const Vector2& screenSize = renderPtr->getScreenSize();
	this->MAXDOTS = nObjects;
	this->objectsList.reserve(this->MAXDOTS);//maybe * sizeof DotStruct
	for (uint16_t i = 0u; i < nObjects; i++) {
		DotStruct* dstr = new DotStruct();
		int vX = (rand() % 2) ? ((rand() % 5) + 3) : (-(rand() % 5) - 3);//pos or negate == 50/50(flip)
		int vY = (rand() % 2) ? ((rand() % 5) + 3) : (-(rand() % 5) - 3);//pos or negate == 50/50(flip)
		dstr->id = i;
		dstr->position = Vector2(rand() % static_cast<int>(screenSize[0]), rand() % static_cast<int>(screenSize[1]));	
		dstr->velocity = Vector2(vX, vY);
		dstr->radius = 4.0f;
		dstr->alpha = 255;
		dstr->connections.clear();
		this->objectsList.emplace_back(dstr);
	}
}
FunnyDots::~FunnyDots()
{
	this->Release();
}
void FunnyDots::Update(const float elapsedTime)
{
	Renderer* render = reinterpret_cast<Renderer*>(Factory::get().getObject(RENDERERINTERFACEVERSION));
	const Vector2& screenSize = render->getScreenSize();
	//std::qsort(objectsList.data(), objectsList.size(),sizeof(DotStruct), FunnyDots::compareDotsForSort);
	for (int i = 0; i < objectsList.size(); i++)
	{
		DotStruct* object = objectsList.at(i);
		//float accvX, accvY;
		//accvX = static_cast<float>(rand() % 250) / 1000.f / render->getAspectRatio();
		//accvY = static_cast<float>(rand() % 150) / 1000.f;//0.150
		//object->velocity.getX() += object->velocity.getX() > 0 ? accvX : -accvX;
		//object->velocity.getY() += object->velocity.getY() > 0 ? accvY : -accvY;
		Vector2 nVelocity = object->velocity * (elapsedTime * FUNNYDOTSSPEEDMULTIPLIER / LOOPFPS);
		object->position += nVelocity + object->forceVelocity;
	//	object->forceVelocity = Vector2(0, 0);
		/*позиция- центр -> левый край = pos - rad/2*/
		if (object->position[0] - object->radius / 2 <= 0.0f || object->position[0] + object->radius / 2 >= screenSize[0]) {
			object->negateVelocity(true);
		/*	const float nVelocityX = (float)(rand() % 5 + 3);
			if (object->velocity.getX() > 0) {//=>столкновение с левым краем
				object->velocity.getX() = nVelocityX;
				object->forceVelocity = Vector2(1, 0);
			}
			else {
				object->velocity.getX() = -nVelocityX;
				object->forceVelocity = Vector2(-1, 0);
			}*/
		}
		if (object->position[1] - object->radius / 2 <= 0 || object->position[1] + object->radius / 2 >= screenSize[1]) {
			object->negateVelocity(false);
			/*const float nVelocityY = (float)(rand() % 5 + 3);
			if (object->velocity.getY() > 0) {//=>столкновение с левым краем
				object->velocity.getY() = nVelocityY;
				object->forceVelocity = Vector2(1, 0);
			}
			else {
				object->velocity.getY() = -nVelocityY;
				object->forceVelocity = Vector2(-1, 0);
			}*/
		}
		Utilities::clamp(object->position.getX(), -object->radius / 2, screenSize[0] + object->radius / 2);
		Utilities::clamp(object->position.getY(), -object->radius / 2, screenSize[1] + object->radius / 2);
		object->connections.clear();
		for (int j = i + 1; j < objectsList.size(); j++)
		{
			DotStruct* innerObject = objectsList.at(j);
			if ((object->position + object->radius / 2).Length(innerObject->position + innerObject->radius / 2) <= this->CONNECTDISTANCE) {
				DotStruct::ConnectedDots str;
				str.dot = j;
				str.alpha = 255.0f;
				object->connections.push_back(str);
			}
			//if()
		}
	}
}
void FunnyDots::Paint(const float elapsedTime)
{
}
void FunnyDots::Paint(const float elapsedTime, void* pRender)
{
	Renderer* render = reinterpret_cast<Renderer*>(pRender);
	for (const auto& object : objectsList)
	{
		for (auto& v : object->connections)
		{
			render->drawLine(object->position, this->objectsList.at(v.dot)->position, Color(88.f, 88.f, 88.f, 88.f));
		}
		if(object->connections.size()>0 || isDotConnected(object->id))render->drawFilledCircle(object->position, object->radius, Color(200, 200, 200, object->alpha));	
	}
}
void FunnyDots::Release()
{
	for (DotStruct* dstr : this->objectsList) {
		delete dstr;
	}
	this->objectsList.clear();
}
const std::vector<DotStruct*>& FunnyDots::getObjectsList()
{
	return objectsList;
}
const unsigned short FunnyDots::getObjectCount()
{
	return this->MAXDOTS;
}
int FunnyDots::compareDotsForSort(const void* p1, const void* p2)
{
	DotStruct* ds1 = (DotStruct*)(p1);
	DotStruct* ds2 = (DotStruct*)(p2);
	return ds1->position.Length2D(Vector2(0, 0)) - ds2->position.Length2D(Vector2(0, 0));
}
bool FunnyDots::isDotConnected(int id)
{
	for (const auto& object : objectsList)
	{
		for (const auto& v : object->connections)
		{
			if (v.dot == id)return true;
		}		
	}
	return false;
}
void DotStruct::negateVelocity(bool byX)
{
	if (byX)this->velocity.getX() = -this->velocity.getX();
	else this->velocity.getY() = -this->velocity.getY();
}
DotStruct::DotStruct()
{
	this->id = 0;
	this->position = Vector2(0,0);
	this->radius = 4.f;
	this->alpha = 255.0f;
	this->forceVelocity = Vector2(0,0);
	this->connections.clear();
}
DotStruct::DotStruct(const DotStruct& vOther)
{
	this->id = vOther.id;
	this->position = vOther.position;
	this->radius = vOther.radius;
	this->alpha = vOther.alpha;
	this->forceVelocity = vOther.forceVelocity;
	this->connections = vOther.connections;
}
DotStruct::DotStruct(int id, const Vector2& position, float radius, float alpha)
{
	this->id = id;
	this->position = position;
	this->radius = radius;
	this->alpha = alpha;
	this->forceVelocity = Vector2(0, 0);
	this->connections.clear();
}
