#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "clsPerson.h"
#include "clsString.h"
#include "clsUtil.h"
using namespace std;
class clsBankClient :public clsPerson
{
private:
	enum enMode {EmptyMode = 0,UpdateMode=1,AddNewMode = 2};
	enMode _Mode;
	string _AccountNumber;
	string _PinCode;
	float _AccountBalance;
	bool _MarkForDelete= false;
	short _Permission = 0;


	static clsBankClient _ConvertLinetoClientObject(string LineData , string Sepertor ="#//#")
	{
		vector<string> vWord = clsString::Split(LineData, Sepertor);
		return clsBankClient(enMode::UpdateMode,vWord[0], vWord[1], vWord[2], vWord[3], vWord[4], vWord[5], stod(vWord[6]));
		
	}
	static clsBankClient _GetEmptyClientObject()
	{
		return  clsBankClient(enMode::EmptyMode, " ", " ", " ", " ", " ", " ", 0);
	}
	static vector<clsBankClient> _LoadClientDataFromFaile()
	{
		vector<clsBankClient>vClients;
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);
		string line = "";
		if (MyFile.is_open())
		{
			while (getline(MyFile, line))
			{
				clsBankClient ClientInfo = _ConvertLinetoClientObject(line);
				vClients.push_back(ClientInfo);
			}
			MyFile.close();
		}
		return vClients; 
	}
	static string _ConvertClientObjectToline(clsBankClient ClientInfo, string Separator = "#//#")
	{
		string Line = "";
		Line += ClientInfo.FirstName + Separator;
		Line += ClientInfo.LastName + Separator;
		Line += ClientInfo.Email + Separator;
		Line += ClientInfo.Phone + Separator;
		Line += ClientInfo.AccountNumber + Separator;
		Line += ClientInfo.PinCode + Separator;
		Line += to_string(ClientInfo.AccountBalance);

		return Line;
	}
	static void _SaveClientsDataToFile(vector<clsBankClient> vClients)
	{
		string LineData = "";
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out);
		if (MyFile.is_open())
		{
			for (clsBankClient & client : vClients)
			{
				if (client.MarkForDelete == false)
				{
					LineData = _ConvertClientObjectToline(client);
					MyFile << LineData << endl;
				}
			}
			MyFile.close();
		}
	}
	void _Update()
	{
		vector<clsBankClient> vClients;
		vClients = _LoadClientDataFromFaile();
		for (clsBankClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client = *this;
				break;
			}
		}

		_SaveClientsDataToFile(vClients);
	}
	void _AddDataLineToFile(string DataLine)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::app | ios::out);
		if (MyFile.is_open())
		{
			MyFile << DataLine << endl;
			MyFile.close();
		}
	}
	void _AddNew()
	{
		_AddDataLineToFile(_ConvertClientObjectToline(*this));
	}
public:
	
	clsBankClient(enMode Mode,string FirstName, string LastName, string Email, string Phone, string AccountNumber, string PinCode, float AccountBalance) :
		clsPerson(FirstName, LastName, Email, Phone)
	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
	   _AccountBalance = AccountBalance;
	}

	bool IsEmpty()
	{
		return (_Mode == enMode::EmptyMode);
	}
	void setMarkForDelete(bool Mark)
	{
		 _MarkForDelete = Mark;
	}
	bool GetMarkForDelete()
	{
		return _MarkForDelete;
	}
	__declspec(property(get = GetMarkForDelete,put = setMarkForDelete)) bool MarkForDelete;

	string GetAccountNumber()
	{
		return _AccountNumber;
	}
	__declspec(property(get = GetAccountNumber)) string AccountNumber;


	void SetAccountBalance(float AccountBalance)
	{
		 _AccountBalance = AccountBalance;
	}
	float GetAccountBalance()
	{
		return _AccountBalance;
	}
	__declspec(property(get = GetAccountBalance , put = SetAccountBalance)) float AccountBalance;

	void SetPinCode(string PinCode)
	{
		 _PinCode = PinCode;
	}
	string GetPinCode()
	{
		return _PinCode;
	}
	__declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

	static clsBankClient Find(string AccountNumber)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);
		if (MyFile.is_open())
		{
			string line ;
			while (getline(MyFile, line))
			{
				clsBankClient Client = _ConvertLinetoClientObject(line);
				if (Client.AccountNumber == AccountNumber)
				{
					MyFile.close();
					return Client;
				}
			}
				MyFile.close();

		}
		return _GetEmptyClientObject();
	}

	static clsBankClient Find(string AccountNumber,string PinCode)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);
		if (MyFile.is_open())
		{
			string line ;
			while (getline(MyFile, line))
			{
				clsBankClient Client = _ConvertLinetoClientObject(line);
				if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode)
				{
					MyFile.close();
					return Client;
				}
			}
				MyFile.close();

		}
		return _GetEmptyClientObject();
	}

	static bool IsClientExist(string AccountNumber)
	{
		clsBankClient Client =clsBankClient:: Find(AccountNumber);
		return (!Client.IsEmpty());
	}

	bool Transfer(float Amount, clsBankClient& DestinationClinet)
	{
		if (Amount > AccountBalance)
		{
			return false;
		}
		else
		{
			Withdraw(Amount);
			DestinationClinet.Deposit(Amount);
			return true;
		}
	}
	static clsBankClient GetAddNewClientObject(string AccoutNumber)
	{
		return clsBankClient(enMode::AddNewMode, "", "", "", "", AccoutNumber,"", 0);
	}
	static vector<clsBankClient> GetClientsList()
	{
		return _LoadClientDataFromFaile();
	}
	

	bool Delete()
	{
		vector<clsBankClient> vClients;
		vClients = _LoadClientDataFromFaile();
		for (clsBankClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.MarkForDelete =true;
				break;
			}
		}

		_SaveClientsDataToFile(vClients);
		*this = _GetEmptyClientObject();
		return true;
	}

	static double GetTotalBlances()
	{
		vector<clsBankClient> vClients = GetClientsList();
		double TotalBlances = 0.0;
		for (clsBankClient& Client : vClients)
		{
			TotalBlances += Client.AccountBalance;
		}
		return TotalBlances;
	}

	void Deposit(double Amount)
	{
		AccountBalance += Amount;
		Save();
	}

	void Withdraw(double Amount)
	{
		AccountBalance -= Amount;
		Save();
	}
	enum enSaveResult { svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };

	enSaveResult Save()
	{
		switch (_Mode)
		{
		case enMode::EmptyMode:	return enSaveResult::svFaildEmptyObject;

		case enMode ::UpdateMode:
		{
			_Update();
			return enSaveResult::svSucceeded;
		}
		case enMode ::AddNewMode:
		{
			if (clsBankClient::IsClientExist(_AccountNumber))
				return enSaveResult::svFaildAccountNumberExists;

			_AddNew();
			_Mode = enMode::UpdateMode;
			return enSaveResult::svSucceeded;
		}
		

		}
	}
};

