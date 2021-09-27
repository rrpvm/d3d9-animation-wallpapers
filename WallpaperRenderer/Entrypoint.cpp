#include "Loader.h"
#include "Factory.h"
#include <Windows.h>
int main() {
	Loader* loader = reinterpret_cast<Loader*>(Factory::get().createObject(Factory::ControllerType::loader, LOADERINTERFACEVERSION));
	loader->Update();
	return 1;
}
/*появилась идея- сделать класс-управление всеми контроллерами, сделать 1 callback функцию в контроллерах. После чего в классе-управлении вызывать все колл-функции*/