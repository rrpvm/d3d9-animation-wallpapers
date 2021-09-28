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
		float animationCycle{ 0.0f };
		float appendTime{ 0.0f };//time when dot was connected with other dot

	};
	int id{ -1 };//id , in before-sorted list of dots
	Vector2 position{ 0,0 };
	Vector2 velocity{ 0,0 };
	Vector2 forceVelocity{ 0,0 };
	bool shiftVelocity{ false };//experimental
	float radius{ 4 };
	float alpha{ FULLBRIGHT };//alpha of connected lines(dots)
	std::vector<ConnectedDots>connections;
	void negateVelocity(bool byX = true);
	DotStruct();
	DotStruct(const DotStruct& vOther);
	DotStruct(int id, const Vector2& position, float radius, float alpha);
	bool isConnectedWith(int i);
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

	//const std::vector<DotStruct*>& getObjectsList();
	//const unsigned __int32 getConnectDistance() {//for debug
	//	return this->CONNECTDISTANCE;
	//};	
	//static int compareDotsForSort(const void* p1, const void* p2);//баг	
private:
	bool isDotConnected(int id);
	void clipDotsPosition(DotStruct* str, const Vector2& screenSize);
	bool magnitize{ true };
	bool mouseInteractive{ true };
	unsigned short MAXDOTS = 30;
	const unsigned __int32 CONNECTDISTANCE = 225;
	std::vector<DotStruct*>objectsList;
};
