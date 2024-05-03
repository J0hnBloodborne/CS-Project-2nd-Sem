#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdio>
#include<cstdlib>
#include<windows.h>
using namespace std;
struct VendingMachineSlot
{
	string name[24] = {"Empty"};
	double price[24]={0};
	int quantity[24]={0};
} item;
void initializeStructure()
{
	for (int i = 0;i<24;i++) item.name[i] = "Empty";
}
int readFromFile()
{
	int end = 1;
	ifstream readFile("Vending Machine Data.txt");
	if (!readFile.is_open())
		{
			cerr<<"Error reading file."<<endl<<"Exiting...";
			exit(1);
		}
	string line;
	while (getline(readFile,line)) 
	{
		stringstream ss(line);
		getline(ss,item.name[end-1],',');
		ss >> item.price[end-1];
		ss >> item.quantity[end-1];
		end += 1;
	}
	readFile.close();
	return end;
}
void writeToFile(int stackPtr)
{
	int x = remove("Vending Machine Data.txt");
	ofstream writeFile;
   	writeFile.open("Vending Machine Data.txt");
    for (int i = 0; i < stackPtr - 1; i++)
    {
   		writeFile << item.name[i] << endl<< item.price[i] << endl << item.quantity[i] << endl;
	}
	writeFile.close();
	cout<<endl;
}
void MaintenanceMode()
{
	int choice;
	bool exitflag = false;
	cout<<"----------------"<<endl<<"MAINTENANCE MODE"<<endl<<"----------------"<<endl;
	do
	{
		cout<<"1. Restock\n2. Restock Single Item\n3. Stock new item\nEnter any other value to exit\nEnter choice: ";cin>>choice;
		switch(choice)
		{
			case 1:
			{
				int stackPtr = readFromFile();
				if (stackPtr == 1)
				{
					cout<<"Nothing to restock."<<endl;
					break;
				}
				else
				{
					for (int i = 0; i < stackPtr; i++)
					{
						item.quantity[i] = 10;
					}
					writeToFile(stackPtr);
				}	
				break;
			}
			case 2:
			{
				int stackPtr = readFromFile();
				if (stackPtr == 1)
				{
					cout<<"Nothing to restock."<<endl;
					break;
				}
				else
				{
					for (int i = 1; i <  24; i++)
					{
						cout<<setw(2)<<setfill('0')<<i<<" "<<item.name[i-1]<<" - $"<<item.price[i-1]<<"  Quantity: "<<item.quantity[i-1]<<endl;
					}
					int id,qty;
					cout<<"Enter item id to restock: ";cin>>id;
					if (id > stackPtr || id < 1)
					{
						cout<<"Empty or Invalid ID."<<endl;
						break;
					}
					cout<<"How much would you like to restock: ";cin>>qty;
					if (qty > 10-item.quantity[id-1])
					{
						cout<<"You cannot put more than 10 items in one slot. Moving to empty slots."<<endl;
						if (stackPtr == 24)
						{
							cout<<"Not enough space in vending machine."<<endl;
							break;
						}
						else
						{
							int freeSlots = 24 - stackPtr;
							cout<<freeSlots+1<<" available. Max free capacity = "<<(freeSlots*10 + 10 - item.quantity[id-1])<<endl<<"Enter quantity again: ";cin>>qty;
							if (qty > (freeSlots*10+10-item.quantity[id-1]))
							{
								cout<<"Not enough space in vending machine."<<endl;
								break;
							}
							else
							{
								while (qty > 10 && freeSlots != 0)
								{
									item.quantity [id-1] = 10;
									item.name[stackPtr-1] = item.name[id-1];
									item.price[stackPtr-1] = item.price[id-1];
									qty -= 10;
									item.quantity[stackPtr-1] = qty;
									stackPtr += 1;
									freeSlots -= 1;
								}
							}
						}
					}
					else if (qty < 1)
					{
						cout<<"Invalid input."<<endl;
						break;
					}
					else
					{
						item.quantity[id-1] += qty;
					}
					writeToFile(stackPtr);
					break;
				}
			}	
			case 3 : 
			{
				int stackPtr = readFromFile();
				int freeSlots = 24 - stackPtr;
				if(freeSlots == 0)
				{
					cout<<"No more space in vending machine.";
				}
				else
				{
					string name;
					double price;
					int qty;
					bool alreadyExists = false;
					cout<<"Enter name: ";cin.ignore();
					getline(cin,name);
					for (int i = 0; i < stackPtr; i++)
					{
						if (name == item.name[i])
						{
							alreadyExists = true;
							break;
						}
					}
					if (alreadyExists)
					{
						cout<<"Item already in vending machine. Use restock option.";
						break;
					}
					item.name[stackPtr-1] = name;
					cout<<"Enter price: ";cin>>price;
					if (price < 0.01)
					{
						cout<<"Invalid input.";
						break;
					}
					else
					{
						item.price[stackPtr-1] = price;
					}
					cout<<"Enter quantity: ";cin>>qty;
					if (qty > 10)
					{
						cout<<"You cannot put more than 10 items in one slot. Moving to empty slots.";
						if (stackPtr == 24)
						{
							cout<<"Not enough space in vending machine."<<endl;
							item.name[stackPtr-1] = "Empty";
							item.price[stackPtr-1] = 0;
							break;
						}
						else
						{
							int freeSlots = 24 - stackPtr;
							cout<<freeSlots+1<<" slots available. Max free capacity = "<<(freeSlots*10 + 10 - item.quantity[stackPtr-1])<<endl<<"Enter quantity again: ";cin>>qty;
							if (qty > freeSlots*10)
							{
								cout<<"Not enough space in vending machine."<<endl;
								item.name[stackPtr-1] = "Empty";
								item.price[stackPtr-1] = 0;
								break;
							}
							else
							{
								while (qty > 10 && freeSlots != 0)
								{
									item.quantity[stackPtr-1] = 10;
									item.name[stackPtr-1] = name;
									item.price[stackPtr-1] = price;
									qty -= 10;
									stackPtr += 1;
									freeSlots -= 1;
								}
								item.name[stackPtr-1] = name;
								item.price[stackPtr-1] = price;
								item.quantity[stackPtr-1] = qty;
							}
						}
					}
					else if (qty < 1)
					{
						cout<<"Invalid Input."<<endl;
						break;
					}
					else
					{
						item.quantity[stackPtr-1] += qty;
						stackPtr += 1;
					}
				    writeToFile(stackPtr);
					break;
				}
				break;
			}
			default :
			{
				cout<<"Exiting Maintenance Mode."<<endl;
				Sleep(1000);
				system("CLS");
				exitflag = true;
				break;	
			}	
		}	
	}
	while (exitflag = false);
}
void UserMode()
{
	cout<<"----------------"<<endl<<"VENDING MACHINE"<<endl<<"----------------"<<endl;	
	int stackPtr = readFromFile();
	if (stackPtr == 1)
	{
		cout<<"Vending machine empty."<<endl;
	}
	else
	{
		for (int i = 1; i <= 24; i++)
	{
		cout<<setw(2)<<setfill('0')<<i<<" "<<item.name[i-1]<<" - $"<<item.price[i-1]<<"\t|\t";
		if (i % 3== 0) cout<<endl;
	}
	int id;
	cout<<"Enter item id: ";cin>>id;
	if (id > stackPtr || id < 1)
	{
		cout<<"Empty or Invalid ID."<<endl;
	}
	else
	{
		char cancel;
		cout<<"Are you sure you want "<<item.name[id - 1]<<" costing $"<<item.price[id-1]<<" (y/n)?: ";cin>>cancel;
		if (cancel != 'y') 
		{
			system("CLS");
			UserMode();	
		}
		else
		{
			cout<<"Enter money.\t";
			Sleep(1000);
			cout<<"Money entered."<<endl;
			cout<<"You purchased "<<item.name[id-1];
			Sleep(1500);
			item.quantity[id-1] -= 1;
			if (item.quantity[id-1] == 0)
			{
				for (int i = id - 1; i <= stackPtr; i++)
				{
					item.name[i-1]=item.name[i];
					item.price[i-1]=item.price[i];
					item.quantity[i-1]=item.quantity[i];
				}
			}
			writeToFile(stackPtr);
			system("CLS");
		}
	}
	}
}
int main()
{
	initializeStructure();
	ofstream writeFile;
    writeFile.open("Vending Machine Data.txt");
	if (!writeFile.is_open())
 	{
		cout<<"Error reading file/ file does not exist.";
	}
	else
	{
		cout<<"Loaded Vending Machine data."<<endl;
	}
	writeFile.close();
	int Code = 1;
	while (Code!=0)
	{
		cout<<"Enter any number to start (0 to exit): ";cin>>Code;
		if (Code == 177013) MaintenanceMode();
		else if (Code == 0) break;
		else UserMode();
	}
}
