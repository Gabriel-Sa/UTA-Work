#include "TrickOrTreater.h"

int TrickOrTreater::ImDone = 0;

TrickOrTreater::TrickOrTreater(std::string name, std::vector<House*> houses) : name{name}, ListOfHouses{houses}
{
}

std::string TrickOrTreater::getName()
{
  return name;
}

void TrickOrTreater::startThread()
{
  TOTThreadPtr = new std::thread(&TrickOrTreater::GoTrickOrTreating, this);
}

void TrickOrTreater::joinThread()
{
  this->TOTThreadPtr->join();
  delete(TOTThreadPtr);
}

void TrickOrTreater::GoTrickOrTreating()
{
  int speed = 1;
  std::string candyReceived = "nothing";
  for(auto& x : ListOfHouses)
  {
    speed = rand() % 500 + 1;
    Walk(speed);
    candyReceived = x->ringDoorbell(path);
    Bucket[candyReceived]++;
  }
  ImDone = ImDone + 1;
}

void TrickOrTreater::Walk(int speed)
{
  int i = 0;
  for(i = 0; i < 11; i++)
  {
    path << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(speed));
  }
}

std::string TrickOrTreater::getPath()
{
  return path.str();
}

std::ostream& operator<<(std::ostream& output, TrickOrTreater& TOT)
{
  output << TOT.name << " - ";
  for(auto x : TOT.Bucket)
  {
    output << x.second << " " << x.first << ", ";
  }
  return output;
}
