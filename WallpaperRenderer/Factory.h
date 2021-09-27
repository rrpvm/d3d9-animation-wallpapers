#pragma once
#include "WallpaperController.h"
#include <map>
#include <string>
#define LOADERINTERFACEVERSION "wallpaper:renderer:loader:1.01"
#define RENDERERINTERFACEVERSION "wallpaper:renderer:render:1.05"
#define WPSTARSINTERFACEVERSION "wallpaper:renderer:wpstars:1.01"
#define FUNNYDOTSITERFACEVERSION "wallpaper:renderer:connectedDots:1.02"
class Factory // creates an factories of wallpaper controller, but: I think that Renderer should be deleted from factory(render can be only 1 obj)
{
public:
	~Factory();
	static Factory& get() {
		static Factory instance{};
		return instance;
	};
	enum ControllerType{
		loader = 0,
		renderer,
		wpstars,
		connectedDots,
	};
	WallpaperController* createObject(ControllerType type,  std::string id);
	WallpaperController* getObject(const std::string& id);

private:
	std::map< std::string, WallpaperController*>factories;
};

