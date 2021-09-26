#pragma once
#include <vector>
#include "Vector2.h"
#define TIMETOFULLBRIGHT 1.0f//in seconds
#define FULLBRIGHT 150.f

/*

struct DotStruct {
	struct ConnectedDots {
		unsigned int dotsId[2]{ -1,-1 };
		float alpha{ FULLBRIGHT };
		float distance{ 0 };
		ConnectedDots(const ConnectedDots& vOther);
		ConnectedDots(unsigned int ids[2], float alpha = FULLBRIGHT, float distance = 0);
		ConnectedDots();
	};
	int id{ -1 };//id , in before-sorted list of dots
	Vector2 position{ 0,0 };
	Vector2 velocity{ 0,0 };
	float radius{ 4 };
	float alpha{ FULLBRIGHT };//alpha of connected lines(dots)
	void negateVelocity();
	void setVelocity(const Vector2& velocity);
	bool intersectionBBox(const Vector2& other, const Vector2& size = Vector2(1, 1));
	bool intersectionBBox(const Vector2& dot, float dotRadius);
};

class FunnyDots
{
public:
	


	FunnyDots();
	void run();
	const std::vector<dotStruct>& getDotList();
	int getIndex(int index);
	const unsigned __int32 getConnectDistance() {//for debug
		return this->CONNECTDISTANCE;
	};
private:
	void connectDots();
	void moveDots();
	const unsigned short MAXDOTS = 5;
	const unsigned __int32 CONNECTDISTANCE = 300;
	std::vector<FunnyDots::dotStruct>dotsList;
};
inline FunnyDots* fndts = nullptr;
*/