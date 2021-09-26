#pragma once
class WallpaperController
{
public:
	virtual ~WallpaperController() {};
	virtual void Update(const float elapsedTime) = 0; // logic
	virtual void Paint(const float elapsedTime) = 0;//draw
	virtual void Paint(const float elapsedTime, void* pRenderer) {};//костыль
	virtual void Release() = 0;	//release
private:
	virtual void Backup() = 0;//restore
};

