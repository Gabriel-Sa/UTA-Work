/*
  Gabriel de Sa
  UTA ID: 1001676832
  CSE 1325 Coding Assignment 3
*/
#include "SnackMachine.h"
#include "SnackLib.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
using namespace std;

enum status{
  worked_noChange,
  worked_withChange,
  error_insufficientPayment,
  error_insufficentChange,
  unknown
};

void get_command_line_params(int argv, char* argc[], string& input, string& output)
{
  if(argc[1] == NULL)
  {
    throw "invalid_argument";
  } else if(argc[2] == NULL)
  {
    throw "invalid_argument";
  } else
  {
    input = argc[1];
    output = argc[2];
  }
}

void displayMenu(int& choice, SnackMachine& myMachine)
{
  cout << "\n" << myMachine.getMachineName() <<"\n"<< endl;
  cout << "0. Walk away" << endl;
  cout << "1. Buy a Snack" << endl;
  cout << "2. Restock Machine" << endl;
  cout << "3. Add Change" << endl;
  cout << "4. Display Machine Info" << endl;
  cout << "\n5. Update Machine Name" << endl;
  cout << "6. Update Snack Price" << endl;
  cout << "\nMake a selection: ";
  cin >> choice;
  while(cin.fail())
  {
    cin.clear();
    cin.ignore(50, '\n');
    cout << "You must enter a number. Reenter your choice: ";
    cin >> choice;
  }
  while(choice > 6|| choice < 0)
  {
    cin.clear();
    cin.ignore(50, '\n');
    cout << "You must enter a number between 0 and 6. Reenter your choice: ";
    cin >> choice;
  }
}

void userDecision(const int choice, SnackMachine& myMachine)
{
  int amountToAdd, newSnackPrice;
  int payment, status = unknown;
  string change, newMachineName;
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
      cout << myMachine;
      break;

    case 5:
      cout << "Please enter the machines new name: ";
      cin.clear();
      cin.ignore(50, '\n');
      getline(cin, newMachineName);
      while(cin.fail())
      {
        cin.clear();
        cin.ignore(50, '\n');
        getline(cin, newMachineName);
      }
      myMachine.setMachineName(newMachineName);
      cout << "\nMachine Name has been updated, name is now: " << myMachine.getMachineName() << endl;
      break;

    case 6:
      cout << "\nPlease enter the new snack price: ";
      cin >> newSnackPrice;
      myMachine.setSnackPrice(newSnackPrice);
      cout << "Snack Price has been updated, new price is now: " << myMachine.getSnackPrice() << endl;
      break;

    default:
      cout << "Make a vaild selection";
  }
}

void displayMachines(vector<SnackMachine>& SetOfSnackMachines, int& machineChoice)
{
  int i = 1;
  cout << "0. Exit" << endl;
  for(auto x : SetOfSnackMachines)
  {
    cout << i << ". " << x.getMachineName() << endl;
    i++;
  }
  cout << i << ". Add a new machine" << endl;
  cout << "Please enter your choice: ";
  cin >> machineChoice;
  if(cin.fail())
  {
    cin.clear();
    cin.ignore(50, '\n');
    cout << "You must enter a number, Please enter reenter: ";
    cin >> machineChoice;
  }
  if(machineChoice > i || machineChoice < 0)
  {
    cin.clear();
    cin.ignore(50, '\n');
    cout << "Plase enter a number between 0 and " << i << ": ";
    cin >> machineChoice;
  }
}

int main(int argv, char* argc[])
{
  int choice = 23, machineChoice = 23;
  string input, output, SnackMachineLine, giveBack;
  try
  {
    get_command_line_params(argv, argc, input, output);
  }
  catch(const char *message)
  {
    cout << "Throw sent a message: " << message <<"\nMissing Command line parameters - - Usuage: input^filename output^filename" << endl;
  }
  ifstream SnackInputFile(input, ios::in);
  ofstream SnackOutputFile(output, ios::out);
  if(SnackInputFile.is_open())
  {
    vector<SnackMachine> SetOfSnackMachines;
    vector<string> ConstructorValues(4);
    while(getline(SnackInputFile, SnackMachineLine))
    {
      if(ParseSnackLine(SnackMachineLine, ConstructorValues) == true)
      {
        SnackMachine temp{ConstructorValues.at(0), atoi(ConstructorValues.at(1).c_str()), atoi(ConstructorValues.at(2).c_str()), atoi(ConstructorValues.at(3).c_str())};
        SetOfSnackMachines.push_back(temp);
      }
    }
    do
    {
      choice = 23;
      machineChoice = 23;
      displayMachines(SetOfSnackMachines, machineChoice);
      if(machineChoice != 0 && (machineChoice-1) < SetOfSnackMachines.size())
      {
        while(choice != 0)
        {
          displayMenu(choice, SetOfSnackMachines.at(machineChoice-1));
          userDecision(choice, SetOfSnackMachines.at(machineChoice-1));
        }
      } else if (machineChoice == (SetOfSnackMachines.size()+1))
      {
        SnackMachine tempAdd;
        SetOfSnackMachines.push_back(tempAdd);
        cout << "New Machine has been added. " << endl;
      }
    } while(machineChoice != 0);
    //write to file.
    for(auto x : SetOfSnackMachines)
      SnackOutputFile << CreateSnackOutputLine(x, giveBack) << endl;
  }
  else
  {
    cout << "Unable to open file." << endl;
  }
  return 0;
}
