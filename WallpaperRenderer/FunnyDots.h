#pragma once
#include <vector>
#include "Vector2.h"
#include "WallpaperController.h"
#define TIMETOFULLBRIGHT 1.0f//in seconds
#define FULLBRIGHT 150.f
#define FUNNYDOTSSPEEDMULTIPLIER 0.5f
struct DotStruct {
	struct /*alignas(16)*/ ConnectedDots {//12 bytes (0xC)
		unsigned int dot;//4bytes
		float alpha{ FULLBRIGHT };//4 bytes
		float distance{ 0 };//4 bytes
	//	ConnectedDots(const ConnectedDots& vOther);
	//	ConnectedDots(unsigned int ids[2], float alpha = FULLBRIGHT, float distance = 0);
		//ConnectedDots();
	};
	int id{ -1 };//id , in before-sorted list of dots
	Vector2 position{ 0,0 };
	Vector2 velocity{ 0,0 };
	Vector2 forceVelocity{ 0,0 };
	float radius{ 4 };
	float alpha{ FULLBRIGHT };//alpha of connected lines(dots)
	std::vector<ConnectedDots>connections;
	void negateVelocity(bool byX = true);
	//void setVelocity(const Vector2& velocity);
	//bool intersectionBBox(const Vector2& other, const Vector2& size = Vector2(1, 1));
	//bool intersectionBBox(const Vector2& dot, float dotRadius);
	DotStruct();
	DotStruct(const DotStruct& vOther);
	DotStruct(int id, const Vector2& position, float radius, float alpha);
};
class FunnyDots : public WallpaperController
{
public:
	FunnyDots(uint16_t nObjects);
	~FunnyDots()override;
	virtual void Update(const float elapsedTime) override;
	virtual void Paint(const float elapsedTime) override;
	virtual void Paint(const float elapsedTime,void* pRender) override;
	virtual void Release() override;
	virtual void Backup()override {};

	const std::vector<DotStruct*>& getObjectsList();
	const unsigned __int32 getConnectDistance() {//for debug
		return this->CONNECTDISTANCE;
	};
	const unsigned short getObjectCount();
	static int compareDotsForSort(const void* p1, const void* p2);//баг
	bool isDotConnected(int id);
private:
	unsigned short MAXDOTS = 30;
	const unsigned __int32 CONNECTDISTANCE = 300;
	std::vector<DotStruct*>objectsList;
};
