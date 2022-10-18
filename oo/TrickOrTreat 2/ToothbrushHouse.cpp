#include "ToothbrushHouse.h"

ToothbrushHouse::ToothbrushHouse(std::string nameHouse, std::map<int, std::string> candyMap) : House(nameHouse, candyMap)
{
}

ToothbrushHouse::~ToothbrushHouse()
{
  std::cout << "ToothbrushHouse is being destroyed" << std::endl;
}

std::string ToothbrushHouse::ringDoorbell(std::ostringstream &stringStream)
{
  door.lock();
  stringStream << "-";
  std::this_thread::sleep_for(std::chrono::seconds(3));
  door.unlock();
  return "TOOTHBRUSH";
}
