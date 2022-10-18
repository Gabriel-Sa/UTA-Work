/*
  Gabriel de Sa
  Student ID: 1001676832
  Coding Assignment 1: Pencil Machine
*/
#include <iostream>
#include <string>
#define PENCILPRICE 100
using namespace std;

enum False
{
  inventory_insufficient,
  inventory_insufficient_0,
  not_enough_change,
  payment_insufficient,
  unknown
};
enum True
{
  exact_payment,
  payd_w_change
};

string displayMoney(int amount)
{
  string dollars{to_string(amount/100)};
  string cents{to_string(abs(amount%100))};
  string dollars_cents = "$" + dollars + "." + (cents.size() == 1 ? "0" : "") + cents;
  cout << dollars_cents;
  return "";
}

bool buyPencil(int& pencilLevel, int& changeLevel, int& status, int& pencilNum, int& paymentAmount, int& changeDue, int& amountDue)
{
  bool worked = false;
  amountDue = pencilNum * PENCILPRICE;
  // initially test to see if the user entered the correct amount of inventory.
  if(pencilNum > pencilLevel)
  {
    status = inventory_insufficient;
  }
  else if(pencilNum <= 0)
  {
    status = inventory_insufficient_0;
  }
  else
  {
    // all false status first:
    if(paymentAmount > changeLevel && paymentAmount != (amountDue))
    {
      status = not_enough_change;
    }

    else if(paymentAmount < (amountDue))
    {
      status = payment_insufficient;
    }
    // all true status
    else if(paymentAmount == (amountDue))
    {
      status = exact_payment;
      pencilLevel = pencilLevel - pencilNum;
      changeLevel = changeLevel + paymentAmount;
      worked = true;
    }

    else if(paymentAmount > (amountDue))
    {
      status = payd_w_change;
      changeDue = paymentAmount - amountDue;
      pencilLevel = pencilLevel - pencilNum;
      changeLevel = changeLevel + (paymentAmount - changeDue);
      worked = true;
    }
    else
    {
      status = unknown;
    }
  }

  return worked;
}

int menu()
{
  int choice = 22; // random number to make sure it fails.
  cout << "\nPlease Select One of the Options Below:" << endl;
  cout << "0. Quit" << endl;
  cout << "1. Buy a Pencil" << endl;
  cout << "2. Check Inventory Level" << endl;
  cout << "3. Check Change Level" << endl;
  cout<< "Choice : ";
  cin >> choice;
  if(cin.fail())
  {
    cin.clear();
    cin.ignore(50, '\n');
    return 5;
  }

  return choice;
}

int main()
{
  int pencilLevel = 100;
  int changeLevel = 500;
  int amountDue = 0;
  int pencilNum = 0;
  int changeDue = 0;
  int paymentAmount = 0;
  int status = 20;
  int choice = 5;
  cout << "Welcome to the UTA Pencil Machine!" << endl;

  while(choice != 0)
  {
    choice = menu();

    switch(choice)
    {
      case 0:
        break;
      case 1:
        if(pencilLevel == 0)
        {
          cout << "Sorry, but we are currently out of pencils. " << endl;
          continue;
        }
        cout << "A pencil costs " << displayMoney(PENCILPRICE) << endl;
        cout << "How many pencils would you like to buy: ";
        cin >> pencilNum;
        while(cin.fail())
        {
          cin.clear();
          cin.ignore(50, '\n');
          cout << "Please enter a number: ";
          cin >> pencilNum;
        }
        if(buyPencil(pencilLevel, changeLevel, status, pencilNum, paymentAmount, changeDue, amountDue) == false)
        { // This makes sure that the user entered a actual number of pencils to buy, if not it'll skip the rest of the loop.
          switch(status)
          {
            case inventory_insufficient:
              cout << "Sorry, but you can only purchase a maximum of " << pencilLevel << " pencils." << endl;
              continue;
              break;
            case inventory_insufficient_0:
              cout << "You must at least purchase one pencil." << endl;
              continue;
              break;
          }
        }
        cout << "That'll be " << displayMoney(amountDue) << endl;
        cout << "Please enter your payment (in cents): ";
        cin >> paymentAmount;
        while(cin.fail())
        {
          cin.clear();
          cin.ignore(50, '\n');
          cout << "Enter payment in cents: ";
          cin >> paymentAmount;
        }
        cin.clear();
        cin.ignore(50, '\n');
        if(buyPencil(pencilLevel, changeLevel, status, pencilNum, paymentAmount, changeDue, amountDue) == true)
        {
          // true status
          switch(status)
          {
            case exact_payment: // exact payment was given
              cout << "Thank you for your payment!\n" << "Dispensing " << pencilNum << " pencils!" << endl;
              break;
            case payd_w_change: // payment given with change.
              cout << "Thank you for your payment of " << displayMoney(paymentAmount) << "\nHere is your change of " << displayMoney(changeDue) << endl;
              cout << "Dispensing " << pencilNum << " pencils!" << endl;
              break;
            default:
              break;
          }
        }
        else
        {
          // false status
          switch(status)
          {
            case not_enough_change: //no change avaliable for this transaction, no transaction completed
              cout << "We're sorry we cannot accept you payment. There is not enough change in the machine." << endl;
              break;
            case payment_insufficient: //payment insufficient
              cout << "Sorry but the amount you gave us was insufficient, pencils were not dispensed." << endl;
              break;
            case unknown: // unknown error occured. try again
              cout << "Sorry, but an unknown error occured. Please try again. " << endl;
              break;
          }
        }
        break;
      case 2:
        cout << "Current inventory level is: " << pencilLevel << endl;
        break;
      case 3:
        cout << "Current change level is: " << displayMoney(changeLevel) << endl;
        break;
      default:
        cout << "Please make a vaild selection." << endl;
    }
  }
  return 0;
}
