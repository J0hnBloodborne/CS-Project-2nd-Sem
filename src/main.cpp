#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<string>
using namespace std;
class VendingMachineSlot:
{
    private:
        string name;
        int quantity;
        double price;
    public:
        static int slotCount;
        VendingMachineSlot()
        {
            name = "Empty";
            quantity = 0;
            price = 0.0;
            slotCount++;
        }
        VendingMachineSlot(string name, int quantity, double price)
        {
            this->name = name;
            this->quantity = quantity;
            this->price = price;
            slotCount++;
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
};
vector<VendingMachineSlot> slot;
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
        VendingMachineSlot temp;
        string n;
        int q;
        double p;
		getline(ss,n,',');
		ss >> p;
		ss >> q;
        temp.setName(n);
        temp.setPrice(p);
        temp.setQuantity(q);
        slot.push_back(temp);
		end += 1;
	}
	readFile.close();
	return end;
}
int main()
{
    return 0;
}