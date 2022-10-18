#include "CandyHouse.h"

CandyHouse::CandyHouse(std::string nameHouse, std::map<int, std::string> candyMap) : House(nameHouse, candyMap)
{
  CandyRankingMap = candyMap;
}

CandyHouse::~CandyHouse()
{
  std::cout << "CandyHouse is being destroyed" << std::endl;
}

std::string CandyHouse::ringDoorbell(std::ostringstream &stringStream)
{
  door.lock();
  stringStream << "+";
  std::this_thread::sleep_for(std::chrono::seconds(3));
  int candyChoice = rand() % CandyRankingMap.size() + 1;
  door.unlock();
  return CandyRankingMap.at(candyChoice);
}
