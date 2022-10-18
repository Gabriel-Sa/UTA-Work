#include "House.h"
#include "TrickOrTreater.h"
#include <vector>

House::House(std::string nameHouse, std::map<int, std::string> candyMap) : houseName{nameHouse}, CandyRankingMap{candyMap}
{
}

House::~House()
{
  std::cout << "House is being destroyed" << std::endl;
}

std::string House::ringDoorbell(std::ostringstream & stringStream)
{
  return 0;
}
