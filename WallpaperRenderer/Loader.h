#pragma once
#include <cassert>
#include "WallpaperController.h"
#define MSG_CREATEWORKERW 0x052C
#define LOOPFPS 60
class Loader : public WallpaperController
{
public:
	~Loader() override;
	Loader();
	virtual void Update(const float elapsedTime = 0) override;
	virtual void Paint(const float elapsedTime = 0) override;//not used
	virtual void Release() override;
	virtual void Backup()override {};
	float getCurrentTime();
	int getFps();
private:
	__forceinline bool createWorkerInstance();//can be void
	__forceinline unsigned __int32 getWorkerHandle();//can be void
	__forceinline void createOverlay();
	float elapsedTime{ 0.f };
	float currentTime{ 0.0f };

	unsigned __int32 workerHandle{ NULL };
	unsigned __int32 workerShell{ NULL };
	unsigned __int32 overlayHandle{ NULL };
	int fps{ 0 };
	
};


