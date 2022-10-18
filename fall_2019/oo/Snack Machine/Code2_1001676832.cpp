/*
  Gabriel de Sa
  UTA ID: 1001676832
  CSE 1325 Coding Assignment 2
*/
#include <iostream>
#include <string>
#include "SnackMachine.h"
using namespace std;

enum status{
  worked_noChange,
  worked_withChange,
  error_insufficientPayment,
  error_insufficentChange,
  unknown
};

void displayMenu(int& choice, SnackMachine& myMachine)
{
  cout << "\n" << myMachine.getMachineName() <<"\n"<< endl;
  cout << "0. Walk away" << endl;
  cout << "1. Buy a Snack" << endl;
  cout << "2. Restock Machine" << endl;
  cout << "3. Add Change" << endl;
  cout << "4. Display Machine Info" << endl;
  cout << "\nMake a selection: ";
  cin >> choice;
  while(cin.fail())
  {
    cin.clear();
    cin.ignore(50, '\n');
    cout << "You must enter a number. Reenter your choice: ";
    cin >> choice;
  }
  while(choice > 4 || choice < 0)
  {
    cin.clear();
    cin.ignore(50, '\n');
    cout << "You must enter a number between 0 and 4. Reenter your choice: ";
    cin >> choice;
  }
}

void userDecision(const int choice, SnackMachine& myMachine)
{
  int amountToAdd;
  int payment, status = unknown;
  string change;
  switch(choice)
  {
    case 0:
      cout << "Are you SURE you don't really want a snack?" << endl;
      break;

    case 1:
      if(myMachine.getInventoryLevel() != 0)
      {
        cout << "A snack costs: " << myMachine.getSnackPrice() << endl;
        cout << "Insert a payment: ";
        cin >> payment;
        while(cin.fail())
        {
          cin.clear();
          cin.ignore(50, '\n');
          cout << "Answer must be a number, reenter your payment: ";
          cin >> payment;
        }
        if(myMachine.buyASnack(payment, change, status))
        {
          switch(status)
          {
            case worked_noChange:
              cout << "Thank you for your exact payment of " << myMachine.displayMoney(payment) << ". Here is your snack." << endl;
              break;
            case worked_withChange:
              cout << "Thank you for you payment of " << myMachine.displayMoney(payment) << ". Here is your change of " << change << " and snack." << endl;
              break;
            default:
              cout << "An unknown error occured. " << endl;
              break;
          }
        }
        else
        {
          switch(status)
          {
            case error_insufficentChange:
              cout << "There is not enough change in this machine to accept your payment. Here is your payment of " << myMachine.displayMoney(payment) << "." << endl;
              break;
            case error_insufficientPayment:
              cout << "The payment of " << myMachine.displayMoney(payment) << " was not sufficient. Please try again. " << endl;
              break;
            default:
                cout << "An unknown error occured. " << endl;
               break;

          }
        }
      }
      else
      {
        cout << "There are no snacks in this machine. " << endl;
      }
      break;

    case 2:
      cout << "Amount of inventory to add to machine: ";
      cin >> amountToAdd;
      while(cin.fail())
      {
        cin.clear();
        cin.ignore(50, '\n');
        cout << "You must enter a number. Reenter your choice: ";
        cin >> amountToAdd;
      }
      if(myMachine.incrementInventory(amountToAdd) == true)
      {
        cout << "Inventory is being added.";
        cout << "\n\nInventory Level is now: " << myMachine.getInventoryLevel() << endl;
      }
      else
      {
        cout << "You have exceeded the machines maximum inventory level." << endl;
        cout << "\n\nInventory Level is now: " << myMachine.getInventoryLevel() << endl;
      }
      break;

    case 3:
      cout << "Amount of change to add to machine: ";
      cin >> amountToAdd;
      while(cin.fail())
      {
        cin.clear();
        cin.ignore(50, '\n');
        cout << "You must enter a number. Reenter your choice: ";
        cin >> amountToAdd;
      }
      if(myMachine.incrementChangeLevel(amountToAdd) == true)
      {
        cout << "Change is being added.";
        cout << "\n\nChange Level is now: " << myMachine.getChangeLevel() << endl;
      }
      else
      {
        cout << "You have exceeded the machines maximum change level." << endl;
        cout << "\n\nChange Level is now: " << myMachine.getChangeLevel() << endl;
      }
      break;

    case 4:
      cout << "\nCurrent Inventory Level : " << myMachine.getInventoryLevel() << endl;
      cout << "Max Inventory Capacity  : " << myMachine.getMaxInventoryCapacity() << "\n" << endl;

      cout << "Current Change Level    : " << myMachine.getChangeLevel() << endl;
      cout << "Max Change Capacity     : " << myMachine.getMaxChangeCapacity() << "\n" << endl;

      cout << "Current Snack Price is  : " << myMachine.getSnackPrice() <<endl;
      break;

    default:
      cout << "Make a vaild selection";
  }
}

int main()
{
  int choice = 23;
  SnackMachine myMachine("Snack Machine", 50, 500, 100);
  while(choice != 0)
  {
    displayMenu(choice, myMachine);
    userDecision(choice, myMachine);
  }
  return 0;
}
