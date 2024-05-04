#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<iomanip>
#include<raylib.h>
#include<windows.h>
using namespace std;
class VendingMachineSlot
{
    private:
        string name;
        int quantity;
        double price;
    public:
        // static int slotCount;
        VendingMachineSlot()
        {
            name = "Empty";
            quantity = 0;
            price = 0.0;
            // slotCount++;
        }
        VendingMachineSlot(string name, int quantity, double price)
        {
            this->name = name;
            this->quantity = quantity;
            this->price = price;
            // slotCount++;
        }
        string getName()
        {
            return name;
        }
        int getQuantity()
        {
            return quantity;
        }
        double getPrice()
        {
            return price;
        }
        void setName(string name)
        {
            this->name = name;
        }
        void setQuantity(int quantity)
        {
            this->quantity = quantity;
        }
        void setPrice(double price)
        {
            this->price = price;
        }
        void operator +=(int x)
        {
            quantity += x;
        }
};
vector<VendingMachineSlot> slot;
void readFromFile()
{
	ifstream readFile("Vending Machine Data.txt");
	if (!readFile.is_open())
		{
			cerr<<"Error reading file."<<endl<<"Exiting...";
			exit(1);
		}
    string n;
    int q;
    double p;
	while (readFile >> n >> p >> q) 
	{
        VendingMachineSlot temp;
        temp.setName(n);
        temp.setPrice(p);
        temp.setQuantity(q);
        slot.push_back(temp);
	}
	readFile.close();
}
void writeToFile()
{
	int x = remove("Vending Machine Data.txt");
	ofstream writeFile;
   	writeFile.open("Vending Machine Data.txt");
    for (int i = 0; i < slot.size(); i++)
    {
   		writeFile << slot[i].getName() << endl << slot[i].getPrice() << endl << slot[i].getQuantity() << endl;
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
		cin.ignore();
		cout<<"1. Restock\n2. Restock Single Item\n3. Stock new item\nEnter any other value to exit\nEnter choice: ";cin>>choice;
		switch(choice)
		{
			case 1:
			{
				readFromFile();
				if (slot.empty())
				{
					cout<<"Nothing to restock."<<endl;
					break;
				}
				else
				{
					for (int i = 0; i < slot.size(); i++)
					{
						slot[i].setQuantity(10);
					}
					writeToFile();
				}	
				break;
			}
			case 2:
			{
				readFromFile();
				if (slot.empty())
				{
					cout<<"Nothing to restock."<<endl;
					break;
				}
				else
				{
					for (int i = 1; i <  24; i++)
					{
						cout<<setw(2)<<setfill('0')<<i<<" "<<slot[i-1].getName()<<" - $"<<slot[i-1].getPrice()<<"  Quantity: "<<slot[i-1].getQuantity()<<endl;
					}
					int id,qty;
					cout<<"Enter item id to restock: ";cin>>id;
					if (id > slot.size() || id < 1)
					{
						cout<<"Empty or Invalid ID."<<endl;
						break;
					}
					cout<<"How much would you like to restock: ";cin>>qty;
					if (qty > 10-slot[id-1].getQuantity())
					{
						cout<<"You cannot put more than 10 items in one slot. Moving to empty slots."<<endl;
						if (slot.size() >= 24)
						{
							cout<<"Not enough space in vending machine."<<endl;
							break;
						}
						else
						{
							int freeSlots = 24 - slot.size();
							cout<<freeSlots+1<<" available. Max free capacity = "<<(freeSlots*10 + 10 - slot[id-1].getQuantity())<<endl<<"Enter quantity again: ";cin>>qty;
							if (qty > (freeSlots*10+10-slot[id-1].getQuantity()))
							{
								cout<<"Not enough space in vending machine."<<endl;
								break;
							}
							else
							{
								while (qty > 10 && freeSlots != 0)
								{
                                    string n;
                                    int q;
                                    double p;
									q = 10;
									n = slot[id-1].getName();
									p = slot[id-1].getPrice();
									qty -= 10;
                                    VendingMachineSlot temp(n,p,q);
                                    slot.push_back(temp);
                                    vector<VendingMachineSlot>::iterator end = slot.end();
									(*end).setQuantity(qty);
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
						slot[id-1]+=qty;
					}
					writeToFile();
					break;
				}
			}	
			case 3 : 
			{
				readFromFile();
				int freeSlots = 24 - slot.size();
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
					for (int i = 0; i < slot.size(); i++)
					{
						if (name == slot[i].getName())
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
                    vector<VendingMachineSlot>::iterator end = slot.end();
					(*end).setName(name);
					cout<<"Enter price: ";cin>>price;
					if (price < 0.01)
					{
						cout<<"Invalid input.";
						break;
					}
					else
					{
						(*end).setPrice(price);
					}
					cout<<"Enter quantity: ";cin>>qty;
					if (qty > 10)
					{
						cout<<"You cannot put more than 10 items in one slot. Moving to empty slots.";
						if (slot.size() == 24)
						{
							cout<<"Not enough space in vending machine."<<endl;
							(*end).setName("Empty");
							(*end).setPrice(0);
							break;
						}
						else
						{
							int freeSlots = 24 - slot.size();
							cout<<freeSlots+1<<" slots available. Max free capacity = "<<freeSlots*10 + 10 - (*end).getQuantity()<<endl<<"Enter quantity again: ";cin>>qty;
							if (qty > freeSlots*10)
							{
								cout<<"Not enough space in vending machine."<<endl;
								(*end).setName("Empty");
								(*end).setPrice(0);
								break;
							}
							else
							{
								while (qty > 10 && freeSlots != 0)
								{
                                    end = slot.end();
									(*end).setQuantity(10);
								    (*end).setName(name);
									(*end).setPrice(price);
                                    VendingMachineSlot temp("Empty",0,0);
                                    slot.push_back(temp);
									qty -= 10;
									freeSlots -= 1;
								}
								VendingMachineSlot temp(name,qty,price);
                                slot.push_back(temp);
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
						(*end)+=qty;
					}
				    writeToFile();
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

int main()
{
    return 0;
}