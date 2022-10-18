#include "House.h"
#include "TrickOrTreater.h"
#include <fstream>
#include <cstring>
using namespace std;

void get_command_line_params(int argc, char* argv[], string& tot, string& houses, string& candies)
{
  if(argv[1] == NULL)
  {
    throw "invalid_argument: ToT file missing. ";
  } else if(argv[2] == NULL)
  {
    throw "invalid_argument: houses file missing";
  } else if(argv[3] == NULL)
  {
    throw "invalid_argument: candy file missing. ";
  }
  {
    tot = argv[1];
    houses = argv[2];
    candies = argv[3];
  }
}

void open_candyRanking(string candiesFile, map<int, string>& candyRanking)
{
  ifstream candyFile(candiesFile);
  string line;
  char * holder;
  int quantity;
  string type;
  while(getline(candyFile, line))
  {
    char token[line.size() + 1];
    strcpy(token, line.c_str());
    holder = strtok(token, "|");
    //Put into values into map
    type = holder;
    holder = strtok(NULL, "|");
    quantity = atoi(holder);
    holder = strtok(NULL, "|");
    candyRanking.insert(make_pair(quantity, type));
  }
  candyFile.close();
}

void open_houses(string housesFile, vector<House*>& houses, map<int, string> candyRanking, vector<string>& houseNames)
{
  ifstream houseFile(housesFile);
  string name;
  while(getline(houseFile, name))
  {
    houseNames.push_back(name);
    House* temp = new House{name, candyRanking};
    houses.push_back(temp);
  }
  houseFile.close();
}

void open_ToTs(string totsFile, vector<TrickOrTreater>& ToTs, vector<House*>& houses)
{
  ifstream totFiles(totsFile);
  string name;
  while(getline(totFiles, name))
  {
    ToTs.push_back(TrickOrTreater{name, houses});
  }
  totFiles.close();
}

void printHeader(vector<string>& houseNames)
{
  int i = 0, length;
  for(i = 0; i < 11; i++)
    cout << " ";
  for(auto x : houseNames)
  {
    cout << x;
    length = 11 - x.length();
    for(i = 0; i <= length; i++)
      cout << " ";
  }
  cout << " " << endl;
}

void delete_allocated(vector<House*>& houses)
{
  for(int i = 0; i < houses.size(); i++)
    delete(houses[i]);
}

int main(int argc, char* argv[])
{
  srand(time(NULL));
  string totFile, housesFile, candiesFile;
  map<int, string> candyRanking;
  vector<House*> houses;
  vector<string> houseNames;
  vector<TrickOrTreater> ToTs;
  try
  {
    get_command_line_params(argc, argv, totFile, housesFile, candiesFile);
  }
  catch(const char * message)
  {
    cout << "get_command_line_params threw an error - " << message << endl;
  }
  open_candyRanking(candiesFile, candyRanking);
  open_houses(housesFile, houses, candyRanking, houseNames);
  open_ToTs(totFile, ToTs, houses);
  for(auto x : candyRanking)
    cout << x.first <<" "<< x.second << endl;
  for(auto& x : ToTs)
  {
    x.startThread();
  }
  while(TrickOrTreater::ImDone != ToTs.size())
  {
   // for(int i = 0; i < 34; i++)
     // cout << "\n";
    system("clear");
    printHeader(houseNames);

    for(auto& x : ToTs)
    {
      cout << x.getPath() << x.getName() << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(5));
  }
  for(auto& x : ToTs)
  {
    cout << x << endl;
  }
  for(auto& x : ToTs)
  {
    x.joinThread();
  }
  delete_allocated(houses);
  return 0;
}
