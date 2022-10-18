#include "House.h"
#include "TrickOrTreater.h"
#include <vector>

House::House(std::string nameHouse, std::map<int, std::string> candyMap) : houseName{nameHouse}, CandyRankingMap{candyMap}
{
}

std::string House::ringDoorbell(std::ostringstream & stringStream)
{
  door.lock();
    stringStream << "*";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    int candyChoice = rand() % CandyRankingMap.size() + 1;
  door.unlock();
  return CandyRankingMap.at(candyChoice);
}
