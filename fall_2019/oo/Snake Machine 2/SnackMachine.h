#ifndef SNACK_MACHINE_H
#define SNACK_MACHINE_H

#include <iostream>
#include <string>

class SnackMachine
{
  public:
    SnackMachine(std::string name = "New Machine", int cost = 50 , int change = 500, int inventory = 100);
    std::string getMachineName();
    bool buyASnack(int, std::string &, int &);
    int getInventoryLevel();
    int getMaxInventoryCapacity();
    bool incrementInventory(int);
    std::string getChangeLevel();
    bool incrementChangeLevel(int);
    std::string getMaxChangeCapacity();
    std::string getSnackPrice();
    std::string displayMoney(int);
    void setMachineName(std::string);
    void setSnackPrice(int);
    friend std::ostream& operator<<(std::ostream&, SnackMachine&);
    friend std::string& CreateSnackOutputLine(SnackMachine& machine, std::string& giveBack);

  private:
    std::string machineName;
    int changeLevel;
    int maxChangeCapacity = 5000;
    int SnackPrice;
    int inventoryLevel;
    int maxInventoryCapacity = 100;
};


#endif
