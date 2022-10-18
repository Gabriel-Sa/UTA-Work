#include "House.h"

#ifndef _CANDYHOUSE_H
#define _CANDYHOUSE_H

class CandyHouse : public House
{
  public:
    CandyHouse(std::string, std::map<int, std::string>);
    ~CandyHouse();

    std::string ringDoorbell(std::ostringstream &);

  protected:
    std::string houseName;
    std::mutex door;
    std::map<int, std::string>CandyRankingMap;
};

#endif
