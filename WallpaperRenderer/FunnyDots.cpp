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
	Loader* loader = reinterpret_cast<Loader*>(Factory::get().getObject(LOADERINTERFACEVERSION));
	const Vector2& screenSize = render->getScreenSize();
	for (int i = 0; i < objectsList.size(); i++)
	{
		DotStruct* object = objectsList.at(i);
		Vector2 nVelocity = object->velocity * (elapsedTime * FUNNYDOTSSPEEDMULTIPLIER / LOOPFPS);
		if (object->shiftVelocity)nVelocity = Vector2(0, 0);
		object->position += nVelocity + object->forceVelocity;
		object->shiftVelocity = false;
		object->forceVelocity = Vector2(0, 0);
		clipDotsPosition(object, screenSize);
		for (int j = i + 1; j < objectsList.size(); j++)//add new connections to the dot
		{
			DotStruct* innerObject = objectsList.at(j);
			if (!object->isConnectedWith(j) && (object->position + object->radius / 2).Length(innerObject->position + innerObject->radius / 2) <= this->CONNECTDISTANCE) {
				DotStruct::ConnectedDots str; 
				str.dot = j;
				str.alpha = 255.0f;
				str.animationCycle = 0.0f;
				str.appendTime = loader->getCurrentTime();
				object->connections.push_back(str);
			}
			//if()
		}
		for (int x = 0; x < object->connections.size(); x++)//clear old connections with the dot + animate new connections(можно засунуть в цикл выше, но так тяжело понять код
		{
			auto& connection = object->connections.at(x);
			DotStruct* dtsr = this->objectsList.at(connection.dot);
			//если мы уже не соединены с точкой - анимация исчезания
			if ((object->position + object->radius / 2).Length(dtsr->position + dtsr->radius / 2) > this->CONNECTDISTANCE) {
				if (connection.animationCycle < 1.0f) {
					/*animate*/
					connection.animationCycle += (elapsedTime / 1000.f) / TIMETOFULLBRIGHT;
					Utilities::clamp(connection.animationCycle, 0.0f, 1.01f);
					connection.alpha = FULLBRIGHT * (1.0f - connection.animationCycle);
					continue;
				}
				else object->connections.erase(object->connections.begin() + x);
			}
			else {
				//анимация проигрывается единожды-> нужно знать время появления
				if (loader->getCurrentTime() - connection.appendTime <= TIMETOFULLBRIGHT * 1000.0f && connection.animationCycle < 1.0f) {
					connection.animationCycle += (elapsedTime / 1000.f) / TIMETOFULLBRIGHT;
					Utilities::clamp(connection.animationCycle, 0.0f, 1.01f);
					connection.alpha = FULLBRIGHT * connection.animationCycle;
				}
				else {
					/*если мы отработали анимацию появления - сброс*/
					connection.animationCycle = 0.0f;
					connection.alpha = FULLBRIGHT;
				}
			}
		}
		if (this->mouseInteractive && object->position.Length(render->getMousePos()) < render->getMouseRadius()) {
			if (render->getMouseAcceleration().DotProduct() == 0) {
				object->position -= object->velocity/2.0f;//обратное направление	
				object->negateVelocity();
				object->negateVelocity(false);
				object->shiftVelocity = true;
			}
			else {
				object->position += (render->getMouseAcceleration()*1.25f);
				object->shiftVelocity = true;
			}
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
			//render->drawLine(object->position, this->objectsList.at(v.dot)->position, Color(111.f, 111.f, 111.f, 111.f));
			render->drawLine(object->position, this->objectsList.at(v.dot)->position, Color(0.f, 0.f, 0.f, v.alpha));
		}
		//if(object->connections.size()>0 || isDotConnected(object->id))render->drawFilledCircle(object->position, object->radius, Color(210, 210, 210, object->alpha));	
		if(object->connections.size()>0 || isDotConnected(object->id))render->drawFilledCircle(object->position, object->radius, Color(0, 0, 0, object->alpha));
		else render->drawFilledCircle(object->position, object->radius, Color(100.f, 100.f, 100.f, object->alpha));
	}
}
void FunnyDots::Release()
{
	for (DotStruct* dstr : this->objectsList) {
		delete dstr;
	}
	this->objectsList.clear();
}
/*
const std::vector<DotStruct*>& FunnyDots::getObjectsList()
{
	return objectsList;
}*/
/*
const unsigned short FunnyDots::getObjectCount()
{
	return this->MAXDOTS;
}
int FunnyDots::compareDotsForSort(const void* p1, const void* p2)
{
	DotStruct* ds1 = (DotStruct*)(p1);
	DotStruct* ds2 = (DotStruct*)(p2);
	return ds1->position.Length2D(Vector2(0, 0)) - ds2->position.Length2D(Vector2(0, 0));
}*/
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
void FunnyDots::clipDotsPosition(DotStruct* object, const Vector2& screenSize)
{
	if (object->position[1] <= object->radius / 2.0f) {
		object->negateVelocity(false);
		object->forceVelocity.getY() += (object->radius / 2.0f) - object->position[1];
	}
	else if (object->position[1] >= screenSize[1] + object->radius / 2.0f)
	{
		object->negateVelocity(false);
		object->forceVelocity.getY() -= object->position[1] - screenSize[1] - object->radius / 2.0f;
	}
	if (object->position[0] <= object->radius / 2.0f) {
		object->negateVelocity();
		object->forceVelocity.getX() += (object->radius / 2.0f) - object->position[0];
	}
	else if (object->position[0] >= screenSize[0] + object->radius / 2.0f)
	{
		object->negateVelocity();
		object->forceVelocity.getX() -= object->position[0] - screenSize[0]-object->radius/2.0f;
	}
	Utilities::clamp(object->position.getX(), -object->radius / 2.0f, screenSize[0] + object->radius/2.0f);
	Utilities::clamp(object->position.getY(), -object->radius / 2.0f, screenSize[1] + object->radius / 2.0f);
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
	this->shiftVelocity = false;
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
	this->shiftVelocity = vOther.shiftVelocity;
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
bool DotStruct::isConnectedWith(int i)
{
	for (const auto& connection : connections)
	{
		if (connection.dot == i)return true;
	}
	return false;
}
