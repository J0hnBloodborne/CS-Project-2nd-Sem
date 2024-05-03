#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<sstream>
#include<cstdio>
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
		stringstream s(line);
        VendingMachineSlot temp;
        string n;
        int q;
        double p;
		getline(s,n,',');
		s >> p;
		s >> q;
        temp.setName(n);
        temp.setPrice(p);
        temp.setQuantity(q);
        slot.push_back(temp);
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
    for (int i = 0; i < slot.size(); i++)
    {
   		writeFile << slot[i].getName() << ","<< slot[i].getPrice() << "," << slot[i].getQuantity() << endl;
	}
	writeFile.close();
	cout<<endl;
}
int main()
{
    return 0;
}