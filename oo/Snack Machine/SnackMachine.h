/*
  Gabriel de Sa
  UTA ID: 1001676832
  CSE 1325 Coding Assignment 2
*/
#include <iostream>

class SnackMachine
{
  public:
    SnackMachine(std::string name, int cost, int change, int inventory) : machineName{name}, changeLevel{change},
    SnackPrice{cost}, inventoryLevel{inventory}
    {
    }

    std::string getMachineName()
    {
      return machineName;
    }

    bool buyASnack(int payment, std::string& change, int &status)
    {
      bool worked = false;
      int calculatedChange;
      if(payment - SnackPrice == 0)
      {
        status = 0;
        worked = true;
        incrementInventory(-1);
        incrementChangeLevel(payment);
      }
      else if(payment - SnackPrice > 0 && (payment-SnackPrice) < changeLevel)
      {
        calculatedChange = payment - SnackPrice;
        change = displayMoney(calculatedChange);
        incrementInventory(-1);
        incrementChangeLevel(-calculatedChange);
        incrementChangeLevel(payment);
        worked = true;
        status = 1;
      }
      else if(payment < SnackPrice)
      {
        status = 2;
      }
      else if((payment - SnackPrice) > changeLevel)
      {
        status = 3;
      }
      else
        status = 4;
      return worked;
    }

    int getInventoryLevel()
    {
      return inventoryLevel;
    }

    int getMaxInventoryCapacity()
    {
      return maxInventoryCapacity;
    }

    bool incrementInventory(int amountToAdd)
    {
      bool worked = false;
      int test = inventoryLevel+amountToAdd;
      if(test <= maxInventoryCapacity)
      {
        inventoryLevel = inventoryLevel + amountToAdd;
        worked = true;
      }
      return worked;
    }

    std::string getChangeLevel()
    {
      return displayMoney(changeLevel);
    }

    bool incrementChangeLevel(int amountToAdd)
    {
      bool worked = false;
      int test = changeLevel+amountToAdd;
      if(test <= maxChangeCapacity)
      {
        changeLevel = changeLevel + amountToAdd;
        worked = true;
      }
      return worked;
    }

    std::string getMaxChangeCapacity()
    {
      return displayMoney(maxChangeCapacity);
    }

    std::string getSnackPrice()
    {
      return displayMoney(SnackPrice);
    }

    std::string displayMoney(int amount)
    {
      std::string dollars{std::to_string(amount/100)};
      std::string cents{std::to_string(abs(amount%100))};
      return "$" + dollars + "." + (cents.size() == 1 ? "0" : "") + cents;
    }

  private:
    std::string machineName;
    int changeLevel;
    int maxChangeCapacity = 5000;
    int SnackPrice;
    int inventoryLevel;
    int maxInventoryCapacity = 100;
};
