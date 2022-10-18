#include "House.h"

class ToothbrushHouse : public House
{
  public:
    ToothbrushHouse(std::string, std::map<int, std::string>);
    ~ToothbrushHouse();
    std::string ringDoorbell(std::ostringstream &);
    
  protected:
      std::string houseName;
      std::mutex door;
};
