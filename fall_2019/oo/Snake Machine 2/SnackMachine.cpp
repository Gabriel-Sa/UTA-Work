/*
  Gabriel de Sa
  UTA ID: 1001676832
  CSE 1325 Coding Assignment 3
*/

#include "SnackMachine.h"

SnackMachine::SnackMachine(std::string name, int cost, int change, int inventory): machineName{name},
changeLevel{change}, SnackPrice{cost}, inventoryLevel{inventory}
{
}

std::string SnackMachine::getMachineName()
{
  return machineName;
}

bool SnackMachine::buyASnack(int payment, std::string& change, int &status)
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

int SnackMachine::getInventoryLevel()
{
  return inventoryLevel;
}

int SnackMachine::getMaxInventoryCapacity()
{
  return maxInventoryCapacity;
}

bool SnackMachine::incrementInventory(int amountToAdd)
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

std::string SnackMachine::getChangeLevel()
{
  return displayMoney(changeLevel);
}

bool SnackMachine::incrementChangeLevel(int amountToAdd)
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

std::string SnackMachine::getMaxChangeCapacity()
{
  return displayMoney(maxChangeCapacity);
}

std::string SnackMachine::getSnackPrice()
{
  return displayMoney(SnackPrice);
}

std::string SnackMachine::displayMoney(int amount)
{
  std::string dollars{std::to_string(amount/100)};
  std::string cents{std::to_string(abs(amount%100))};
  return "$" + dollars + "." + (cents.size() == 1 ? "0" : "") + cents;
}
// new functions

void SnackMachine::setMachineName(std::string newMachineName)
{
  machineName = newMachineName;
}

void SnackMachine::setSnackPrice(int newSnackPrice)
{
  SnackPrice = newSnackPrice;
}

std::ostream& operator<<(std::ostream& output, SnackMachine& myMachine)
{
  output<< "\nCurrent Inventory Level : " << myMachine.getInventoryLevel() << std::endl
  << "Max Inventory Capacity  : " << myMachine.getMaxInventoryCapacity() << "\n" << std::endl
  << "Current Change Level    : " << myMachine.getChangeLevel() << std::endl
  << "Max Change Capacity     : " << myMachine.getMaxChangeCapacity() << "\n" << std::endl
  << "Current Snack Price is  : " << myMachine.getSnackPrice() << std::endl;
  return output;
}

std::string& CreateSnackOutputLine(SnackMachine& machine, std::string& giveBack)
{
  giveBack = machine.machineName + "|" + std::to_string(machine.SnackPrice) + "|" + std::to_string(machine.changeLevel) + "|" + std::to_string(machine.inventoryLevel);
  return giveBack;
}
