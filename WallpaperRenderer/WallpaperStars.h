#pragma once
#include "WallpaperController.h"
#include "Vector2.h"
#include <vector>
#define EFFECTTIME 1.0f
#define SPEEDMULTIPLIERSTARS 0.5f
class WallpaperStars : public WallpaperController
{
public:
	struct Star {
		Vector2 position{ 0.f, 0.f };
		Vector2 velocity{ 0.f, 0.f };
		float radius{ 0.f };
		float brightness{ 0.f };
		float imZ{ 0.f };//�������� ��������� (�������)
		bool hasEffect{ false };
		float animationCycle{ 0.0f };
		bool inverseCycle{ false };
		Star(const Star& vStar) {
			if (this == &vStar)return;//optimization
			this->position = vStar.position;
			this->velocity = vStar.velocity;
			this->radius = vStar.radius;
			this->brightness = vStar.brightness;
			this->imZ = vStar.imZ;
			this->hasEffect = hasEffect;
			this->animationCycle = vStar.animationCycle;
			this->inverseCycle = vStar.inverseCycle;
		}
		Star(const Vector2& pos, const Vector2& vel, float rad, float bri,bool effect);
	};
	virtual ~WallpaperStars() override;
	virtual void Update(const float elapsedTime) override; // logic
	virtual void Paint(const float elapsedTime) override;//draw
	virtual void Paint(const float elapsedTime, void* pRenderer) override;//����� ������� ���
	virtual void Release() override;	//release
	WallpaperStars();
	std::vector<Star*>& getStarsList();
	short getStarsMax();
	void setStarsMax(short max);
	//void movement_scale(Vector2& pos);
	Vector2 movement_scale(const Vector2& pos);
private:
	virtual void Backup();//restore
	std::vector<Star*>starsList;
	short starsMax{ 80 };
};

