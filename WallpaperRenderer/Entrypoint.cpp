#include "Loader.h"
#include "Factory.h"
#include <iostream>
int main() {
	std::ios::sync_with_stdio(false);
	Loader* loader = reinterpret_cast<Loader*>(Factory::get().createObject(Factory::ControllerType::loader, LOADERINTERFACEVERSION));
	loader->Update();
	return 1;
}