#include "Factory.h"
#include "Loader.h"
#include "Renderer.h"
#include "WallpaperStars.h"
Factory::~Factory()
{
	/*for (int i = 0; i < factories.size();i++)
	{
		auto& pair = factories.at(i);
		delete pair.first;
	}*/
	std::map<std::string, WallpaperController*>::iterator it = factories.begin();
	while (it != factories.end())
	{
		delete it->second;
		it++;
	}

}
WallpaperController* Factory::createObject(ControllerType type,  std::string id)
{
	WallpaperController* result = nullptr;
	switch (type)
	{
	case Factory::loader: {
		Loader* loaderV = new Loader();
		this->factories.insert({ id,loaderV });
		result = loaderV;
		break;
	}
	case Factory::renderer: {
		Renderer* renderF = new Renderer();
		this->factories.insert({ id,renderF });
		result = renderF;
		break;
	}
	case Factory::wpstars: {
		WallpaperStars* factory = new WallpaperStars();
		this->factories.insert({ id,factory });
		result = factory;
		break;
	}
	default:
		break;
	}
	return result;
}

WallpaperController* Factory::getObject(std::string id)
{
	auto result = factories.count(id);
	if (result) {
		return  factories.find(id)->second;
	}
	return nullptr;
}
