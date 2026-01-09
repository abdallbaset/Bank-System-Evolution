#pragma once
#include "clsUser.h"
#include  "clsScreen.h"
#include "clsInputValidate.h"
#include "Global.h"
#include "clsMainScreen.h"
using namespace std;
class clsLoginScreen:protected clsScreen
{
private:
	void static _Login()
	{
		bool LoginFaild = false;
		string UserName = "";
		string PassWord = "";

		do
		{
			if (LoginFaild)
			{
				cout << "Invlaid UserName/PassWord!\n";
			}

			cout << "\nEnter UserName ? ";
			cin >> UserName;
			cout << "Enter PassWord ? ";
			cin >> PassWord;
			CurrentUser = clsUser::Find(UserName, PassWord);
			LoginFaild = CurrentUser.IsEmpty();

		} while (LoginFaild);

		clsMainScreen::ShowMainMenu();
	
	}
public:
	static void showLoginScreen()
	{
		system("cls");
		_DrawScreenHeader("Login Screen");
		_Login();
	}
};

