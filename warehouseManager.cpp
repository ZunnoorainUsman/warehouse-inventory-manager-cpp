#include<iostream>
#include<fstream>
#include<cstdio>
using namespace std;

void updateInventory(ofstream& file)
{
	int productID, quantity, n;
	cout << "How many u wanna add: ";
	cin >> n;
	for (int i = 0; i < n; ++i)
	{
		cout << "Enter " << i + 1 << endl;
		cout << "Enter Product id: ";
		cin >> productID;
		cout << "Enter quantatiy: ";
		cin >> quantity;
		file << productID << " " << quantity << endl;
		cout << endl;
	}
	cout << "Successfully added" << endl;
}
void syncStock(ifstream& file, int idArray[],  int stockArray[], int& itemCount)
{
	itemCount = 0;
	while (itemCount < 100 && file >> idArray[itemCount])
	{
		file >> stockArray[itemCount];
		itemCount++;
	}
}
void applyAudit(int& qty)
{
	qty -= 2;
}
void applyAudit(int& qty, int customLoss)
{
	qty -= customLoss;
}
void syncArrayToFile(ofstream& fout, int idArray[], int stockArray[], int count) {
	for (int i = 0; i < count; i++) {
		fout << idArray[i] << " " << stockArray[i] << endl;
	}
}
void DeleteInventory(ofstream& file, ifstream& fin,int target)
{
	int productID, quantity;
	while (fin >> productID)
	{
		fin >> quantity;
		if (target == productID)
		{
			cout << endl;
			cout << productID << " " << quantity << endl;
			cout << "This record is deleted." << endl;
			continue;
		}
		file << productID << " " << quantity << endl;
	}
	cout << "Successfully deleted" << endl;
}
int main()
{
	int ID[100], Stock[100];
	int count = 0;
	int choice;
	do
	{
		cout << "======Warehouse Manager======" << endl;
		cout << "1. Add new shipment" << endl;
		cout << "2. View Stock report" << endl;
		cout << "3.Run Safety Audit" << endl;
		cout << "4.Delete by ID" << endl;
		cout << "5.Cancel" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1)
		{
			cout << "==== Add new shipment Selected ====" << endl;
			ofstream fapp("warehouse.txt", ios::app);
			if (fapp.is_open())
			{
				updateInventory(fapp);
				fapp.close();
			}
		}
		else if (choice == 2)
		{
			cout << "==== View Stock report Selected ====" << endl;
			cout << endl;
			ifstream fin("warehouse.txt");
			if (fin.is_open())
			{
				syncStock(fin, ID, Stock, count);
				fin.close();
			}
			else
			{
				cout << "error" << endl;
				return 1;
			}
			cout << "======== STOCK REPORT ========" << endl;
			cout << "Index  | Product ID | Quantity" << endl;
			cout << "------------------------------" << endl;
			for (int i = 0; i < count; ++i)
			{
				cout << "Item " << i + 1 << ":| " << ID[i] << "        | " << Stock[i] << endl;
			}
			cout << endl;
			cout << "Total items: " << count << endl;
		}
		else if (choice == 3)
		{
			ifstream fin_load("warehouse.txt");
			if (fin_load.is_open()) {
				syncStock(fin_load, ID, Stock, count);
				fin_load.close();
			}
			cout << "==== Run Safety Audit Selected ====" << endl;
			int id;
			cout << "Enter product ID  u wanna audit: ";
			cin >> id;
			bool found = false;
			int index;
			for (int i = 0; i < count; ++i)
			{
				if (id == ID[i])
				{
					cout << "current Stock: " << Stock[i] << endl;
					index = i;
					found = true;
					break;
				}
			}
			if (!found)
			{
				cout << "ID dosent exist" << endl;
				continue;
			}

			int auditChoice;
			cout << "Choose audit type: " << endl;
			cout << "1.for standard -2." << endl;
			cout << "2 for custom" << endl;
			cin >> auditChoice;
			if (auditChoice == 1)
			{
				applyAudit(Stock[index]);
				cout << "Standard Audit Applied." << endl;
				cout << "Syncing with file..." << endl;
				cout << "Audit Complete." << endl;
				cout << "New stock for "<<ID[index]<<" is: " << Stock[index] << endl;
			}
			else if (auditChoice == 2)
			{
				int custom;
				cout << "Enter custom amount: ";
				cin >> custom ;
				applyAudit(Stock[index], custom);
				cout << "Custom Audit Applied." << endl;
				cout << "Syncing with file..." << endl;
				cout << "Audit Complete." << endl;
				cout << "New stock for " << ID[index] << " is: " << Stock[index] << endl;

			}
			else
			{
				cout << "wrong choice" << endl;
				continue;
			}
			ofstream fout("warehouse.txt");
			if (fout.is_open())
			{
				syncArrayToFile(fout, ID, Stock, count);
				fout.close();
			}

		}
		else if (choice == 4)
		{
			ifstream fin("warehouse.txt");
			ofstream fout( "temp.txt" );
			int target;
			cout << "Which Id u wanna del: ";
			cin >> target;
			cout << endl;
			if (fin.is_open())
			{
				DeleteInventory(fout, fin, target);
			}
			else
			{
				cout << "file not found";
				continue;
			}
			fin.close();
			fout.close();
			remove("warehouse.txt");
			rename("temp.txt", "warehouse.txt");
		}
		else if (choice == 5)
		{
			cout << "Finished" << endl;
			break;
		}
		else
		{
			cout << "Enter valid request." << endl;
			continue;
		}
	} while (choice != 5);

	return 0;
}